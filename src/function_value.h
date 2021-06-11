#pragma once
#include "const_value.h"
#include <string>

/// Contains a function value of a component - e.g. (AC 10m 10).
class Function_value
{
public:
  /// The type of function value.
  /// @warning Current only the AC function is supported.
  std::string function_type;

  /// The constant value of the amplitude.
  Const_value amplitude;

  /// The constant value of the phase.
  Const_value phase;

  /// Create a new function value with the given type, amplitude, and phase.
  Function_value(std::string itype, const std::string& iamp, const std::string& iphase);
};
