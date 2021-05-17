#include "circuit.h"
#include "Eigen/Core"
#include "Eigen/LU"
#include "Eigen/Dense"

/// Solves for the DC Steady-state solution of a given circuit (Transients are ignored!)
class DC_Simulator
{
public:
  Circuit circuit;
  Eigen::MatrixXf *conductance_matrix;
  Eigen::VectorXf *current_vector;
  Eigen::VectorXf* voltage_vector;

  /// Create a DC Simulator to simulate the given circuit.
  DC_Simulator(Circuit input_circuit);

  /// Generates the conductance matrix for the given circuit.
  void generate_conductance_matrix();

  /// Generates the current vector for the given circuit.
  void generate_current_vector();

  /// Solves the circuit for the voltage vector, and assigns the solution to voltage_vector.
  void solve();
};
