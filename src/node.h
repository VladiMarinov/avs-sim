#pragma once

#include <string>
#include <vector>
#include "component.h"

class Node
{
public:
  int number;
  std::string name;
  std::vector<Component> components;
  
  Node(std::string iname);
  int toNumber(std::string name);

  void addComponent(Component c);
};
