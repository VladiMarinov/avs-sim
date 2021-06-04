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




class AVS_sim
{
public: 
    AVS_sim(std::string inputfile_name);

    void simulate(); 
    void itterate_over_ac();

private: 

    std::unique_ptr<Parser> parser;
    std::unique_ptr<Circuit> circuit;
    std::vector<double> DC_voltages;
    AC_Directive ac_dir; 



};