#include "circuit.h"
#include "Eigen/Core"
#include "Eigen/LU"
#include "Eigen/Dense"

class DC_Simulator
{
public:
  Circuit circuit;
  Eigen::MatrixXf *conductance_matrix;
  Eigen::VectorXf *current_vector;
  Eigen::VectorXf voltage_vector;

  DC_Simulator(Circuit input_circuit);

  void generate_conductance_matrix();

  ///generate vector with all currents going into the node
  void generate_current_vector();

  void solve();
};
