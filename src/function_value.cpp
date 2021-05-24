#include "function_value.h"

Function_value::Function_value(std::string itype, std::string iamp, std::string iphase):
  amplitude(iamp),
  phase(iphase)
{
  function_type = itype;
}
