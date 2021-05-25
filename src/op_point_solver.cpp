#include "op_point_solver.h"
#include "util.h"

#include <iomanip>

OP_Point_Solver::OP_Point_Solver(Circuit input_circuit)
{
    circuit = input_circuit;
}

void OP_Point_Solver::create_initial_lin_circuit()
{
    std::vector<Component> lin_components;
    for(Component component : circuit.circuit_components)
    {
        if(component.type == DIODE)
        {
            std::vector<Component> equiv_components = linearize_diode(initial_V_guess, component);
            lin_components.push_back(equiv_components[0]);
            lin_components.push_back(equiv_components[1]);
        }
        else
        {
            lin_components.push_back(component);
        }
    }

    lin_circuit = Circuit(lin_components);
}


void OP_Point_Solver::update_lin_circuit()
{
    std::vector<Component> lin_components;
    for(Component component : circuit.circuit_components)
    {
        if(component.type == DIODE)
        {
            double diode_voltage = util::voltage_across_2T_component(circuit, component, curr_voltages);
            std::vector<Component> equiv_components = linearize_diode(diode_voltage, component);
            lin_components.push_back(equiv_components[0]);
            lin_components.push_back(equiv_components[1]);
        }
        else
        {
            lin_components.push_back(component);
        }
    }

    lin_circuit = Circuit(lin_components);
}

std::vector<Component> OP_Point_Solver::linearize_diode(double VD, Component diode)
{
    std::vector<Component> equiv;
    double ID = IS * (std::exp(VD/VT) - 1);
    double GD = IS * std::exp(VD/VT) / VT;
    double IEQ = ID - (VD * GD);

    Component R_equiv;
    R_equiv.type = RESISTOR;
    R_equiv.designator = "R" + diode.designator;
    R_equiv.nodes = diode.nodes;
    R_equiv.value_type = CONSTANT_VAL;
    R_equiv.const_value = std::make_shared<Const_value>(1/GD);
    equiv.push_back(R_equiv);

    Component I_equiv;
    I_equiv.type = CURRENT_SOURCE;
    I_equiv.designator = "I" + diode.designator;
    I_equiv.nodes = diode.nodes;
    I_equiv.value_type = CONSTANT_VAL;
    I_equiv.const_value = std::make_shared<Const_value>(IEQ);
    equiv.push_back(I_equiv);

    return equiv;
}

bool OP_Point_Solver::hasConverged()
{
  for(uint32_t i = 0; i < curr_voltages.size(); i++)
  {
    if (std::abs(curr_voltages[i] - prev_voltages[i]) > ABS_VTOL) return false;
  }
  return true;
}

void OP_Point_Solver::solve()
{ 
    // Initial solve 
    dc_sim = std::make_unique<DC_Simulator>(lin_circuit);
    curr_voltages = dc_sim->get_voltage_vector();
    prev_voltages = std::vector<double>(curr_voltages.size(), 0); // Initialize prev_voltages to vector full of zeroes.
   
    int i;
    for (i = 0; i < MAX_ITERATIONS && !hasConverged(); i++)
    {
      // std::cout << "NEWTON RAPHSON ITER: " << i << std::endl;
      update_lin_circuit();
      dc_sim = std::make_unique<DC_Simulator>(lin_circuit);
      prev_voltages = curr_voltages;
      curr_voltages = dc_sim->get_voltage_vector();
    }
  
    if (hasConverged())
    {
      std::cout << "OP POINT SOLVER CONVERGED IN " << i << " ITERATIONS WITH THE FOLLOWING VOLTAGES: " << std::endl;
      for (double voltage : curr_voltages)
      {
        std::cout << std::setprecision(17) <<  voltage << std::endl;
      }
    }
    else
    {
      std::cout << " OP POINT SOLVER DID NOT CONVERGE, CHANGE CIRCUIT OR SETTING AND TRY AGAIN..." << std::endl;
      exit(EXIT_FAILURE);
    }
    
}
