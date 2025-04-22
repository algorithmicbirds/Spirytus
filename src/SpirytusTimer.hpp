#pragma once 

#include <chrono>

class SpirytusTimer {
  public:
    SpirytusTimer();
    float Mark();
    float Peek() const;

  private:
    std::chrono::steady_clock::time_point last;
};