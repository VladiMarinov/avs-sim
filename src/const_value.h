#pragma once
#include <iostream>
#include <string>
#include <cctype>

/// Contains a constant value literal, and its corresponding numeric value. 
/// For example 10k -> 10,000 & 1m -> 0.001
class Const_value
{
public: 
    /// The constant literal as parsed from the netlist.
    std::string str_value; 

    /// The numeric value of constant -> e.g. "10k" is 10000
    double numeric_value;
    
    /// Construct a constant value from the given literal. 
    Const_value(std::string input_str_value);

    /// Returns the numeric value of the given literal as a double.
    double str_to_numeric(std::string input_str_value);

    /// Returns if the current character is **definitely** a single character multiplier.
    /// @note p,n,u,K,G,T are all definitely single character multipliers.
    /// @note m might not be a single character multiplier if followed by "eg" -> forming Meg 
    bool is_single_character_multipler(char c);

    /// Returns the numeric value of the passed multiplier literal. For example 10k -> 10^3.
    double str_to_multiplier(std::string multiplier);
};
