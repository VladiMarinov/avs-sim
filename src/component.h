#pragma once
#include <string>
#include "type.h" 

class Component
{
public:
  ComponentType type; 
  std::string designator;
  std::string node1;
  std::string node2;
  std::string node3;
  std::string node4;
  std::string value;
};
