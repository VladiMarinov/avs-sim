#include "function_value.h"


#include <utility>


Function_value::Function_value(std::string itype, std::string iamp, std::string iphase):
  amplitude(std::move(iamp)),
  phase(std::move(iphase))
{
  function_type = std::move(itype);
}
