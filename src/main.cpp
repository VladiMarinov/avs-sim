#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "parser.h" 
#include "dc_simulator.h" 
#include "circuit.h"
#include "op_point_solver.h"
#include "models.h"

int main(int argc, char** argv)
{
    if (argc < 2) 
    {
      std::cout << "No input file given..." << std::endl;
      return EXIT_FAILURE;
    }
    
    std::unique_ptr<Parser> parser (new Parser (argv[1]));
    parser->parse();
    // for(Component c : parser->components)
    // {
      // printComponent(c);
    // }
    // printACdir(parser->ac_dir);
//
    // std::cout <<"\n----------------------------\n";

    Circuit circuit(parser->components);
    // for(Node node : circuit.nodes)
    // {
      // printNode(node);
    // }

    Circuit DC_Circuit = circuit.get_DC_Equivalent_Circuit().remove_ground();
    // DC_Simulator sim(DC_Circuit);

    OP_Point_Solver op_point_solver(DC_Circuit);
    op_point_solver.create_initial_lin_circuit();
    op_point_solver.solve();
    // for (Component c : op_point_solver.lin_circuit.circuit_components)
    // {
      // printComponent(c);
    // }

    return 0;
}
