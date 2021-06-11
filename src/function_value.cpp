#include "function_value.h"


#include <utility>


Function_value::Function_value(std::string itype, const std::string& iamp, const std::string& iphase):
  amplitude(iamp),
  phase(iphase)
{
  function_type = std::move(itype);
}
