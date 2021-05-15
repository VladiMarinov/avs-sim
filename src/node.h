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

  /// The node ID, but as a number. (e.g. N001 would have a node number equal to 1)
  int number;

  /// All components (represented as Component objects), that are connected to the node.
  std::vector<Component> components;
 
  /// Construct a new node, with the given ID.
  Node(std::string iname);

  /// Convert the string-based node ID into the node number, by removing the leading N, and converting to int.
  /// @returns The node ID number, as an int.
  int toNumber(std::string name);

  /// Add a Component object into the vector of components connected to the node.
  /// @param c The component to add to the vector.
  void addComponent(Component c);
};
