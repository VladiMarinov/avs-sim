#pragma once

#include<iostream>
#include<chrono>
#include<string>

namespace benchmark{

class Timer{
private:
    std::string finished_message;
    std::chrono::time_point < std::chrono::high_resolution_clock > start_time_point;
public:
    Timer();
    Timer(std::string i_finished_message);
    ~Timer();
};

}
