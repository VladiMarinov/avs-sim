#define _USE_MATH_DEFINES

#include "ac_simulator.h"
#include <iostream>
#include "util.h"
#include "linearizer.h"

AC_Simulator::AC_Simulator()
{

}

AC_Simulator::AC_Simulator(Circuit input_circuit, std::vector<double> input_op_voltages, double input_freq)
{
  large_signal_circuit = input_circuit.remove_ground();
  sim_freq = input_freq;
  op_voltages = input_op_voltages;
  generate_small_signal_circuit();
  small_signal_circuit = small_signal_circuit.remove_ground();

  int total_voltage_sources = small_signal_circuit.num_AC_voltage_sources + small_signal_circuit.num_DC_voltage_sources;

  uint32_t matrix_size = small_signal_circuit.nodes.size();
  //std::cout << "Matrix size will be " << matrix_size << std::endl;
  conductance_matrix =  std::unique_ptr<Eigen::MatrixXcd>(new Eigen::MatrixXcd(matrix_size, matrix_size));
  current_vector = std::unique_ptr<Eigen::VectorXcd>(new Eigen::VectorXcd(matrix_size));
  unknown_vector = std::unique_ptr<Eigen::VectorXcd>(new Eigen::VectorXcd(total_voltage_sources + matrix_size));
  B_matrix = std::unique_ptr<Eigen::MatrixXcd>(new Eigen::MatrixXcd(matrix_size, total_voltage_sources));
  C_matrix = std::unique_ptr<Eigen::MatrixXcd>(new Eigen::MatrixXcd(total_voltage_sources, matrix_size));
  D_matrix = std::unique_ptr<Eigen::MatrixXcd>(new Eigen::MatrixXcd(total_voltage_sources, total_voltage_sources));
  A_matrix = std::unique_ptr<Eigen::MatrixXcd>(new Eigen::MatrixXcd(total_voltage_sources + matrix_size,  total_voltage_sources+ matrix_size));
  e_vector = std::unique_ptr<Eigen::VectorXcd>(new Eigen::VectorXcd(total_voltage_sources));
  z_vector = std::unique_ptr<Eigen::VectorXcd>(new Eigen::VectorXcd(matrix_size + total_voltage_sources));
  generate_conductance_matrix();
  generate_B_matrix();
  generate_C_matrix();
  generate_D_matrix();
  generate_A_matrix();
  generate_current_vector();
  generate_e_vector();
  generate_z_vector();
  solve();
}

void AC_Simulator::generate_small_signal_circuit()
{
  std::vector<Component> small_signal_components;
  for(Component component : large_signal_circuit.circuit_components)
  {
      if(component.type == DIODE)
      {
          double diode_voltage = util::voltage_across_2T_component(large_signal_circuit, component, op_voltages);
          std::vector<Component> equiv_components = Linearizer::small_signal_diode(diode_voltage, component);
          small_signal_components.push_back(equiv_components[0]);
      }
      else if (component.type == BJT)
      {
          double Vbe = util::voltage_between_nodes(large_signal_circuit, component.nodes[1], component.nodes[2], op_voltages);
          double Vbc = util::voltage_between_nodes(large_signal_circuit, component.nodes[1], component.nodes[0], op_voltages);
          std::vector<Component> equiv_components;
          
          if (component.model_value->model_name == "NPN")
          {
            equiv_components = Linearizer::small_signal_NPN(Vbe, Vbc, component);
          }
          else if (component.model_value->model_name == "PNP")
          {
            equiv_components = Linearizer::small_signal_PNP(-Vbe, -Vbc, component);
          }

          for (Component c : equiv_components)
          {
            small_signal_components.push_back(c);
          }
      }
      else if (component.type == MOSFET)
      {
        double Vgs = util::voltage_between_nodes(large_signal_circuit, component.nodes[1], component.nodes[2], op_voltages);
        double Vds = util::voltage_between_nodes(large_signal_circuit, component.nodes[0], component.nodes[2], op_voltages);
        std::vector<Component> equiv_components;
        if (component.model_value->model_name == "NMOS")
        {
          equiv_components = Linearizer::small_signal_NMOS(Vgs, Vds, component);
        }
        else if (component.model_value->model_name == "PMOS")
        {
          equiv_components = Linearizer::small_signal_PMOS(Vgs, Vds, component);
        }
        
        for (Component c : equiv_components)
        {
          small_signal_components.push_back(c);
        }
      }
      else
      {
          small_signal_components.push_back(component);
      }
  }
  small_signal_circuit = Circuit(small_signal_components);
  // for (Component c : small_signal_circuit.circuit_components)
  // {
  //   //util::printComponent(c);
  // }
}

