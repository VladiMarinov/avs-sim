#include "parser.h"
#include <iostream>
#include <cctype>
#include <fstream>

Parser::Parser(std::string i)
{
  input_file = i;
}

void Parser::skip_whitespace()
{
  while (current_line[curr_pos] == ' ' || current_line[curr_pos] == '\t'
      || current_line[curr_pos] == '(' || current_line[curr_pos] == ')') 
  {
    curr_pos++;
  }
}

void Parser::parse()
{
  std::ifstream input_file_stream;
  input_file_stream.open(input_file);

  if (!input_file_stream.is_open())
  {
    std::cout << "Failed to open specified file..." << std::endl;
    exit(EXIT_FAILURE);
  }

  while (!endFound && input_file_stream)
  {
    std::getline(input_file_stream, current_line);
    curr_pos = 0;
    parse_line();
  }

  input_file_stream.close();

  if (!OP_directiveFound && !AC_directiveFound)
  {
    std::cout << "ERROR: No directives found...." << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Parser::parse_line()
{
  if (!current_line.empty())
  {
    skip_whitespace();

    if (current_line[curr_pos] != '*' && current_line[curr_pos] != '.')
    {
      components.push_back(parse_component()); // TODO: check if emplace_back is better here?
    }
    if (current_line[curr_pos] == '.')
    {
      parse_directive();
    }
  }
}

Component Parser::parse_component()
{
  skip_whitespace();

  if (current_line[curr_pos] == 'V' ||
      current_line[curr_pos] == 'I' ||
      current_line[curr_pos] == 'R' ||
      current_line[curr_pos] == 'C' ||
      current_line[curr_pos] == 'L' ||
      current_line[curr_pos] == 'D')
    return parse_two_terminal();

  if (current_line[curr_pos] == 'Q' ||
      current_line[curr_pos] == 'M')
    return parse_three_terminal();

  if (current_line[curr_pos] == 'G')
    return parse_four_terminal();

  std::cout << "Component Designator Letter not recognized..." << std::endl;
  exit(EXIT_FAILURE);
}

Component Parser::parse_two_terminal()
{
  Component component;
  component.designator = parse_next_token();
  component.findType();
  component.nodes.push_back(parse_next_token());
  component.nodes.push_back(parse_next_token());
  parse_value(component);
  return component;
}

Component Parser::parse_three_terminal()
{
  Component component;
  component.designator = parse_next_token();
  component.findType();
  component.nodes.push_back(parse_next_token());
  component.nodes.push_back(parse_next_token());
  component.nodes.push_back(parse_next_token());
  parse_value(component);
  return component;
}

Component Parser::parse_four_terminal()
{
  Component component;
  component.designator = parse_next_token();
  component.findType();
  component.nodes.push_back(parse_next_token());
  component.nodes.push_back(parse_next_token());
  component.nodes.push_back(parse_next_token());
  component.nodes.push_back(parse_next_token());
  parse_value(component);
  return component;
}

std::string Parser::parse_next_token()
{
  std::string token = "";
  while (current_line[curr_pos] != ' ' && current_line[curr_pos] != '\t'
      && current_line[curr_pos] != '(' && current_line[curr_pos] != ')' 
      && curr_pos < current_line.length())
  {
    token += current_line[curr_pos];
    curr_pos++;
  }

  skip_whitespace();
  return token;
}

void Parser::parse_value(Component &c)
{
  skip_whitespace();
  if (!std::isdigit(current_line[curr_pos]) && (current_line[curr_pos] != '-'))
  {
    if (c.type == RESISTOR || c.type == INDUCTOR || c.type == CAPACITOR || c.type == VCCS)
    {
      std::cout << "ERROR at line: \n";
      std::cout << current_line << std::endl;
      std::cout << "Component of this type must have a constant numeric value - function/model is not possible...\n";
      exit(EXIT_FAILURE);
    }
    if (c.type == VOLTAGE_SOURCE || c.type == CURRENT_SOURCE)
    {
      std::string type = parse_next_token();
      std::string amplitude = parse_next_token();
      std::string phase = parse_next_token();
      c.function_value = std::make_shared<Function_value>(type, amplitude, phase);
      c.value_type = FUNCTION_VAL;
    }
    if (c.type == DIODE || c.type == MOSFET || c.type == BJT)
    {
      c.model_value = std::make_shared<Model_value>(parse_next_token());
      c.value_type = MODEL_VAL;
    }
  } 
  else
  {
    if (c.type == DIODE || c.type == MOSFET || c.type == BJT)
    {
      std::cout << "ERROR at line: \n";
      std::cout << current_line << std::endl;
      std::cout << "Component of this type must have a model value - function/constant is not possible...\n";
      exit(EXIT_FAILURE);
    }
    if ( (current_line[curr_pos] == '-') && (c.type == RESISTOR || c.type == INDUCTOR || c.type == CAPACITOR) ) // to avoid having negative values for constant value components that can't have them
    {
      std::cout << "ERROR at line: \n";
      std::cout << current_line << std::endl;
      std::cout << "Commponents of type : " << c.type << " cannot have a negative value \n";
      exit(EXIT_FAILURE);

    }
    std::string nt = parse_next_token();
    c.const_value = std::make_shared<Const_value>(nt);
    c.value_type = CONSTANT_VAL;

  }
}

void Parser::parse_directive()
{
  std::string next_token = parse_next_token();
  if (next_token == ".end")
  {
    endFound = true;
  }
  if (next_token == ".op")
  {
    OP_directiveFound = true;
  }
  if (next_token == ".ac")
  {
    if (!AC_directiveFound)
    {
      ac_dir.sweep_type = parse_next_token();
      ac_dir.points_per_dec = std::stoi(parse_next_token());
      ac_dir.start_freq = Const_value(parse_next_token());
      ac_dir.stop_freq = Const_value(parse_next_token());
      AC_directiveFound = true;
    }
    else
    {
      std::cout << "ERROR: Multiple AC directives found..." << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

bool Parser::has_found_AC_directive()
{
  return AC_directiveFound;
}
