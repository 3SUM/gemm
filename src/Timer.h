#pragma once

#include <chrono>
#include <stdexcept>

class Timer {
   private:
    bool status;
    double _duration;
    std::chrono::system_clock::time_point t1;

   public:
    Timer() : status(false), _duration(0) {}

    void start() {
        t1 = std::chrono::system_clock::now();
        status = true;
    }

    void stop() {
        if (!status) {
            throw std::runtime_error("Timer has not been started!");
        }
        auto t2 = std::chrono::system_clock::now();
        status = false;
        _duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    }

    double duration() const {
        return _duration;
    }
};