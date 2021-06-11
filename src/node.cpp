#include "node.h"
#include <iostream>
#include <utility>


Node::Node(std::string iname) 
{
  name = std::move(iname);
  
}

void Node::addComponent(const Component& c)
{
  bool alreadyExists = false;
  for (const Component& check_c : components)
  {
    if (check_c.designator == c.designator) alreadyExists = true;
  }
  if (!alreadyExists) components.push_back(c); // TODO: Check if emplace_back is better here...
}

