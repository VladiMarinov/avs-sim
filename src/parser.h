#pragma once

#include <vector>
#include "component.h"

class Parser
{
private:
  std::string input_file;
  std::string current_line;
  int curr_pos;
  std::vector<Component> components;
public:
  Parser(std::string input_file);
  void skip_whitespace();
  void parse();
  void parse_line();
  Component parse_component();
  Component parse_two_terminal();
  Component parse_three_terminal ();
  Component parse_four_terminal ();
};
