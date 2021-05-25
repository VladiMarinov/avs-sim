#pragma once
#include <vector>
#include "circuit.h"
#include "ac_directive.h"

namespace util
{
  double voltage_between_nodes(Circuit circuit, std::string n1, std::string n2, std::vector<double> voltage_vector)
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

  /// Calculate the voltage across the given component.
  /// @warning Only intended for two-terminal components
  double voltage_across_2T_component(Circuit circuit, Component component, std::vector<double> voltage_vector)
  {
    std::string node_ID_1 = component.nodes[0];
    std::string node_ID_2 = component.nodes[1];

    return util::voltage_between_nodes(circuit, node_ID_1, node_ID_2, voltage_vector);
  }

  void printComponent(Component c)
  {
    std::cout <<"----------------------------\n";
    std::cout << c.designator << std::endl;
    for(std::string n : c.nodes)
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
  void printACdir(AC_Directive dir)
  {
    std::cout <<"----------------------------\n";
    std::cout << dir.sweep_type << std::endl;
    std::cout << dir.points_per_dec << std::endl;
    std::cout << dir.start_freq << std::endl;
    std::cout << dir.stop_freq << std::endl;
    std::cout <<"----------------------------\n";
  }

  void printNode(Node node){
    std::cout << node.name;
    for(Component c : node.components)
    {
      std::cout << " " << c.designator;
    }
    std::cout << std::endl;
  }
}
