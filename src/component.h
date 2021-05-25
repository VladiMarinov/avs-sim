#pragma once
#include <string>
#include <vector>
#include "type.h" 
#include "const_value.h" 
#include "function_value.h" 
#include "model_value.h" 
#include "value_type.h" 

#include<memory>

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

  /// The type of value of the given component - might be one of either CONSTANT_VAL, FUNCTION_VAL, or MODEL_VAL
  ValueType value_type;

  /// Constant Value of component. 
  /// @note Might be null/random if the value type of this component is Function or Model.
  std::shared_ptr<Const_value> const_value;
  
  /// Function value of component.
  /// @note Might be null/random if the value type of this component is Constant or Model.
  std::shared_ptr<Function_value> function_value;

  /// Model Value of component. 
  /// @note Might be null/random if the value type of this component is Constant or Function.
  std::shared_ptr<Model_value> model_value;

  Component();

  /// 2-terminal constructor
  Component(ComponentType type_, std::string designator_, std::string n1_, std::string n2_, double val_);
  
  /// 4-terminal constructor
  Component(ComponentType type_, std::string designator_, std::string n1_, std::string n2_, std::string n3_, std::string n4_, double val_);

  /// Sets the type of the component, by looking at the first letter of the designator.
  void findType();

  /// Returns a component type, by looking at the first letter of the designator.
  /// @returns ComponentType enum with the type corresponding to the designator.
  ComponentType parse_type();
};
