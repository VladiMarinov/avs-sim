#pragma once

#include <vector>
#include <complex>
#include "component.h"
#include "node.h"

/// Represents a circuit, consisting of a vector of nodes.
class Circuit
{
public:


  std::vector<Component> circuit_components;

  /// The nodes that consist the circuit. Each node contains all the components connected to it.
  std::vector<Node> nodes;
  
  uint32_t num_DC_voltage_sources;
  uint32_t num_AC_voltage_sources;
  
  std::vector<Component> DC_voltage_sources;
  std::vector<Component> AC_voltage_sources;

  Circuit();

  /// Create a new circuit, given the vector of parsed components.
  /// @param parsed_components A vector of all components that make up the circuit. Those are expected to be read from the input file by the parser.
  Circuit(const std::vector<Component>& parsed_components);

  /// Checks if a node with a given ID has already been created and added to the vector of nodes.
  /// @param node_id The node ID to check
  /// @returns Does the node with the given ID exist in the vector.
  bool node_exists(const std::string& node_id);

  /// Add a component to a node in the vector of nodes.
  /// @param node_id The ID of the node to which the component should be added.
  /// @param component The component to add.
  void add_component(const std::string& node_id, const Component& component);

  /// Returns a copy of this circuit, with the ground node removed from the nodes vector.
  /// @note Both the AC_Simulator and DC_Simulator expect no ground node in the circuit they are passed.
  Circuit remove_ground();

  /// Returns the Steady-state DC equivalent of the circuit.
  /// Capacitors are discarded - they are open circuits in Steady-state DC.
  /// Inductors are modeled as 0V voltage sources - this is the easiest way to model a short circuit.
  /// Resistors, Voltage sources, and Current sources are left as is.
  Circuit get_DC_Equivalent_Circuit();

  /// Returns the Steady-state DC equivalent components for this circuit.
  /// Those are then used to construct a DC equivalent circuit.
  /// @see get_DC_Equivalent_Circuit()
  std::vector<Component> get_DC_Equivalent_Components();


  /// Returns the circuit, with all DC sources set to 0 -> Voltage sources are transformed into 0V short circuits, while Current sources are discarded as they are open circuit.
  Circuit get_AC_Equivalent_Circuit();

  /// Returns a copy of the component vector, but with all sources set to 0.
  /// This is used to construct the AC_equivalent_circuit.
  /// @see get_AC_Equivalent_Circuit())
  std::vector<Component> get_AC_Equivalent_Components();

  /// Calculates the total DC conductance directly connected to a given node. This is used when calculating the main diagonal entries of the conductance matrix.
  double DC_total_conductance_into_node(const Node& node);
  
  /// Calculates the total DC conductance that directly connects two given nodes. This is used when calculating the non main-diagonal entries of the conductance matrix.
  double DC_total_conductance_between_nodes(const Node& node1, const Node& node2);

  /// Calculates the total DC current coming into the given node from **current sources only**. 
  double DC_total_current_into_node(const Node& node);

  /// Calculates the total AC conductance directly connected to a given node. This is used when calculating the main diagonal entries of the conductance matrix.
  std::complex<double> AC_total_conductance_into_node(const Node& node, double freq);
  
  /// Calculates the total AC conductance that directly connects two given nodes. This is used when calculating the non main-diagonal entries of the conductance matrix.
  std::complex<double> AC_total_conductance_between_nodes(const Node& node1, const Node& node2, double freq);

  /// Calculates the total AC current coming into the given node from **current sources only**. 
  std::complex<double> AC_total_current_into_node(const Node& node);

  /// Returns any conductance due to Voltage-controlled Current sources between the two given nodes.
  double conductance_between_nodes_from_VCCS(Component vccs, const Node& node1, const Node& node2);

  /// Returns whether the given component has any terminal connected to the given node.
  bool is_component_connected_to(const Component& component, const Node& node);

  };
