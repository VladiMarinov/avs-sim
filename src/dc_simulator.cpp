#include "dc_simulator.h"
#include <iostream> 

DC_Simulator::DC_Simulator(Circuit input_circuit)
{
  circuit = input_circuit.remove_ground();

  int matrix_size = circuit.nodes.size();
  std::cout << "Matrix size will be " << matrix_size << std::endl;
  conductance_matrix = new Eigen::MatrixXf (matrix_size, matrix_size);
  current_vector = new Eigen::VectorXf (matrix_size);
  voltage_vector = new Eigen::VectorXf (matrix_size);

}

void DC_Simulator::generate_conductance_matrix()
{
  for(int col = 0; col < circuit.nodes.size(); col++)
  {
    for(int row = 0; row <= col; row++ )
    {
      std::cout << "Row/Col: " << row << " " << col <<std::endl;
      std::cout << "Row/Col node: " << circuit.nodes[row].name << " " << circuit.nodes[col].name <<std::endl;

      if(col == row)
      {
        (*conductance_matrix)(row,col) = circuit.total_conductance_into_node(circuit.nodes[row]);
      }
      else
      {
        (*conductance_matrix)(row,col) = -circuit.total_conductance_between_nodes(circuit.nodes[row],circuit.nodes[col]);
        (*conductance_matrix)(col,row) = -circuit.total_conductance_between_nodes(circuit.nodes[row],circuit.nodes[col]);
      }
    }

  }

  std::cout<< "Conductance matrix:" << std::endl;
  std::cout << *conductance_matrix << std::endl;
}

void DC_Simulator::generate_current_vector()
{
  for(int i = 0; i < circuit.nodes.size(); i++)
  {
    (*current_vector)(i) = circuit.total_current_into_node(circuit.nodes[i]);
  }

  std::cout<< "Current vector:" << std::endl;
  std::cout << *current_vector << std::endl;
}

void DC_Simulator::solve()
{
  std::cout<< "Voltage vector:" << std::endl;
  (*voltage_vector) = (*conductance_matrix).lu().solve(*current_vector);
  std::cout << *voltage_vector  << std::endl;
}
