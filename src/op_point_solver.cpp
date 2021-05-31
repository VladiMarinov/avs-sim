#include "op_point_solver.h"
#include "util.h"
#include "models.h"
#include <cmath>

#include "linearizer.h"

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
            std::vector<Component> equiv_components = Linearizer::linearize_diode(models::D().initial_V_guess, component);
            lin_components.push_back(equiv_components[0]);
            lin_components.push_back(equiv_components[1]);
        }
        else if (component.type == BJT)
        {
          std::vector<Component> equiv_components;
          if (component.model_value->model_name == "NPN")
          {
            equiv_components = Linearizer::linearize_NPN(models::NPN().initial_Vbe_guess, models::NPN().initial_Vbc_guess, component);
          }
          else if (component.model_value->model_name == "PNP")
          {
            equiv_components = Linearizer::linearize_PNP(models::PNP().initial_Veb_guess, models::PNP().initial_Vcb_guess, component);
          }
          
          for (Component c : equiv_components)
          {
            lin_components.push_back(c);
          }
        }
        else if (component.type == MOSFET)
        {
          std::vector<Component> equiv_components;
          if (component.model_value->model_name == "NMOS")
          {
            equiv_components = Linearizer::linearize_NMOS(models::NMOS().initial_Vgs_guess, models::NMOS().initial_Vds_guess, component);
          }
          else if (component.model_value->model_name == "PMOS")
          {
            equiv_components = Linearizer::linearize_PMOS(models::PMOS().initial_Vgs_guess, models::PMOS().initial_Vds_guess, component);
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
            std::vector<Component> equiv_components = Linearizer::linearize_diode(diode_voltage, component);
            lin_components.push_back(equiv_components[0]);
            lin_components.push_back(equiv_components[1]);
        }
        else if (component.type == BJT)
        {
            double Vbe = util::voltage_between_nodes(circuit, component.nodes[1], component.nodes[2], curr_voltages);
            double Vbc = util::voltage_between_nodes(circuit, component.nodes[1], component.nodes[0], curr_voltages);
            std::vector<Component> equiv_components;
            
            if (component.model_value->model_name == "NPN")
            {
              equiv_components = Linearizer::linearize_NPN(Vbe, Vbc, component);
            }
            else if (component.model_value->model_name == "PNP")
            {
              equiv_components = Linearizer::linearize_PNP(-Vbe, -Vbc, component);
            }

            for (Component c : equiv_components)
            {
              lin_components.push_back(c);
            }
        }
        else if (component.type == MOSFET)
        {
          double Vgs = util::voltage_between_nodes(circuit, component.nodes[1], component.nodes[2], curr_voltages);
          double Vds = util::voltage_between_nodes(circuit, component.nodes[0], component.nodes[2], curr_voltages);
          std::vector<Component> equiv_components;
          if (component.model_value->model_name == "NMOS")
          {
            equiv_components = Linearizer::linearize_NMOS(Vgs, Vds, component);
          }
          else if (component.model_value->model_name == "PMOS")
          {
            equiv_components = Linearizer::linearize_PMOS(Vgs, Vds, component);
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
    // for (Component c : lin_circuit.circuit_components)
    // {
      // util::printComponent(c);
    // }
}

bool OP_Point_Solver::hasConverged()
{
  for(uint32_t i = 0; i < curr_voltages.size(); i++)
  {
    if (std::isnan(curr_voltages[i]) || std::isinf(curr_voltages[i]))
    {
      std::cout << "Newton-Raphson Solver encountered NaN or Infinity... This is most likely due to the supplied circuit being unsolvable in certain conditions. See if any nodes have a very high impedance to ground!" << std::endl;
      exit(EXIT_FAILURE);
    }
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
