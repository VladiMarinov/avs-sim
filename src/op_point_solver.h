#include "circuit.h"
#include "dc_simulator.h"
#include <cmath>
#include <vector>

class OP_Point_Solver
{
public:

    Circuit circuit;
    Circuit lin_circuit;
    DC_Simulator *dc_sim;
    const int MAX_ITERATIONS = 500;
    const double ABS_VTOL = 1e-6;
    const double IS = 10e-6;
    const double VT = 0.025;
    const double initial_V_guess = 0.7;

    OP_Point_Solver(Circuit input_circuit);

    void create_initial_lin_circuit();
    void update_lin_circuit();
    std::vector<Component> linearize_diode(double VD, Component diode);
    void solve();
};