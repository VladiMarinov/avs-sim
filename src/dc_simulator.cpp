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
  B_matrix = new Eigen::MatrixXf(matrix_size, circuit.num_voltage_sources);
  C_matrix = new Eigen::MatrixXf(circuit.num_voltage_sources, matrix_size);
  D_matrix = new Eigen::MatrixXf(circuit.num_voltage_sources, circuit.num_voltage_sources);
  A_matrix = new Eigen::MatrixXf(circuit.num_voltage_sources + matrix_size, circuit.num_voltage_sources + matrix_size);
  generate_conductance_matrix();
  generate_B_matrix();
  generate_C_matrix();
  generate_D_matrix();
  generate_A_matrix();

}

void DC_Simulator::generate_conductance_matrix()
{
  for(uint32_t col = 0; col < circuit.nodes.size(); col++)
  {
    for(uint32_t row = 0; row <= col; row++ )
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

//  std::cout<< "Conductance matrix:" << std::endl;
 // std::cout << *conductance_matrix << std::endl;
}
void DC_Simulator::generate_B_matrix()
{
  //*B_matrix = Eigen::MatrixXf::Zero(circuit.nodes.size(), circuit.num_voltage_sources);

  for (uint32_t col = 0; col < circuit.num_voltage_sources; col ++)
  {
    for (uint32_t row = 0 ; row < circuit.nodes.size() ; row++)
    {
      if (circuit.is_component_connected_to(circuit.voltage_sources[col], circuit.nodes[row]))
      {
        if (circuit.nodes[row].name == circuit.voltage_sources[col].nodes[0])
        {
          (*B_matrix)(row, col ) = 1;
        }
        else if (circuit.nodes[row].name == circuit.voltage_sources[col].nodes[1])
        {
          (*B_matrix)(row, col ) = -1;
        }
      }
      else
      {
        (*B_matrix)(row, col) = 0;
      }
    }
  }
}

void DC_Simulator::generate_C_matrix()
{
  *C_matrix = B_matrix->transpose();
}
void DC_Simulator::generate_D_matrix()
{
  *D_matrix = Eigen::MatrixXf::Zero(circuit.num_voltage_sources, circuit.num_voltage_sources);
}

void DC_Simulator::generate_A_matrix()
{
  *A_matrix << *conductance_matrix, *B_matrix, *C_matrix, *D_matrix; 
  std::cout << *A_matrix <<std::endl;
}


void DC_Simulator::generate_current_vector()
{
  for(uint32_t i = 0; i < circuit.nodes.size(); i++)
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
