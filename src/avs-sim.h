#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <cmath>

#include "ac_directive.h"
#include "parser.h"
#include "op_point_solver.h"
#include "ac_simulator.h"
#include "circuit.h"




/// The main class of the program - runs the neccessary simulations, and writes to the output file.
class AVS_sim
{
public: 
    
    /// Run the simulator with the given netlist
    AVS_sim(std::string inputfile_name);

    /// Starts the neccessary simulations
    void simulate(); 

    /// Does the AC Sweep iterations
    void itterate_over_ac();

private: 

    std::unique_ptr<Parser> parser;
    std::unique_ptr<Circuit> circuit;
    std::vector<double> DC_voltages;
    bool has_AC_directive;
    AC_Directive ac_dir; 
    
};
