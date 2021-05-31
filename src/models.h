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

/// BASED ON LTSPICE 2N2907
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

/// BASED ON OWN MODEL OF NMOS
/// @note K = KP/2 * W/L
//  @note K = beta/2
struct NMOS
{
 const double KP = 0.2;
 const double lambda = 0.01;
 const double Vt = 2;
 const double W = 20;
 const double L = 8;

 //guess saturation
 const double initial_Vgs_guess = 3;
 const double initial_Vds_guess = 0.5;

 double beta;
 NMOS()
 {
    beta = KP * (W/L); //width - length ratio
   // beta = 4;
 }
};

struct PMOS
{
  const double KP = 0.2;
  const double lambda = 0.01;
  const double Vt = -0.8;
  const double W = 1;
  const double L = 1;

  //guess saturation
  const double initial_Vgs_guess = -1.0;
  const double initial_Vds_guess = -2.0;

  double beta;
  PMOS()
  {
    beta = KP * (W/L); //width - length ratio 
  }

};

}


