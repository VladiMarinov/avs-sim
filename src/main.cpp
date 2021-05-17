#include <iostream>
#include <vector>
#include <string>
#include "parser.h" 
#include "circuit.h"
 
void printComponent(Component c)
{
  std::cout <<"----------------------------\n";
  std::cout << c.designator << std::endl;
  for(std::string n : c.nodes)
  {
    std::cout << n << std::endl;
  }
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

void printNode(Node node){
  std::cout << node.name;
  for(Component c : node.components)
  {
    std::cout << " " << c.designator;
  }
  std::cout << std::endl;
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
      printComponent(c); /
    }
    printACdir(parser->ac_dir);

    std::cout <<"\n----------------------------\n";

    Circuit circuit(parser->components);
    for(Node node : circuit.nodes)
    {
      printNode(node);
    }

    return 0;
}
