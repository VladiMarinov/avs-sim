#pragma once

#include <string>
#include <vector>
#include "component.h"

class Node
{
private:
  int number;
  std::string name;
  std::vector<Component> components;

public:
  Node(std::string iname);
  int toNumber(std::string name);

  void add(Component c);
};
