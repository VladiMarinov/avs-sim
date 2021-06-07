#include <iostream>
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
      benchmark::Timer timer;

      AVS_sim avs_sim(argv[1]);
      avs_sim.simulate();
    }

    return 0;
}
