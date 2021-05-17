#include "circuit.h"
#include "Eigen/Core"
#include "Eigen/LU"

class DC_Simulator
{
public:
  Circuit circuit;
  Eigen::MatrixXf conductance_matrix;

  DC_Simulator(Circuit input_circuit);
  void generate_conductance_matrix();
};
