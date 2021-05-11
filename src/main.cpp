#include <iostream>
#include "parser.h" 

void printComponent(Component c)
{
  std::cout <<"----------------------------\n";
  std::cout << c.designator << std::endl;
  std::cout << c.node1 << std::endl;
  std::cout << c.node2 << std::endl;
  std::cout << c.node3 << std::endl;
  std::cout << c.node4 << std::endl;
  std::cout << c.value << std::endl;
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

    return 0;
}
