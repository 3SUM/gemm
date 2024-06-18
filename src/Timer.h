#pragma once

#include <chrono>
#include <stdexcept>

class Timer {
   private:
    bool status;
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;

   public:
    Timer() : status(false) {}

    void start() {
        t1 = std::chrono::high_resolution_clock::now();
        status = true;
    }

    void stop() {
        if (!status) {
            throw std::runtime_error("Timer has not been started!");
        }
        t2 = std::chrono::high_resolution_clock::now();
        status = false;
    }

    auto duration() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    }
};