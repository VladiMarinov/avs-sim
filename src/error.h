#pragma once

#include <string>
#include <iostream>

namespace AVS_ERROR
{
  
  enum ParserError
  {
    COMPONENT_REDEFINITION,
    FAILED_FILE_OPEN,
    NO_DIRECTIVES,
    UNKNOWN_COMPONENT_DESIGNATOR,
    EXPECTED_CONST_VAL,
    EXPECTED_MODEL_VAL,
    UNEXPECTED_NEGATIVE_VAL,
    MULTIPLE_AC_DIRECTIVE
  };

  inline void print_parser_info(const std::string& filename, const std::string& line, uint32_t line_num, uint32_t col)
  {
    std::cout << "In netlist " << filename << " on line " << line_num << ":\n";
    std::cout << "  " << line_num << "| " << line <<"\n";
   
    uint32_t needed_white_space = col + std::to_string(line_num).size();
    std::cout << "   " << "|";
    for(uint32_t i = 0; i < needed_white_space; i++ ) std::cout <<" ";
    std::cout << "^\n";
  }

  inline void print_error_message(ParserError error)
  {
    std::string err_msg;
    switch (error)
    {
      case COMPONENT_REDEFINITION:        err_msg = "A component with the same designator already exists... Component designators must be unique!"; break; 
      case FAILED_FILE_OPEN:              err_msg = "Could not open specified netlist, check if the file exists..."; break; 
      case NO_DIRECTIVES:                 err_msg = "Found no simulation directives... Nothing to simulate..."; break; 
      case UNKNOWN_COMPONENT_DESIGNATOR:  err_msg = "Unrecognized component designator letter."; break; 
      case EXPECTED_CONST_VAL:            err_msg = "Components of this type can only have a constant value" ; break; 
      case EXPECTED_MODEL_VAL:            err_msg = "The value of a component of this type must be a model. Please specify the model name of this component."; break; 
      case UNEXPECTED_NEGATIVE_VAL:       err_msg = "Components of this type cannot have a negative value"; break; 
      case MULTIPLE_AC_DIRECTIVE:         err_msg = "Multiple .ac directives found - a netlist can contain a maximum of one."; break; 
      default:                            err_msg = "Unrecognized error... contact developers." ;
    }
    std::cout << err_msg;
  }

  inline void parser_error(ParserError error,const std::string& filename, const std::string& line,  uint32_t line_num, uint32_t col)
  {
    std::cout << "PARSER ERROR: ";
    if (error != FAILED_FILE_OPEN && error != NO_DIRECTIVES)
    {
      print_parser_info(filename, line, line_num, col);
    }

    print_error_message(error);
    exit(EXIT_FAILURE);
  }
} 
