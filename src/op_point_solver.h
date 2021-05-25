#include "circuit.h"
#include "dc_simulator.h"
#include <cmath>
#include <vector>
#include <memory>

class OP_Point_Solver
{
public:

    Circuit circuit;
    Circuit lin_circuit;
    std::unique_ptr<DC_Simulator> dc_sim;
    const int MAX_ITERATIONS = 1000;
    const double ABS_VTOL = 1e-6;

    std::vector<double> prev_voltages;
    std::vector<double> curr_voltages;

    OP_Point_Solver(Circuit input_circuit);

    void create_initial_lin_circuit();
    void update_lin_circuit();
    std::vector<Component> linearize_diode(double VD, Component diode);
    std::vector<Component> linearize_NPN(double Vbe, double Vbc, Component npn);
    void solve();

    bool hasConverged();
};