void AC_Simulator::generate_conductance_matrix()
{
  for(uint32_t col = 0; col < small_signal_circuit.nodes.size(); col++)
  {
    for(uint32_t row = 0; row < small_signal_circuit.nodes.size(); row++ )
    {
      // TODO: DEBUG PRINT MIGHT BE UNNECESSARY
      // std::cout << "Row/Col: " << row << " " << col <<std::endl;
      // std::cout << "Row/Col node: " << circuit.nodes[row].name << " " << circuit.nodes[col].name <<std::endl;

      if(col == row)
      {
        (*conductance_matrix)(row,col) = small_signal_circuit.AC_total_conductance_into_node(small_signal_circuit.nodes[row], sim_freq);
      }
      else
      {
        (*conductance_matrix)(row,col) = -small_signal_circuit.AC_total_conductance_between_nodes(small_signal_circuit.nodes[row],small_signal_circuit.nodes[col], sim_freq);
      }
    }
  }

//  std::cout<< "Conductance matrix:" << std::endl;
 // std::cout << *conductance_matrix << std::endl;
}
void AC_Simulator::generate_B_matrix()
{
  for (uint32_t col = 0; col < small_signal_circuit.num_AC_voltage_sources; col ++)
  {
    for (uint32_t row = 0 ; row < small_signal_circuit.nodes.size() ; row++)
    {
      if (small_signal_circuit.is_component_connected_to(small_signal_circuit.AC_voltage_sources[col], small_signal_circuit.nodes[row]))
      {
        if (small_signal_circuit.nodes[row].name == small_signal_circuit.AC_voltage_sources[col].nodes[0])
        {
          (*B_matrix)(row, col ) = 1;
        }
        else if (small_signal_circuit.nodes[row].name == small_signal_circuit.AC_voltage_sources[col].nodes[1])
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
  for (uint32_t col = 0; col < small_signal_circuit.num_DC_voltage_sources; col ++)
  {
    for (uint32_t row = 0 ; row < small_signal_circuit.nodes.size() ; row++)
    {
      if (small_signal_circuit.is_component_connected_to(small_signal_circuit.DC_voltage_sources[col], small_signal_circuit.nodes[row]))
      {
        if (small_signal_circuit.nodes[row].name == small_signal_circuit.DC_voltage_sources[col].nodes[0])
        {
          (*B_matrix)(row, col+ small_signal_circuit.num_AC_voltage_sources) = 1;
        }
        else if (small_signal_circuit.nodes[row].name == small_signal_circuit.DC_voltage_sources[col].nodes[1])
        {
          (*B_matrix)(row, col + small_signal_circuit.num_AC_voltage_sources ) = -1;
        }
      }
      else
      {
        (*B_matrix)(row, col + small_signal_circuit.num_AC_voltage_sources) = 0;
      }
    }
  }
}

void AC_Simulator::generate_C_matrix()
{
  *C_matrix = B_matrix->transpose();
}

void AC_Simulator::generate_D_matrix()
{
  int total_voltage_sources = small_signal_circuit.num_AC_voltage_sources + small_signal_circuit.num_DC_voltage_sources;
  *D_matrix = Eigen::MatrixXcd::Zero(total_voltage_sources, total_voltage_sources);
}

void AC_Simulator::generate_A_matrix()
{
  *A_matrix << *conductance_matrix, *B_matrix, *C_matrix, *D_matrix; 
  //std::cout << *A_matrix <<std::endl;
}

void AC_Simulator::generate_current_vector()
{
  for(uint32_t i = 0; i < small_signal_circuit.nodes.size(); i++)
  {
    (*current_vector)(i) = small_signal_circuit.AC_total_current_into_node(small_signal_circuit.nodes[i]);
  }
}

void AC_Simulator::generate_e_vector()
{
  for (uint32_t i = 0; i < small_signal_circuit.num_AC_voltage_sources ; i++)
  {
    double A = small_signal_circuit.AC_voltage_sources[i].function_value->amplitude.numeric_value;
    double phase =  (M_PI/180.0) * small_signal_circuit.AC_voltage_sources[i].function_value->phase.numeric_value;
      
    std::complex<double> phasor = A * std::exp(std::complex<double>(0,phase));

    (*e_vector)(i) = phasor;
  }
  for (uint32_t i = small_signal_circuit.num_AC_voltage_sources; i < small_signal_circuit.num_AC_voltage_sources + small_signal_circuit.num_DC_voltage_sources; i++)
  {
    // DC sources in an AC sim will always have a value of 0 -> short circuits.
    (*e_vector)(i) = 0;
  }
}

void AC_Simulator::generate_z_vector()
{
  *z_vector << *current_vector, *e_vector; 
  // std::cout << *z_vector << std::endl;
}

void AC_Simulator::solve()
{
  // std::cout << std::endl;
  // std::cout<< "unknown vector:" << std::endl;
  (*unknown_vector) = (*A_matrix).lu().solve(*z_vector);
  //std::cout << *unknown_vector  << std::endl;
}

std::vector<std::complex<double>> AC_Simulator::get_voltage_vector()
{
  std::vector<std::complex<double>> voltage_vector;
  for (uint32_t i = 0; i < small_signal_circuit.nodes.size(); i++)
  {
    voltage_vector.push_back((*unknown_vector)(i));
  }
  return voltage_vector;
}
