package cp1.solution;

import cp1.base.ResourceId;
import cp1.base.ResourceOperation;

import java.util.*;

// Klasa reprezentujaca aktualny stan danej transakcji
public class Transaction {

    private final long startTime;
    private boolean aborted = false;
    private final Thread startedBy;
    private final Set<ResourceId> accessedResources = new HashSet<>();
    private final Stack<Pair<ResourceOperation, ResourceId>> allOperations = new Stack<>();

    public Transaction(long startTime, Thread startedBy) {
        this.startTime = startTime;
        this.startedBy = startedBy;
    }

    public Thread startedBy() {
        return startedBy;
    }

    public long startTime() {
        return startTime;
    }

    public Set<ResourceId> accessedResources() {
        return accessedResources;
    }

    public void obtainAccess(ResourceId rid) {
        accessedResources.add(rid);
    }

    public boolean hasAccess(ResourceId rid) {
        return accessedResources.contains(rid);
    }

    public boolean isAborted() {
        return aborted;
    }

    public synchronized void abortTransaction() {
        if (!aborted) {
            aborted = true;
            startedBy.interrupt();
        }
    }

    public void addOperation(ResourceId rid, ResourceOperation o) {
        allOperations.push(new Pair<ResourceOperation, ResourceId>(o, rid));
    }

    public Stack<Pair<ResourceOperation, ResourceId>> getAllOperations() {
        return allOperations;
    }

}
