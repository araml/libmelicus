#pragma once

#include <audio_status.hpp>

#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>

#include <iostream>

template <typename RetVal>
struct task {
    template <typename Fn, typename... Args>
    task(Fn fn, Args &&... args) {
        runnable = new std::thread([fn, this](Args &&... args) {
            while (true) {
                std::unique_lock<std::mutex> lk(mtx);
                run.wait(lk, [this]() -> bool { return ready; });

                if (stop)
                    break;

                return_value.store(fn(static_cast<Args &&>(args)...)); // fwd
                ready.store(false);
            }
        }, args...);
    }

    ~task() { 
        end();
        if (runnable->joinable())
            runnable->join();
        delete runnable;
    }
    
    auto is_running() -> bool { 
        /*std::unique_lock<std::mutex> lk(mtx, std::try_to_lock);
        if (lk.owns_lock()) { 
            return ready;
        } else { // if we couldn't grab the lock assume its running
            return true;
        }*/
        return ready;
    }

    auto query() -> void {
        std::unique_lock<std::mutex> lk(mtx);
        ready.store(true);
        lk.unlock();
        run.notify_one();
    }

    auto get() -> RetVal { 
        std::unique_lock<std::mutex> lk(mtx);
        //std::cout << "Retval" << std::endl;
        if (ready) { 
            lk.unlock();
            while (ready) {}
        } else { 
            lk.unlock();
            query();
            while (ready) {}
        }
        
        return return_value;
    }

    std::atomic<bool> ready{false}, stop{false};
private:
    auto end() -> void { 
        std::lock_guard<std::mutex> lock(mtx);
        stop.store(true);
        ready.store(true);
        run.notify_one();
    }
    
    std::atomic<RetVal> return_value;
    std::thread *runnable;
    std::mutex mtx, qu;
    std::condition_variable run;

};

