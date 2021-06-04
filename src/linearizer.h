#include <vector>
#include "component.h"

/// A utility class that contains function for DC-linearization and Small-signal linearization of non-linear components.
class Linearizer 
{
public:
  /// Returns the equivalent linearized model components of the Diode.
  /// The linearized model is made around the supplied operating point voltages.
  static std::vector<Component> linearize_diode(double VD, Component diode); 

  /// Returns the equivalent linearized model components of the NPN.
  /// The linearized model is made around the supplied operating point voltages.
  static std::vector<Component> linearize_NPN(double Vbe, double Vbc, Component npn); 

  /// Returns the equivalent linearized model components of the PNP.
  /// The linearized model is made around the supplied operating point voltages.
  static std::vector<Component> linearize_PNP(double Veb, double Vcb, Component pnp); 
  
  /// Returns the equivalent linearized model components of the N-channel MOSFET.
  /// The linearized model is made around the supplied operating point voltages.
  static std::vector<Component> linearize_NMOS(double Vgs, double Vds, Component nmos); 

  /// Returns the equivalent linearized model components of the P-channel MOSFET.
  /// The linearized model is made around the supplied operating point voltages.
  /// @warning This is currently broken and does not give correct linearized models.
  static std::vector<Component> linearize_PMOS(double Vgs, double Vds, Component pmos); 

  /// Returns the equivalent small-signal model components of the Diode.
  /// The linearized model is made around the supplied operating point voltages.
  static std::vector<Component> small_signal_diode(double VD, Component diode);

  /// Returns the equivalent small-signal model components of the NPN.
  /// The linearized model is made around the supplied operating point voltages.
  static std::vector<Component> small_signal_NPN(double Vbe, double Vbc, Component npn); 

  /// Returns the equivalent small-signal model components of the PNP.
  /// The linearized model is made around the supplied operating point voltages.
  static std::vector<Component> small_signal_PNP(double Veb, double Vcb, Component pnp); 

  /// Returns the equivalent small-signal model components of the N-channel MOSFET.
  /// The linearized model is made around the supplied operating point voltages.
  static std::vector<Component> small_signal_NMOS(double Vgs, double Vds, Component nmos); 

  /// Returns the equivalent small-signal model components of the P-channel MOSFET.
  /// The linearized model is made around the supplied operating point voltages.
  /// @warning This is currently broken and does not return a correct small-signal model.
  static std::vector<Component> small_signal_PMOS(double Vgs, double Vds, Component pmos); 
};
