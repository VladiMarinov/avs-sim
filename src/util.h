#pragma once
#include <utility>

#include <vector>
#include <complex>
#include "circuit.h"
#include "ac_directive.h"

namespace util
{
  /// Calculates the voltage between any two given nodes in a circuit.
  /// @param The circuit to find voltage in.
  /// @param The Node ID of the first node.
  /// @param The Node ID of the second node.
  /// @param The voltage vector from which to look up relevant voltages.
  inline double voltage_between_nodes(Circuit circuit, const std::string& n1, const std::string& n2, std::vector<double> voltage_vector)
  {
    circuit = circuit.remove_ground(); // Just in case we are passed a circuit with a ground node.
    double voltage_at_node_1;
    double voltage_at_node_2;

    for(uint32_t i = 0; i < circuit.nodes.size(); i++)
    {
      if (circuit.nodes[i].name == n1)
      {
        voltage_at_node_1 = voltage_vector[i]; 
      }
      if (circuit.nodes[i].name == n2)
      {
        voltage_at_node_2 = voltage_vector[i]; 
      }
    }
    
      return voltage_at_node_1 - voltage_at_node_2;
  }

  inline std::complex<double> voltage_at_node(Circuit circuit, const std::string& n, std::vector<std::complex<double> > voltage_vector)
  {
    circuit = circuit.remove_ground(); // Just in case we are passed a circuit with a ground node.

    for(uint32_t i = 0; i < circuit.nodes.size(); i++)
    {
      if (circuit.nodes[i].name == n)
      {
        return voltage_vector[i];
      }
    }
    std::cout << "node " << n << " doesn't exist" <<std::endl;
    exit(EXIT_FAILURE);
  }

  /// Calculate the voltage across the given component.
  /// @warning Only intended for two-terminal components
  inline double voltage_across_2T_component(Circuit circuit, Component component, std::vector<double> voltage_vector)
  {
    std::string node_ID_1 = component.nodes[0];
    std::string node_ID_2 = component.nodes[1];

    return util::voltage_between_nodes(std::move(circuit), node_ID_1, node_ID_2, std::move(voltage_vector));
  }

  /// Prints a component in a readable way -> FOR DEBUGGING
  inline void printComponent(const Component& c)
  {
    std::cout <<"----------------------------\n";
    std::cout << c.designator << std::endl;
    for(const std::string& n : c.nodes)
    {
      std::cout << n << std::endl;
    }
    if (c.value_type == CONSTANT_VAL) std::cout << c.const_value->numeric_value <<std::endl;
    if (c.value_type == MODEL_VAL) std::cout << c.model_value->model_name <<std::endl;
    if (c.value_type == FUNCTION_VAL){
      std::cout << c.function_value->amplitude.numeric_value <<std::endl;
      std::cout << c.function_value->phase.numeric_value <<std::endl;
    }
    std::cout <<"----------------------------\n";
  }
  
  /// Prints an AC Directive in a readable way -> FOR DEBUGGING
  inline void printACdir(const AC_Directive& dir)
  {
    std::cout <<"----------------------------\n";
    std::cout << dir.sweep_type << std::endl;
    std::cout << dir.points_per_dec << std::endl;
    std::cout << dir.start_freq.numeric_value << std::endl;
    std::cout << dir.stop_freq.numeric_value << std::endl;
    std::cout <<"----------------------------\n";
  }

  /// Prints the designators of all components connected to the node -> FOR DEBUGGING
  inline void printNode(const Node& node){
    std::cout << node.name;
    for(const Component& c : node.components)
    {
      std::cout << " " << c.designator;
    }
    std::cout << std::endl;
  }
}
