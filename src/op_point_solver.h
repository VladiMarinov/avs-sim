#pragma once
#include "circuit.h"
#include "dc_simulator.h"
#include <cmath>
#include <vector>
#include <memory>

/// Implements iterative Newton-Raphson method to find the quiescent operating point of a given circuit.
class OP_Point_Solver
{
public:

    
    Circuit circuit;
    Circuit lin_circuit;
    std::unique_ptr<DC_Simulator> dc_sim;

    /// The maximum number of iterations for Newton-Raphson. If the algorithm reaches that number of iterations, it stops and returns a non-convergence error.
    const int MAX_ITERATIONS = 500;

    /// The absolute voltage tolerance, when checking for node voltage convergence.
    const double ABS_VTOL = 1e-9;

    std::vector<double> prev_voltages;
    std::vector<double> curr_voltages;

    /// Construct an OP_Point_Solver for the given circuit.
    OP_Point_Solver(Circuit input_circuit);

    /// Creates the intial linearized circuit using the initial voltage guesses specified in each Model struct
    // @see models.h
    void create_initial_lin_circuit();

    /// Updates the linearized circuit using the current voltage guess from the latest Newton-Raphson iteration.
    void update_lin_circuit();

    /// Runs the Newton-Raphson algorithm.
    void solve();

    /// Checks whether the circuit has converged, by comparing the previous and current iteration node voltages.
    /// @note In order for convergence to be reached *ALL* differences in node voltages between the current and previous iteration must be less than ABS_VTOL.
    bool hasConverged();
};
