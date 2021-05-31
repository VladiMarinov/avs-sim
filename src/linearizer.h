#include <vector>
#include "component.h"

class Linearizer 
{
public:
  static std::vector<Component> linearize_diode(double VD, Component diode); 
  static std::vector<Component> linearize_NPN(double Vbe, double Vbc, Component npn); 
  static std::vector<Component> linearize_PNP(double Veb, double Vcb, Component pnp); 
  static std::vector<Component> linearize_NMOS(double Vgs, double Vds, Component nmos); 
  static std::vector<Component> linearize_PMOS(double Vgs, double Vds, Component pmos); 
};
