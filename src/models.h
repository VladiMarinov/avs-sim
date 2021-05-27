#pragma once

namespace models
{

struct D
{
  const double IS = 1e-14;
  const double VT = 0.026; 
  const double initial_V_guess = 0.7;
};

/// BASED ON LTSPICE 2N2222
struct NPN
{
  const double IS = 1e-14;
  const double BF = 200.0;
  const double BR = 3.0;
  const double VT = 0.026;

  const double initial_Vbe_guess = 0.7;
  const double initial_Vbc_guess = -1.0;

  double AF;
  double AR;
  double IES;
  double ICS;
  NPN()
  {
    AF = BF / (BF + 1);
    AR = BR / (BR + 1);
    IES = IS / AF;
    ICS = IS / AR;
  }
};

struct PNP
{
  const double IS = 1e-14;
  const double BF = 250.0;
  const double BR = 3.0;
  const double VT = 0.026;

  const double initial_Veb_guess = 0.7;
  const double initial_Vcb_guess = -1.0;

  double AF;
  double AR;
  double IES;
  double ICS;
  PNP()
  {
    AF = BF / (BF + 1);
    AR = BR / (BR + 1);
    IES = IS / AF;
    ICS = IS / AR;
  }
};

}


