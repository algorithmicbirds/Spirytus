#include "SpirytusTimer.hpp"

SpirytusTimer::SpirytusTimer() { last = std::chrono::steady_clock::now(); }

float SpirytusTimer::Mark() {
    const auto old = last;
    last = std::chrono::steady_clock::now();
    const std::chrono::duration<float> frameTime = last - old;
    return frameTime.count();
}

float SpirytusTimer::Peek() const {
    return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
}



