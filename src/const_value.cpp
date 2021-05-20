
#include "const_value.h"


Const_value::Const_value(std::string input_str_value)
{
    str_value = input_str_value;
    numeric_value = str_to_numeric(input_str_value);
}

float Const_value::str_to_numeric(std::string str_value)
{
    std::string str_significand; 
    bool has_found_decimal_point = false;
    std::string multiplier; 
    for(uint32_t i =0 ; i < str_value.length(); i++)
    {
        char c = tolower(str_value[i]);
        if (std::isdigit(c))
        {
            str_significand += c;
        }
        else if (c == '.')
        {
            if (has_found_decimal_point)
            {
                std::cout << "two decimal points found "<< std::endl;
                exit(EXIT_FAILURE);
            }
            else {
                has_found_decimal_point = true;
                str_significand += c;
            }
        } 
        else {
            if (is_single_character_multipler(c))
            {
                multiplier = c; 
                break; 
            }   
            else if (c == 'm')
            {
                if (tolower(str_value[i+1]) == 'e' && tolower(str_value[i+2]) == 'g')
                {
                    multiplier = "Meg"; 
                    break;
                }
                else 
                {
                    multiplier = "m";
                    break;
                }
            }
        }
    }  
    
}

bool Const_value::is_single_character_multipler(char c)
{
    c = tolower(c);
    if (c == 'p' || c == 'n' || c == 'u' || c == 'k' || c == 'g' || c == 't' ) return true; 
    return false;
}

float Const_value::str_to_multiplier(std::string multiplier)
{
    switch(multiplier)
    {
        case "p" : return 0.000000000001;
        case "n" : return 0.000000001;
        case "u" : return 0.000001;
        case "m" : return 0.001;
        case "k" : return 1000;
        case "Meg" : return 1000000;
        case "G" : return 1000000000;
        case "t" : return 1000000000000;
        case "k" : return 1000;
    }
}