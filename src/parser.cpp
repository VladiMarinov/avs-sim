#include "parser.h"
#include <iostream>
#include <fstream>

Parser::Parser(std::string i)
{
  input_file = i; 
}

void Parser::skip_whitespace()
{
  while(current_line[curr_pos] == ' ' || current_line[curr_pos] == '\t')
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

  while(input_file_stream)
  {
    std::getline(input_file_stream, current_line);
    curr_pos = 0;
    std::cout << current_line << std::endl;
    parse_line();
  }

  input_file_stream.close();
}

void Parser::parse_line()
{
  skip_whitespace();

  if(current_line[curr_pos] != '*' && current_line[curr_pos] != '.'  && !current_line.empty())
  {
    components.push_back(parse_component()); // TODO: check if emplace_back is better here? 
  }
}

Component Parser::parse_component()
{
  skip_whitespace();

  if(current_line[curr_pos] == 'V' ||
     current_line[curr_pos] == 'I' ||
     current_line[curr_pos] == 'R' ||
     current_line[curr_pos] == 'C' ||
     current_line[curr_pos] == 'L' ||
     current_line[curr_pos] == 'D' )  return parse_two_terminal();

  if(current_line[curr_pos] == 'Q' ||
     current_line[curr_pos] == 'M' )  return parse_three_terminal();

  if(current_line[curr_pos] == 'G' )  return parse_four_terminal();

  std::cout << "Component Designator Letter not recognized..." << std::endl;
  exit(EXIT_FAILURE);
}

Component Parser::parse_two_terminal()
{
  Component component;
  component.designator = parse_next_token(); 
  component.node1 = parse_next_token(); 
  component.node2 = parse_next_token(); 
  component.value = parse_value(); 
  return component;
}

Component Parser::parse_three_terminal()
{
  Component component;
  component.designator = parse_next_token(); 
  component.node1 = parse_next_token(); 
  component.node2 = parse_next_token(); 
  component.node3 = parse_next_token(); 
  component.value = parse_value(); 
  return component;
}

Component Parser::parse_four_terminal()
{
  Component component;
  component.designator = parse_next_token(); 
  component.node1 = parse_next_token(); 
  component.node2 = parse_next_token(); 
  component.node3 = parse_next_token(); 
  component.node4 = parse_next_token(); 
  component.value = parse_value(); 
  return component;
}

std::string Parser::parse_next_token()
{
  std::string token = "";
  while(current_line[curr_pos] != ' ' && current_line[curr_pos] != '\t' )
  {
    token += current_line[curr_pos];
    curr_pos++;
  }
  skip_whitespace();
  return token;
}

std::string Parser::parse_value()
{
  std::string val = "";
  int len = current_line.length();
  while(curr_pos < len)
  {
    val += current_line[curr_pos];
    curr_pos++;
  }
  return val;
}
