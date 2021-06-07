#include <iostream>
// #include <vector>
// #include <memory>
// #include <string>
// #include "parser.h"
// #include "dc_simulator.h"
// #include "ac_simulator.h"
// #include "circuit.h"
// #include "op_point_solver.h"
// #include "models.h"
#include "avs-sim.h"
#include "timer.h"

int main(int argc, char** argv)
{
    if (argc < 2) 
    {
      std::cout << "No input file given..." << std::endl;
      return EXIT_FAILURE;
    }
    

    {
      benchmark::Timer timer("SIMULATION FINISHED IN: ");

      AVS_sim avs_sim(argv[1]);
      avs_sim.simulate();
    }

    return 0;
}
