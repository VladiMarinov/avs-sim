#pragma once
#include <string>
#include <vector>
#include "type.h" 
#include "const_value.h" 
#include "function_value.h" 
#include "model_value.h" 
#include "value_type.h" 

/// The Component class describes a single component parsed from the Netlist.
class Component
{
public:
  /// The enum containing the type of the component 
  ComponentType type; 

  /// The designator of the component, begins with the letter corresponding to the component type, followed by numbers that for a unique component ID.
  std::string designator;

  /// The IDs (e.g. N001, 0, N120) of the nodes that the component is connected to. The number of nodes, depenend on how many terminals the component has.
  std::vector<std::string> nodes;

  ValueType value_type;

  /// Constant Value of component. Might be empty if component has function/model value
  Const_value *const_value;
  
  Function_value *function_value;

  Model_value *model_value;

  /// Sets the type of the component, by looking at the first letter of the designator.
  void findType();

  /// Returns a component type, by looking at the first letter of the designator.
  /// @returns ComponentType enum with the type corresponding to the designator.
  ComponentType parse_type();
};
