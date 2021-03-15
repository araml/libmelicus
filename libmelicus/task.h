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
                result_available.store(true);
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
        return ready;
    }

    auto query() -> void {
        std::unique_lock<std::mutex> lk(mtx);
        // this could be a problem if we didnt hold the lock
        if (ready || result_available)
            return;
        else 
            internal_query();
    }

    auto wait_for() -> void {
        while (!result_available) {}
    }

    auto get() -> RetVal { 
        std::unique_lock<std::mutex> lk(mtx);
       
        if (ready) { 
            lk.unlock();
            wait_for();
        } else if (!result_available) {
            internal_query();
            lk.unlock();
            wait_for();    
        }
        
        // consume result
        result_available.store(false);
        return return_value;
    }

    std::atomic<bool> ready{false}, result_available{false}, stop{false};
private:
    // Should be called with a lock taken
    auto internal_query() -> void { 
        ready.store(true);
        run.notify_one();
    }

    auto end() -> void { 
        std::lock_guard<std::mutex> lock(mtx);
        stop.store(true);
        ready.store(true);
        run.notify_one();
    }
    
    std::atomic<RetVal> return_value;
    std::thread *runnable;
    std::mutex mtx;
    std::condition_variable run;

};

