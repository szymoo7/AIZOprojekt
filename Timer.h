//
// Created by szyme on 15.04.2025.
//

#ifndef AIZOPROJEKT_TIMER_H
#define AIZOPROJEKT_TIMER_H


#include <chrono>

class Timer {
public:
    Timer() {
        startTime = stopTime = 0;
    };
    void reset() {
        startTime = stopTime = 0;
    }

    int start() {
        startTime = (int) std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch())
                .count();
        return 0;
    }

    int stop() {
        stopTime = (int) std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch())
                .count();
        return 0;
    }

    int result() const {
        return stopTime - startTime;
    }

private:
    int startTime;
    int stopTime;
};


#endif //AIZOPROJEKT_TIMER_H
