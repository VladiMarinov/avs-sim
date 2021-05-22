#pragma once
#include <string>

/// Contains a model value of a component - e.g. NPN, D ...
class Model_value
{
public:
  /// The model name
  std::string model_name;

  /// Create a model value with the given model name.
  Model_value(std::string imodel);
};
