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
          std::vector<Component> equiv_components;
          if (component.model_value->model_name == "NPN")
          {
            equiv_components = linearize_NPN(models::NPN().initial_Vbe_guess, models::NPN().initial_Vbc_guess, component);
          }
          else if (component.model_value->model_name == "PNP")
          {
            equiv_components = linearize_PNP(models::PNP().initial_Veb_guess, models::PNP().initial_Vcb_guess, component);
          }
          
          for (Component c : equiv_components)
          {
            lin_components.push_back(c);
          }
        }
        if (component.type == MOSFET)
        {
          std::vector<Component> equiv_components;
          if (component.model_value->model_name == "NMOS")
          {
            equiv_components = linearize_NMOS(models::NMOS().initial_Vgs_guess, models::NMOS().initial_Vds_guess, component);
          }
          else if (component.model_value->model_name == "PMOS")
          {
            //equiv_components = linearize_PMOS(models::PMOS().initial_Veb_guess, models::PMOS().initial_Vcb_guess, component);
          }
          
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
    for (Component c : lin_circuit.circuit_components)
    {
      util::printComponent(c);
    }
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
            std::vector<Component> equiv_components;
            
            if (component.model_value->model_name == "NPN")
            {
              equiv_components = linearize_NPN(Vbe, Vbc, component);
            }
            else if (component.model_value->model_name == "PNP")
            {
              equiv_components = linearize_PNP(-Vbe, -Vbc, component);
            }

            for (Component c : equiv_components)
            {
              lin_components.push_back(c);
            }
        }
        if (component.type == MOSFET)
        {
          double Vgs = util::voltage_between_nodes(circuit, component.nodes[1], component.nodes[2], curr_voltages);
          double Vds = util::voltage_between_nodes(circuit, component.nodes[0], component.nodes[2], curr_voltages);
          std::vector<Component> equiv_components;
          if (component.model_value->model_name == "NMOS")
          {
            equiv_components = linearize_NMOS(Vgs, Vds, component);
          }
          else if (component.model_value->model_name == "PMOS")
          {
            //equiv_components = linearize_PMOS(models::PMOS().initial_Veb_guess, models::PMOS().initial_Vcb_guess, component);
          }
          
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
    for (Component c : lin_circuit.circuit_components)
    {
      util::printComponent(c);
    }
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

    equiv.push_back( Component(RESISTOR, "R" + diode.designator, diode.nodes[0], diode.nodes[1], 1/GD));
    equiv.push_back( Component(CURRENT_SOURCE, "I" + diode.designator, diode.nodes[0], diode.nodes[1], IEQ));

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

  double gee = (IES/VT) * std::exp(Vbe/VT); 
  double gce = AF * gee;
  double gcc = (ICS/VT) * std::exp(Vbc/VT); 
  double gec = AR * gcc; 

  double ie = (-IES * (std::exp(Vbe/VT) - 1)) + (AR*ICS * (std::exp(Vbc/VT) - 1));
  double ic = (-ICS * (std::exp(Vbc/VT) - 1)) + (AF*IES * (std::exp(Vbe/VT) - 1));

  double IE = ie + gee*Vbe - gec*Vbc;
  double IC = ic + gcc*Vbc - gce*Vbe;

  // WARNING: the order of the following push_backs is important - DO NOT REORDER
  // This is due to the fact that the node order in the components must come up first as CBE.
  // otherwise u risk messing up the order with respect to the parsed circuit.

  // TODO: My intuition screams that adding the base-collector VCCS first should always fix any possible issues, but CHECK THIS AND MAKE SURE.
  // This is a massive bug opportunity...

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

std::vector<Component> OP_Point_Solver::linearize_PNP(double Veb, double Vcb, Component pnp)
{
  std::vector<Component> equiv;

  models::NPN model_pnp;

  std::string collector = pnp.nodes[0];
  std::string base = pnp.nodes[1];
  std::string emmiter = pnp.nodes[2];

  double IES = model_pnp.IES;
  double ICS = model_pnp.ICS;
  double VT = model_pnp.VT;
  double AF = model_pnp.AF;
  double AR = model_pnp.AR;

  double gee = (IES/VT) * std::exp(Veb/VT); 
  double gce = AF * gee;
  double gcc = (ICS/VT) * std::exp(Vcb/VT); 
  double gec = AR * gcc; 

  double ie = (IES * (std::exp(Veb/VT) - 1)) - (AR*ICS * (std::exp(Vcb/VT) - 1));
  double ic = (ICS * (std::exp(Vcb/VT) - 1)) - (AF*IES * (std::exp(Veb/VT) - 1));

  double IE = -ie + gee*Veb - gec*Vcb;
  double IC = -ic + gcc*Vcb - gce*Veb;

  // WARNING: the order of the following push_backs is important - DO NOT REORDER
  // This is due to the fact that the node order in the components must come up first as CBE.
  // otherwise u risk messing up the order with respect to the parsed circuit.

  // TODO: My intuition screams that adding the base-collector VCCS first should always fix any possible issues, but CHECK THIS AND MAKE SURE.
  // This is a massive bug opportunity...

  // Rcc
  equiv.push_back( Component(RESISTOR, "Rcc" + pnp.designator, collector, base, 1.0/gcc ) ); 

  // VCCS base-emmiter
  equiv.push_back( Component(VCCS, "VCCS_eb_" + pnp.designator, base, emmiter, collector, base, gec ) );

  // VCCS base-collector
  equiv.push_back( Component(VCCS, "VCCS_cb_" + pnp.designator, base, collector, emmiter, base, gce ) ); 

  // Ree
  equiv.push_back( Component(RESISTOR, "Ree" + pnp.designator, base, emmiter, 1.0/gee ) ); 
  
  // Current source base-collector
  equiv.push_back( Component(CURRENT_SOURCE, "IC__" + pnp.designator, base, collector, IC ) ); 

  // Current source base-emmiter
  equiv.push_back( Component(CURRENT_SOURCE, "IE__" + pnp.designator, base, emmiter, IE ) ); 

  return equiv;
}

std::vector<Component> OP_Point_Solver::linearize_NMOS(double Vgs, double Vds, Component nmos)
{
  std::vector<Component> equiv;

  models::NMOS model_nmos;

  std::string drain = nmos.nodes[0];
  std::string gate = nmos.nodes[1];
  std::string source = nmos.nodes[2];

  double KP = model_nmos.KP;
  double lambda = model_nmos.lambda;
  double Vt = model_nmos.Vt;

  double id = 0;
  double Gds = 0;
  double gm = 0;
  double IEQ = 0;

  std::cout << "DEBUG: Vgs: " << Vgs << " , Vds: " << Vds<< std::endl;

  //CUT-OFF REGION
  if (Vgs < Vt)
  {
    std::cout << "ENTERING CUT-OFF REGION..." << std::endl;
    id = 0;
    Gds = 0;
    gm = 0;
  }
  //LINEAR REGION
  if ((Vds >= 0) && (Vds <= Vgs - Vt))
  {
    std::cout << "ENTERING LINEAR REGION..." << std::endl;
    id = KP * ( 2 * (Vgs - Vt) * Vds - Vds * Vds);
    Gds = 2 * KP * (Vgs - Vt - Vds);
    gm = 2 * KP * Vds; 
  }
  //SATURATION REGION
  if ((Vgs - Vt >= 0) && (Vds >= Vgs - Vt))
  {
    std::cout << "ENTERING SATURATION REGION..." << std::endl;
    id = KP * (Vgs - Vt) * (Vgs - Vt) * (1 + lambda * Vds);
    Gds = KP * lambda * (Vgs - Vt) * (Vgs - Vt);
    gm = 2 * KP * (Vgs - Vt) * (1 + lambda * Vds);
  }

  IEQ = id - (Gds * Vds) - (gm * Vgs);

  //CAPACITOR DRAIN-GATE
  equiv.push_back( Component(CAPACITOR, "C_dg_" + nmos.designator, drain, gate, 1e-6) );

  //CAPACITOR GATE_SOURCE
  equiv.push_back( Component(CAPACITOR, "C_gs_" + nmos.designator, gate, source, 1e-6) );

  // VCCS
  equiv.push_back( Component(VCCS, "VCCS_" + nmos.designator, drain, source, gate, source, gm ) );

  // R
  equiv.push_back( Component(RESISTOR, "R_" + nmos.designator, drain, source, 1.0/Gds ) ); 
  
  // Current source 
  equiv.push_back( Component(CURRENT_SOURCE, "IEQ__" + nmos.designator, drain, source, IEQ ) ); 

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
    dc_sim = std::unique_ptr<DC_Simulator>(new DC_Simulator(lin_circuit));
    curr_voltages = dc_sim->get_voltage_vector();
    prev_voltages = std::vector<double>(curr_voltages.size(), 0); // Initialize prev_voltages to vector full of zeroes.
   
    int i;
    for (i = 0; i < MAX_ITERATIONS && !hasConverged(); i++)
    {
      // std::cout << "NEWTON RAPHSON ITER: " << i << std::endl;
      update_lin_circuit();
      dc_sim = std::unique_ptr<DC_Simulator>(new DC_Simulator(lin_circuit));
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
