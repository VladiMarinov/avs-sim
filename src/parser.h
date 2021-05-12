#pragma once

#include <vector>
#include "component.h"
#include "ac_directive.h"

class Parser
{
private:
  std::string input_file;
  std::string current_line;
  int curr_pos;
  bool directiveFound = false;
  bool endFound = false;
public:
  std::vector<Component> components; // TODO: make private after testing
  AC_Directive ac_dir;

  Parser(std::string input_file);
  void skip_whitespace();
  void parse();
  void parse_line();
  Component parse_component();
  Component parse_two_terminal();
  Component parse_three_terminal();
  Component parse_four_terminal();
  ComponentType parse_componentType(Component c);
  
  std::string parse_next_token();
  std::string parse_value();
  void parse_directive();
};
