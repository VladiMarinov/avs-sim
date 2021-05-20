#pragma once
#include <iostream>
#include <string>
#include <cctype>


class Const_value
{
public: 
    std::string str_value; 
    float numeric_value;
    
    Const_value(std::string input_str_value);

    float str_to_numeric(std::string input_str_value);
    bool is_single_character_multipler(char c);
    float str_to_multiplier(std::string multiplier);
};
