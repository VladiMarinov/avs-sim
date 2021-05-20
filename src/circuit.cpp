#include "circuit.h"
#include "type.h"
#include <iostream>

// TODO: figure out why this is neccessary and how to get rid of it...
Circuit::Circuit()
{
}

Circuit::Circuit(std::vector<Component> input_components)
{
  circuit_components = input_components;

  num_voltage_sources =0;
  for(Component c : input_components)
  {
    if (c.type == VOLTAGE_SOURCE)
    {
      num_voltage_sources++; 
      voltage_sources.push_back(c);
    }
    for(std::string node_id : c.nodes)
    {
      if(!node_exists(node_id))
      {
        nodes.push_back(Node(node_id));
      }
      add_component(node_id, c);
    }
  }
}

bool Circuit::node_exists(std::string node_id)
{
  for(Node n : nodes)
  {
    if(n.name == node_id)
    {
      return true;
    }
  }
  return false;
}

void Circuit::add_component(std::string node_id, Component component)
{
  for(Node& n : nodes)
  {
    if(n.name == node_id)
    {
      n.addComponent(component); 
    }
  }
}

Circuit Circuit::remove_ground()
{
  Circuit no_ground_circuit = *this;
  for(uint32_t n = 0; n < no_ground_circuit.nodes.size(); n++)
  {
    if(no_ground_circuit.nodes[n].name == "0")
    {
      no_ground_circuit.nodes.erase(no_ground_circuit.nodes.begin() + n);
    }
  }

  return no_ground_circuit;
}

Circuit Circuit::get_DC_Equivalent_Circuit()
{
  return Circuit(get_DC_Equivalent_Components());
}

std::vector<Component> Circuit::get_DC_Equivalent_Components()
{
  std::vector<Component> dc_equivalent_components;
  for (Component c: circuit_components)
  {
    if (c.type == INDUCTOR)
    {
      Component equiv;
      equiv.type  = VOLTAGE_SOURCE;
      equiv.designator = c.designator;
      equiv.nodes = c.nodes;
      equiv.value = "0";

      dc_equivalent_components.push_back(equiv);
    }
    else if (c.type != CAPACITOR  )
    {
      dc_equivalent_components.push_back(c);
    }
    // CAPACITOR IS IGNORED
  }
  return dc_equivalent_components;
}



double Circuit::total_conductance_into_node(Node node)
{
  double total_conductance = 0.0;
  for(Component component: node.components)
  {
    if(component.type == RESISTOR)
    {
      // TODO: make getComponnentValue function that reads value modifiers as well
      total_conductance += 1/stof(component.value);
    }
  }

  return total_conductance;
}

double Circuit::total_conductance_between_nodes(Node node1, Node node2)
{
  double total_conductance = 0.0;
  for(Component component: node1.components)
  {
    if (is_component_connected_to(component, node2))
    {
      if(component.type == RESISTOR)
      {
        // TODO: make getComponnentValue function that reads value modifiers as well
        total_conductance += 1/stof(component.value);
      }
    }
  }

  return total_conductance;
}

bool Circuit::is_component_connected_to(Component component, Node node)
{
  std::string target_node_ID = node.name;
  for(std::string current_node_ID : component.nodes)
  {
    if(current_node_ID == target_node_ID) return true; 
  }
  return false;
}

double Circuit::total_current_into_node(Node node)
{
  double total_current = 0.0;
  for(Component component : node.components)
  {
    if(component.type == CURRENT_SOURCE)
    {
      if(component.nodes[0] == node.name)
      {
        //current going out
        total_current -= std::stof(component.value);
      }
      else
      {
        //current going in
        total_current += std::stof(component.value);
      }
    }
  }
  return total_current;
}

