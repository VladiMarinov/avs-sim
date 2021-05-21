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
      equiv.const_value = new Const_value("0");

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
      total_conductance += 1.0/component.const_value->numeric_value;
    }
    if(component.type == VCCS)
    {
      total_conductance += conductance_between_nodes_from_VCCS(component, node, node);
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
        total_conductance += 1.0/component.const_value->numeric_value;
      }
      if(component.type == VCCS)
      {
        total_conductance += -conductance_between_nodes_from_VCCS(component, node1, node2);
      }
    }
  }

  return total_conductance;
}

double Circuit::conductance_between_nodes_from_VCCS(Component vccs, Node node1, Node node2)
{
  // VCCS node order: 0 N+ | 1 N-| 2 NC+ | 3 NC- |
  if (node1.name == vccs.nodes[0] && node2.name == vccs.nodes[2]) return vccs.const_value->numeric_value;
  if (node1.name == vccs.nodes[1] && node2.name == vccs.nodes[3]) return vccs.const_value->numeric_value;
  if (node1.name == vccs.nodes[0] && node2.name == vccs.nodes[3]) return -vccs.const_value->numeric_value;
  if (node1.name == vccs.nodes[1] && node2.name == vccs.nodes[2]) return -vccs.const_value->numeric_value;

  return 0.0;
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
        total_current -= component.const_value->numeric_value;
      }
      else
      {
        //current going in
        total_current += component.const_value->numeric_value;
      }
    }
  }
  return total_current;
}

