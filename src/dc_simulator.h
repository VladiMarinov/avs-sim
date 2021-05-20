#include "circuit.h"
#include "Eigen/Core"
#include "Eigen/LU"
#include "Eigen/Dense"

/// Solves for the DC Steady-state solution of a given circuit (Transients are ignored!).
/// @note For the notational conventions used for naming the vectors and matrices, check <https://lpsa.swarthmore.edu/Systems/Electrical/mna/MNA3.html> 
/// @warning The circuit that is passed to the constructor **must** not contain a ground node.
class DC_Simulator
{
public:
  Circuit circuit;
  
  /// In MNA this is commonly reffered to as the G matrix
  Eigen::MatrixXf *conductance_matrix;
  /// In MNA this is commonly reffered to as the i vector
  Eigen::VectorXf *current_vector;
  /// In MNA this is commonly reffered to as the x vector.
  Eigen::VectorXf *unknown_vector;
  Eigen::MatrixXf *B_matrix;
  Eigen::MatrixXf *C_matrix;
  Eigen::MatrixXf *D_matrix;
  Eigen::MatrixXf *A_matrix;
  Eigen::VectorXf *e_vector;
  Eigen::VectorXf *z_vector;

  /// Create a DC Simulator to simulate the given circuit.
  /// @warning The circuit that is passed to the constructor **must** not contain a ground node.
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

  /// Generates the e vecot for the given circuit.
  void generate_e_vector();

  /// Generates the z vector, by appropriately combining the current and e vector.
  void generate_z_vector();

  /// Solves the circuit for the unknown vector.
  void solve();
};
