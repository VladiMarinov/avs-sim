#pragma once
#include <vector>
#include "circuit.h"

namespace util
{
  /// Calculate the voltage across the given component.
  /// @warning Only intended for two-terminal components
  double voltage_across_2T_component(Circuit circuit, Component component, std::vector<double> voltage_vector)
  {
    circuit = circuit.remove_ground(); // Just in case we are passed a circuit with a ground node.
    std::string node_ID_1 = component.nodes[0];
    std::string node_ID_2 = component.nodes[1];

    double voltage_at_node_1;
    double voltage_at_node_2;

    for(uint32_t i = 0; i < circuit.nodes.size(); i++)
    {
      if (circuit.nodes[i].name == node_ID_1)
      {
        voltage_at_node_1 = voltage_vector[i]; 
      }
      if (circuit.nodes[i].name == node_ID_2)
      {
        voltage_at_node_2 = voltage_vector[i]; 
      }
    }
    
    return voltage_at_node_1 - voltage_at_node_2;
  }
}