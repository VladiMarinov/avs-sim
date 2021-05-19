#pragma once

#include <vector>
#include "component.h"
#include "ac_directive.h"

/// The Parser class parses a SPICE-based Nelist file into a vector of components and directives containing the simulation details.
class Parser
{
private:
  std::string input_file;
  std::string current_line;
  uint32_t curr_pos;
  bool directiveFound = false;
  bool endFound = false;
public:
  std::vector<Component> components; // TODO: make private after testing
  AC_Directive ac_dir;

  /// Construct a Parser
  /// @param input_file The path to the file to be parsed.
  Parser(std::string input_file);

  /// Moves curr_pos to the next non-whitespace character in the line. 
  void skip_whitespace();

  /// Parse the input file
  void parse();

  /// Parse the current line of the input file
  void parse_line();

  /// Parse a line containing a component 
  /// @returns Parsed component inside a Component object
  Component parse_component();

  /// Parse a line containing a two-terminal component
  /// @returns Parsed component inside a Component object
  Component parse_two_terminal();

  /// Parse a line containing a three-terminal component
  /// @returns Parsed component inside a Component object
  Component parse_three_terminal();

  /// Parse a line containing a four-terminal component
  /// @returns Parsed component inside a Component object
  Component parse_four_terminal();

  /// Parse the next token (i.e. "word") in the line
  /// @returns The next token in the line
  std::string parse_next_token();

  /// Parses the value of a component, by assigning everything from the current position till the end of the line into a string.
  /// returns The value of the component in a given line.
  std::string parse_value();

  /// Parses a directive (any line beginning with a '.') . If the directive is an '.ac' directive it is assigned to the ac_dir member object.
  void parse_directive();
};
