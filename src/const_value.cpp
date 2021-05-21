#include "const_value.h"
#include <iostream>

Const_value::Const_value(std::string input_str_value)
{
    str_value = input_str_value;
    if (str_value != "D")
    {
      numeric_value = str_to_numeric(input_str_value);
    }
}

double Const_value::str_to_numeric(std::string str_value)
{
    std::string str_significand; 
    bool has_found_decimal_point = false;
    std::string multiplier; 
    for(uint32_t i = 0 ; i < str_value.length(); i++)
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
    if (str_significand.empty())
    {
      std::cout << "Unexpected value found ->  " << str_value << std::endl;
      exit(EXIT_FAILURE);
    }
    return std::stod(str_significand) * str_to_multiplier(multiplier); 
}

bool Const_value::is_single_character_multipler(char c)
{
    c = tolower(c);
    if (c == 'p' || c == 'n' || c == 'u' || c == 'k' || c == 'g' || c == 't' ) return true; 
    return false;
}

double Const_value::str_to_multiplier(std::string multiplier)
{
  if (multiplier == "p")   return 0.000000000001;
  if (multiplier == "n")   return 0.000000001;
  if (multiplier == "u")   return 0.000001;
  if (multiplier == "m")   return 0.001;
  if (multiplier == "k")   return 1000.0; 
  if (multiplier == "Meg") return 1000000.0;
  if (multiplier == "g")   return 1000000000.0;
  if (multiplier == "t")   return 1000000000000.0;
  return 1.0;
}

