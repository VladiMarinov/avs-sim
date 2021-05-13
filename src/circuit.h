#pragma once

#include <vector>
#include "component.h"
#include "node.h"

class Circuit
{
public:
  std::vector<Node> nodes;
  Circuit(std::vector<Component> parsed_components);

  bool node_exists(std::string node_id);
  void add_component(std::string node_id, Component component);
};
