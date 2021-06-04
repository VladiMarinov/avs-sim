#include "node.h"
#include <iostream>

Node::Node(std::string iname) 
{
  name = iname;
  
}

void Node::addComponent(Component c)
{
  bool alreadyExists = false;
  for (Component check_c : components)
  {
    if (check_c.designator == c.designator) alreadyExists = true;
  }
  if (!alreadyExists) components.push_back(c); // TODO: Check if emplace_back is better here...
}

