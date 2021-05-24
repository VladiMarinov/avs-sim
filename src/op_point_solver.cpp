#include "op_point_solver.h"

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

void OP_Point_Solver::solve()
{
    dc_sim = new DC_Simulator(lin_circuit);
    std::cout << "solving..." << std::endl;
    for (double voltage : dc_sim->get_voltage_vector())
    {
        std::cout << voltage << std::endl;
    }
    // for (int i = 0; i < MAX_ITERATIONS; i++)
    // {

    // }
}
