#pragma once

#include<iostream>
#include<chrono>
#include<string>

/// Useful classes for benchmarking
namespace benchmark{

/// Lifetime-based timer for benchmarking.
/// Will print its lifetime when destructor called.
/// Scopes can be used for easy lifetime setting.
class Timer{
private:
    std::string finished_message;
    std::chrono::time_point < std::chrono::high_resolution_clock > start_time_point;
public:
    /// Create a timer with default finished message (TIMER:)
    Timer();

    /// Create a timer with the given finished message
    Timer(std::string i_finished_message);

    /// When the destructor is called the timer will print its lifetime...
    ~Timer();
};

}
