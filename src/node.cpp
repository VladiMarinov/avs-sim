#include "node.h"
#include <iostream>

Node::Node(std::string iname) 
{
  name = iname;
  number = toNumber(name);
}

int Node::toNumber(std::string name)
{
  if(name == "0")
  {
    return 0;
  }
  else
  {
    name.erase(name.begin());
    return std::stoi(name);
  }
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

