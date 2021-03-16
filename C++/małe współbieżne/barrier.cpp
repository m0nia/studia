#include <mutex>
#include <thread>
#include <condition_variable>
#include "log.h"

//Monika Michaluk mm395135

class Barrier {
public:
    Barrier(int r) : resistance(r) {}
    void reach(){
        std::unique_lock<std::mutex> lock(mut);
        if(resistance > 1) {
            resistance--;
            barrier.wait(lock, [this] { return resistance == 0; });
        } else if(resistance == 1){
            resistance--;
            //lock.unlock(); // nie zauważyłam różnicy, czy notify_all() wywoływane jest przez wątek
                            // który zajmuje mutex czy nie, wydaje mi się, że nie ma to znaczenia?
            barrier.notify_all();
        }
    }

private:
    int resistance;
    std::mutex mut;
    std::condition_variable barrier;
};

void fun(Barrier& b, int nr){
    log("Próbuję przejść przez barierę, nr. ", nr);
    b.reach();
    log("Przeszedłem przez barierę, nr. ", nr);
}

constexpr int N = 10;
int main(){
    Barrier b(5);
    std::thread threads[N];
    for(int i = 0 ; i < N; i++) {
        threads[i] = std::thread([&b, i] { fun(b, i); });
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    for(auto & thread : threads)
        thread.join();
}