#pragma once
#include <string>
#include <vector>
#include "type.h" 

class Component
{
public:
  ComponentType type; 
  std::string designator;
  std::vector<std::string> nodes;
  std::string value;

  void findType();
  ComponentType parse_type();
};
