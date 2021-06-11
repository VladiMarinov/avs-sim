#include "model_value.h"


#include <utility>


Model_value::Model_value(std::string imodel)
{
  model_name = std::move(imodel);
}
