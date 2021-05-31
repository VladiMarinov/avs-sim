#pragma once
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
    const int MAX_ITERATIONS = 500;
    const double ABS_VTOL = 1e-9;

    std::vector<double> prev_voltages;
    std::vector<double> curr_voltages;

    OP_Point_Solver(Circuit input_circuit);

    void create_initial_lin_circuit();
    void update_lin_circuit();
    void solve();

    bool hasConverged();
};
