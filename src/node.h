#pragma once

#include <string>
#include <vector>
#include "component.h"

/// Describes a node in the circuit to be simulated. Each node consists of the components connected to it.
class Node
{
public:
  /// The unique node ID, as parsed from the Netlist (e.g. N001, N003, 0).
  /// @note The Node ID 0 is always the ground node, as per SPICE standards.
  std::string name;

  /// All components (represented as Component objects), that are connected to the node.
  std::vector<Component> components;
 
  /// Construct a new node, with the given ID.
  Node(std::string iname);

  /// Add a Component object into the vector of components connected to the node.
  /// @param c The component to add to the vector.
  /// @warning The function checks whether a component with the given designator already exists in the vector, and does not add a second copy if one already exists.
  void addComponent(const Component& c);
};
