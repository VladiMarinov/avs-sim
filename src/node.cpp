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
  components.push_back(c);
  printNode(); // TODO: Check if emplace_back is better here...
}

void Node::printNode()
{
  std::cout << name;
  std::cout<< " size: " << components.size();
  for(Component c : components)
  {
    std::cout << " - " << c.designator;
  }
  std::cout << std::endl;
}


