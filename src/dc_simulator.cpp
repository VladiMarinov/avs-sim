#include "dc_simulator.h"
#include <iostream> 

DC_Simulator::DC_Simulator(Circuit input_circuit)
{
  circuit = input_circuit;
}

void DC_Simulator::generate_conductance_matrix()
{
  int matrix_size = circuit.nodes.size() - 1;
  std::cout << "Matrix size will be " << matrix_size << std::endl;
}
