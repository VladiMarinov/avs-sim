#pragma once

#include "circuit.h"
#include "Eigen/Core"
#include "Eigen/LU"
#include "Eigen/Dense"
#include <vector>
#include <memory>

class AC_Simulator
{
public:
  Circuit large_signal_circuit;
  Circuit small_signal_circuit;
  double sim_freq;
  std::vector<double> op_voltages;

  std::unique_ptr<Eigen::MatrixXcd> conductance_matrix;
  std::unique_ptr<Eigen::VectorXcd> current_vector;
  std::unique_ptr<Eigen::VectorXcd> unknown_vector;
  std::unique_ptr<Eigen::MatrixXcd> B_matrix;
  std::unique_ptr<Eigen::MatrixXcd> C_matrix;
  std::unique_ptr<Eigen::MatrixXcd> D_matrix;
  std::unique_ptr<Eigen::MatrixXcd> A_matrix;
  std::unique_ptr<Eigen::VectorXcd> e_vector;
  std::unique_ptr<Eigen::VectorXcd> z_vector;

  /// Default DC Sim constructor.
  AC_Simulator();

  /// Create a DC Simulator to simulate the given circuit.
  /// @warning The circuit that is passed to the constructor **must** not contain a ground node.
  AC_Simulator(Circuit input_circuit, std::vector<double> input_op_voltages, double input_freq);

  /// Generated small signal circuit from the original large_signal_circuit
  void generate_small_signal_circuit();

  /// Generates the conductance matrix for the given circuit.
  void generate_conductance_matrix();

  /// Generates the B matrix for the given circuit. 
  void generate_B_matrix();

  /// Generates the C matrix for the given circuit.
  void generate_C_matrix();

  /// Generates the D matrix for the given circuit.
  void generate_D_matrix();

  /// Generates the A matrix, by appropriately combining the G,B,C, and D matrices.
  void generate_A_matrix();

  /// Generates the current vector for the given circuit.
  void generate_current_vector();

  /// Generates the e vecot for the given circuit.
  void generate_e_vector();

  /// Generates the z vector, by appropriately combining the current and e vector.
  void generate_z_vector();

  /// Solves the circuit for the unknown vector.
  void solve();

  std::vector<std::complex<double> > get_voltage_vector();
};
