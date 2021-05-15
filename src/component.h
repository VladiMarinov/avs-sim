#pragma once
#include <string>
#include <vector>
#include "type.h" 

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

  /// The value of the component. Can be numeric,a model number, or a function for AC sources.
  std::string value;

  /// Sets the type of the component, by looking at the first letter of the designator.
  void findType();

  /// Returns a component type, by looking at the first letter of the designator.
  /// @returns ComponentType enum with the type corresponding to the designator.
  ComponentType parse_type();
};
