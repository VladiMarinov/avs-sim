#pragma once

#include <string>
#include "const_value.h"

/// Desribes a '.ac' directive, as seen in the Netlist.
class AC_Directive
{
public:
  /// The type of frequency sweep that the simulation will run.
  /// @note For now only Decade sweeps are expected and supported.
  std::string sweep_type;

  /// The points per decade to be simulated.
  uint32_t points_per_dec;

  /// The starting frequency of the simulation.
  Const_value start_freq;

  /// The stop frequency of the simulation.
  Const_value stop_freq;
};
