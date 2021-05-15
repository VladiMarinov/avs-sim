#pragma once

#include <string>

/// Desribes a '.ac' directive, as seen in the Netlist.
class AC_Directive
{
public:
  /// The type of frequency sweep that the simulation will run.
  /// @note For now only Decade sweeps are expected and supported.
  std::string sweep_type;

  /// The points per decade to be simulated.
  std::string points_per_dec;

  /// The starting frequency of the simulation.
  std::string start_freq;

  /// The stop frequency of the simulation.
  std::string stop_freq;
};
