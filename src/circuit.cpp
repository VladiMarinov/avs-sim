#include "circuit.h"
#include <iostream>

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
  for(Node n : nodes)
  {
    if(n.name == node_id)
    {
      std::cout << "Trying to add component to node..." << std::endl;
      n.addComponent(component); 
    }
  }
}