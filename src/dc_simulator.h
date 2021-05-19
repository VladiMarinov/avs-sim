#include "circuit.h"
#include "Eigen/Core"
#include "Eigen/LU"
#include "Eigen/Dense"

/// Solves for the DC Steady-state solution of a given circuit (Transients are ignored!).
/// For the notational conventions used for naming the vectors and matrices, check README.md
class DC_Simulator
{
public:
  Circuit circuit;
  Eigen::MatrixXf *conductance_matrix;
  Eigen::VectorXf *current_vector;
  Eigen::VectorXf *voltage_vector;
  Eigen::MatrixXf *B_matrix;
  Eigen::MatrixXf *C_matrix;
  Eigen::MatrixXf *D_matrix;
  Eigen::MatrixXf *A_matrix;

  /// Create a DC Simulator to simulate the given circuit.
  DC_Simulator(Circuit input_circuit);

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

  /// Solves the circuit for the voltage vector, and assigns the solution to voltage_vector.
  void solve();
};
