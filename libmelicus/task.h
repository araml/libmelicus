#pragma once

#include <audio_status.hpp>

#include <mutex>
#include <atomic>
#include <condition_variable>


struct task { 
    template <typename callable, typename... Args>
    task(callable fn, Args &&... args) { 
        while (true) { 
            fn(static_cast<Args &&>(args)...);              
        }
    }

    auto query() -> void { 

    }

    auto get() -> melicus::status { 
        return melicus::status{};
    }


private:
    std::atomic<bool> ready, terminate;
    std::mutex mtx;
    std::condition_variable cond;

};

