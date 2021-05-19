#pragma once

#include <vector>
#include "component.h"
#include "node.h"

/// Represents a circuit, consisting of a vector of nodes.
class Circuit
{
public:


  std::vector<Component> circuit_components;

  /// The nodes that consist the circuit. Each node contains all the components connected to it.
  std::vector<Node> nodes;
  uint32_t num_voltage_sources;
  std::vector<Component> voltage_sources;

  Circuit();

  /// Create a new circuit, given the vector of parsed components.
  /// @param parsed_components A vector of all components that make up the circuit. Those are expected to be read from the input file by the parser.
  Circuit(std::vector<Component> parsed_components);

  /// Checks if a node with a given ID has already been created and added to the vector of nodes.
  /// @param node_id The node ID to check
  /// @returns Does the node with the given ID exist in the vector.
  bool node_exists(std::string node_id);

  /// Add a component to a node in the vector of nodes.
  /// @param node_id The ID of the node to which the component should be added.
  /// @param component The component to add.
  void add_component(std::string node_id, Component component);

  /// Returns a copy of the circuit, with the ground node removed from the nodes vector.
  Circuit remove_ground();

  Circuit get_DC_Equivalent_Circuit();

  std::vector<Component> get_DC_Equivalent_Components();

  /// Calculates the total conductance directly connected to a given node. 
  float total_conductance_into_node(Node node);
  
  /// Calculates the total conductance that directly connects two given nodes. This is used when calculating the main diagonal entries of the conductance matrix.
  float total_conductance_between_nodes(Node node1, Node node2);

  /// Returns whether the given component has any terminal connected to the given node.
  bool is_component_connected_to(Component component, Node node);

  /// Calculates the total current coming into the given node from **current sources only**. This is used when calculating the non main-diagonal entries of the conductance matrix.
  float total_current_into_node(Node node);
};
