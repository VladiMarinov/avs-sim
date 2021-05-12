#pragma once

#include <vector>
#include "component.h"
#include "node.h"

class Circuit
{
private:
  std::vector<Node> nodes;

public:
  Circuit(std::vector<Component> parsed_components);

};
