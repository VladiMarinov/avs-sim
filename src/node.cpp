#include "node.h"

Node::Node(std::string iname) 
{
  name = iname;
  number = toNumber(name);
}

int Node::toNumber(std::string name)
{
  name.erase(name.begin());
  return std::stoi(name);
}

void Node::add(Component c)
{
  components.push_back(c); // TODO: Check if emplace_back is better here...
}



