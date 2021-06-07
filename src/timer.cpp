#include "timer.h"

benchmark::Timer::Timer(){
    finished_message = ""; // Default name is empty string
    start_time_point = std::chrono::high_resolution_clock::now();
}

benchmark::Timer::Timer( std::string i_finished_message ){
    finished_message = i_finished_message;
    start_time_point = std::chrono::high_resolution_clock::now();
}

benchmark::Timer::~Timer(){
    auto end_time_point = std::chrono::high_resolution_clock::now();

    auto start_us = std::chrono::time_point_cast < std::chrono::microseconds > (start_time_point).time_since_epoch().count();
    auto end_us = std::chrono::time_point_cast < std::chrono::microseconds > (end_time_point).time_since_epoch().count();

    auto duration_us = end_us - start_us;
    double duration_ms = duration_us * 0.001f;

    if( finished_message != "" ){
        std::cout << "---- " << finished_message << ": " << duration_us << "us (" << duration_ms << "ms)" << std::endl;
    }
    else{
        std::cout << "---- TIMER: " << duration_us << "us (" << duration_ms << "ms)" << std::endl;
    }
}
