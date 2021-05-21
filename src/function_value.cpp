#include "function_value.h"

Function_value::Function_value(std::string itype, std::string iamp, std::string iphase)
{
  function_type = itype;
  amplitude = new Const_value(iamp);
  phase = new Const_value(iphase);
}
