#include "circuit.h"
#include "type.h"
#include <iostream>

Circuit::Circuit()
{
}

Circuit::Circuit(std::vector<Component> parsed_components)
{
  for(Component c : parsed_components)
  {
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

float Circuit::total_conductance_into_node(Node node)
{
  float total_conductance = 0;
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

float Circuit::total_conductance_between_nodes(Node node1, Node node2)
{
  float total_conductance = 0;
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

