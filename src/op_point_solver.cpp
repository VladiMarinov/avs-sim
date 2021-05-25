#include "op_point_solver.h"
#include "util.h"
#include "models.h"
#include <cmath>

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
            std::vector<Component> equiv_components = linearize_diode(models::D().initial_V_guess, component);
            lin_components.push_back(equiv_components[0]);
            lin_components.push_back(equiv_components[1]);
        }
        if (component.type == BJT)
        {
            std::vector<Component> equiv_components = linearize_NPN(models::NPN().initial_Vbe_guess, models::NPN().initial_Vbc_guess, component);
            for (Component c : equiv_components)
            {
              lin_components.push_back(c);
            }
        }
        else
        {
            lin_components.push_back(component);
        }
    }

    lin_circuit = Circuit(lin_components);
    // for (Component c : lin_circuit.circuit_components)
    // {
      // util::printComponent(c);
    // }
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
        if (component.type == BJT)
        {
            double Vbe = util::voltage_between_nodes(circuit, component.nodes[1], component.nodes[2], curr_voltages);
            double Vbc = util::voltage_between_nodes(circuit, component.nodes[1], component.nodes[0], curr_voltages);
            std::vector<Component> equiv_components = linearize_NPN(Vbe, Vbc, component);
            for (Component c : equiv_components)
            {
              lin_components.push_back(c);
            }
        }
        else
        {
            lin_components.push_back(component);
        }
    }

    lin_circuit = Circuit(lin_components);
    // for (Component c : lin_circuit.circuit_components)
    // {
      // util::printComponent(c);
    // }
}

std::vector<Component> OP_Point_Solver::linearize_diode(double VD, Component diode)
{
    std::vector<Component> equiv;

    models::D D_;
    double IS = D_.IS;
    double VT = D_.VT;

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

std::vector<Component> OP_Point_Solver::linearize_NPN(double Vbe, double Vbc, Component npn)
{
  std::vector<Component> equiv;

  models::NPN model_npn;

  std::string collector = npn.nodes[0];
  std::string base = npn.nodes[1];
  std::string emmiter = npn.nodes[2];

  double IES = model_npn.IES;
  double ICS = model_npn.ICS;
  double VT = model_npn.VT;
  double AF = model_npn.AF;
  double AR = model_npn.AR;

  double gee = (IES/VT)* std::exp(Vbe/VT); 
  double gce = AF * gee;
  double gcc = (ICS/VT) * std::exp(Vbc/VT); 
  double gec = AR * gcc; 

  double ie = (-IES * (std::exp(Vbe/VT) - 1)) + (AR*ICS * (std::exp(Vbc/VT) - 1));
  double ic = (-ICS * (std::exp(Vbc/VT) - 1)) + (AF*IES * (std::exp(Vbe/VT) - 1));

  double IE = ie + gee*Vbe - gec*Vbc;
  double IC = ic + gcc*Vbc - gce*Vbe;

  // WARNING: the order of the following push_backs is important - DO NOT REORDER

  // VCCS base-collector
  equiv.push_back( Component(VCCS, "VCCS_bc_" + npn.designator, collector, base, base, emmiter, gce ) ); 

  // Ree
  equiv.push_back( Component(RESISTOR, "Ree" + npn.designator, base, emmiter, 1.0/gee ) ); 
  
  // VCCS base-emmiter
  equiv.push_back( Component(VCCS, "VCCS_be_" + npn.designator, emmiter, base, base, collector, gec ) );

  // Rcc
  equiv.push_back( Component(RESISTOR, "Rcc" + npn.designator, base, collector, 1.0/gcc ) ); 
  
  // Current source base-collector
  equiv.push_back( Component(CURRENT_SOURCE, "IC__" + npn.designator, collector, base, IC ) ); 

  // Current source base-emmiter
  equiv.push_back( Component(CURRENT_SOURCE, "IE__" + npn.designator, emmiter, base, IE ) ); 

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
