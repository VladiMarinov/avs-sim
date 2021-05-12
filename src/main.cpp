#include <iostream>
#include <vector>
#include <string>
#include "parser.h" 

void printComponent(Component c)
{
  std::vector<std::string> componentType = {
    "RESISTOR",       //0   R
    "INDUCTOR",       //1   L
    "CAPACITOR",      //2   C
    "VOLTAGE_SOURCE", //3   V
    "CURRENT_SOURCE", //4   I
    "DIODE",          //5   D
    "BJT",            //6   Q
    "MOSFET",         //7   M
    "VCCS"            //8   G
  };

  std::cout <<"----------------------------\n";
  std::cout << componentType[c.type] << " with enum " << c.type << std::endl;
  std::cout << c.designator << std::endl;
  std::cout << c.node1 << std::endl;
  std::cout << c.node2 << std::endl;
  std::cout << c.node3 << std::endl;
  std::cout << c.node4 << std::endl;
  std::cout << c.value << std::endl;
  std::cout <<"----------------------------\n";
}
void printACdir(AC_Directive dir)
{
  std::cout <<"----------------------------\n";
  std::cout << dir.sweep_type << std::endl;
  std::cout << dir.points_per_dec << std::endl;
  std::cout << dir.start_freq << std::endl;
  std::cout << dir.stop_freq << std::endl;
  std::cout <<"----------------------------\n";
}


int main(int argc, char** argv)
{
    if (argc < 2) 
    {
      std::cout << "No input file given..." << std::endl;
      return EXIT_FAILURE;
    }
    
    Parser* parser = new Parser(argv[1]);
    parser->parse();
    for(Component c : parser->components)
    {
      printComponent(c);
    }
    printACdir(parser->ac_dir);

    return 0;
}
