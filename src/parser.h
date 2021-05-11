#pragma once

#include <vector>
#include "component.h"

class Parser
{
private:
  std::string input_file;
  std::string current_line;
  std::vector<Component> components;
};
