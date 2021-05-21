#pragma once
#include "const_value.h"
#include <string>

class Function_value
{
public:
  std::string function_type;
  Const_value *amplitude;
  Const_value *phase;

  Function_value(std::string itype, std::string iamp, std::string iphase);
};
