package cp1.solution;

import cp1.base.*;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

public class TransactionManagerBasic implements TransactionManager {

    private final ConcurrentMap<Long, Transaction> transactions = new ConcurrentHashMap<>();
    private final ConcurrentMap<ResourceId, Resource> resources = new ConcurrentHashMap<>();
    private final ConcurrentMap<ResourceId, Long> occupiedResources = new ConcurrentHashMap<>();
    private final ConcurrentMap<Long, ResourceId> waitForMap = new ConcurrentHashMap<>();
    private final ConcurrentMap<ResourceId, Semaphore> resource = new ConcurrentHashMap<>();
    private final LocalTimeProvider timeProvider;

    public TransactionManagerBasic(Collection<Resource> resources, LocalTimeProvider timeProvider) {
        for (Resource r : resources) {
            this.resources.put(r.getId(), r);
            resource.put(r.getId(), new Semaphore(1, true));
        }
        this.timeProvider = timeProvider;
    }

    @Override
    public void startTransaction() throws AnotherTransactionActiveException {
        long currTime = timeProvider.getTime();
        long currentId = Thread.currentThread().getId();
        if (transactions.containsKey(currentId)) {
            throw new AnotherTransactionActiveException();
        }
        transactions.put(currentId, new Transaction(currTime, Thread.currentThread()));
    }

    /*
     * Rywalizacje o wylaczny dostep do zasobow realizuje przez probe podniesienia semaforow odpowiadajacych
     * poszczegolnym zasobom. Jesli watkowi udalo sie przejsc przez odpowiedni semafor, to uznaje, ze dostep do zasobu
     * zostal mu wowczas przyznany, niezaleznie od tego czy (jesli zostal przerwany w miedzyczasie) zdazyl zaktualizowac
     * stan swojej transakcji i mape zajetych zasobow.
     */
    @Override
    public void operateOnResourceInCurrentTransaction(ResourceId rid, ResourceOperation operation) throws NoActiveTransactionException, UnknownResourceIdException, ActiveTransactionAborted, ResourceOperationException, InterruptedException {
        if (!isTransactionActive()) throw new NoActiveTransactionException();
        if (!resources.containsKey(rid)) throw new UnknownResourceIdException(rid);
        if (isTransactionAborted())
            throw new ActiveTransactionAborted(); // jest aborted tylko jesli czeka na zasob wiec nnic sie nie psuje tu
        if (Thread.interrupted()) throw new InterruptedException();
        long current = Thread.currentThread().getId();
        Transaction myTransaction = transactions.get(current);
        boolean hasAccess = false;

        //Jesli ktos mial dostep wczesniej do zasobu, to od razu moze wykonywac operacje na nim..
        if (!myTransaction.hasAccess(rid)) {
            // ..w przeciwnym przypadku rywalizuje o dostep do zasobu:
            try {
                hasAccess = resource.get(rid).tryAcquire(0, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
                throw e;
            }

            if (!hasAccess) {
                //Jesli nie udalo sie uzyskac dostepu, nalezy sprawdzic, czy nie ma zakleszczenia
                waitForMap.put(current, rid);
                Set<Long> cycle = findCycle(current);
                if (!cycle.isEmpty()) { // jesli wystapil cykl, odpowiednia transakcja jest anulowana
                    abortLatestTransaction(cycle);
                }
                try {
                    resource.get(rid).acquire(); // czekanie na zasob
                } catch (InterruptedException e) {
                    if (isTransactionAborted()) {
                        //Thread.currentThread().interrupt(); -- tresc zadania nie mowi jasno, czy ta linijka powinna tu byc
                        throw new ActiveTransactionAborted();
                    }
                    throw e;
                } finally {
                    waitForMap.remove(current, rid);
                }
            }
        }
        occupiedResources.putIfAbsent(rid, current);
        myTransaction.obtainAccess(rid); //jesli juz mial dostep to nic nie zmienia

        operation.execute(resources.get(rid));
        // Jesli operacja zakonczyla sie sukcesem, ale watek zostal przerwany w jej trakcie, to wywolanie metody
        // operateOnResourceInCurrentTransaction konczy sie wyjatkiem InterruptedException nie zmieniajac stanu zasobu
        // -- nalezy wowczas przywrocic stan zasobu sprzed wykonania operacji
        if (Thread.interrupted()) {
            operation.undo(resources.get(rid));
            throw new InterruptedException();
        } else {
            myTransaction.addOperation(rid, operation);
        }
    }

    @Override
    public void commitCurrentTransaction() throws NoActiveTransactionException, ActiveTransactionAborted {
        if (!isTransactionActive()) throw new NoActiveTransactionException();
        if (isTransactionAborted()) throw new ActiveTransactionAborted();
        long current = Thread.currentThread().getId();
        releaseResources(current);
    }

    @Override
    public void rollbackCurrentTransaction() {
        long current = Thread.currentThread().getId();
        if (transactions.containsKey(current)) {
            Stack<Pair<ResourceOperation, ResourceId>> allOperations = transactions.get(current).getAllOperations();
            while (!allOperations.isEmpty()) {
                Pair<ResourceOperation, ResourceId> p = allOperations.pop();
                p.first().undo(resources.get(p.second()));
            }
            releaseResources(current);
        }
    }

    @Override
    public boolean isTransactionActive() {
        return transactions.containsKey(Thread.currentThread().getId());
    }

    @Override
    public boolean isTransactionAborted() {
        if (!transactions.containsKey(Thread.currentThread().getId())) return false;
        return transactions.get(Thread.currentThread().getId()).isAborted();
    }

    /*
     * Metoda wykrywajaca zakleszczenie - sprawdza czy watek startingThread nie nalezy do cyklu w "grafie"
     * watkow czekajacych na zasoby
     */
    private Set<Long> findCycle(Long startingThread) {
        Set<Long> cycle = new HashSet<>();
        Long t = startingThread;
        while (t != null && waitForMap.containsKey(t) && !cycle.contains(t)) {
            cycle.add(t);
            ResourceId rid = waitForMap.get(t);
            if (rid != null) {
                t = occupiedResources.get(rid);
            } else
                t = null;
        }
        if (t != null && t.equals(startingThread))
            return cycle;
        return new HashSet<>();
    }


    /*
     * W przekazanym w paramecie cyklu szuka najmlodszej transakcji i o najwiekszym ID watku i anuluje ja,
     * o ile nie zostala wczesniej anulowana przez inny watek
     */
    private void abortLatestTransaction(Set<Long> cycle) {
        Transaction latest = transactions.get(Thread.currentThread().getId());
        long maxTime = latest.startTime();
        for (Long threadId : cycle) {
            Transaction t = transactions.get(threadId);
            if (t == null) // tak moze sie zdarzyc, gdy powstal cykl i inny watek anulowal juz transakcje i zostala ona wycofana
                return; // wtedy cyklu juz nie ma i nie musimy anulowac transakcji
            long startTime = t.startTime();
            if (startTime > maxTime) {
                latest = t;
                maxTime = startTime;
            } else if (startTime == maxTime && t.startedBy().getId() > latest.startedBy().getId()) {
                latest = t;
                maxTime = startTime;
            }
        }
        latest.abortTransaction();
    }

    private void releaseResources(long current) {
        Set<ResourceId> myResources = transactions.get(current).accessedResources();

        for (ResourceId r : myResources) {
            occupiedResources.remove(r);
        }
        transactions.remove(current);

        for (ResourceId r : myResources) {
            resource.get(r).release();
        }
    }
}