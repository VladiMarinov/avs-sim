#include "linearizer.h"
#include "models.h"
#include <cmath>


std::vector<Component> Linearizer::linearize_diode(double VD, Component diode)
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

std::vector<Component> Linearizer::linearize_NPN(double Vbe, double Vbc, Component npn)
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

std::vector<Component> Linearizer::linearize_PNP(double Veb, double Vcb, Component pnp)
{
  std::vector<Component> equiv;

  models::PNP model_pnp;

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

std::vector<Component> Linearizer::linearize_NMOS(double Vgs, double Vds, Component nmos)
{
  std::vector<Component> equiv;

  models::NMOS model_nmos;

  std::string drain = nmos.nodes[0];
  std::string gate = nmos.nodes[1];
  std::string source = nmos.nodes[2];


  //double KP = model_nmos.KP;
  double lambda = model_nmos.lambda;
  double Vt = model_nmos.Vt;
  double beta = model_nmos.beta;
  double Cgd = model_nmos.Cgd;
  double Cgs = model_nmos.Cgs;

  double id = 0;
  double Gds = 0;
  double gm = 0;
  double IEQ = 0;
  bool isCutoff = false;

  // std::cout << "DEBUG: Vgs: " << Vgs << " , Vds: " << Vds<< " , Vt: " << Vt << std::endl;

  //CUT-OFF REGION
  if (Vgs < Vt)
  {
    isCutoff = true; 
    // std::cout << "ENTERING CUT-OFF REGION..." << std::endl;
    id = 0;
    Gds = 0;
    gm = 0;
  }
  //LINEAR REGION
  if ((Vgs >= Vt) && (Vgs - Vt > Vds)) // think about Vds >=0 
  {
    // std::cout << "ENTERING LINEAR REGION..." << std::endl;
    id = beta * ( (Vgs - Vt) * Vds - 0.5 * Vds * Vds);
    Gds = beta * (Vgs - Vt - Vds);
    gm = beta * Vds; 
  }
  //SATURATION REGION
  if ((Vgs  >= Vt) && (Vgs - Vt <= Vds))
  {
    // std::cout << "ENTERING SATURATION REGION..." << std::endl;
    id = 0.5 * beta * (Vgs - Vt) * (Vgs - Vt) * (1 + lambda * Vds);
    Gds = 0.5 * beta * lambda * (Vgs - Vt) * (Vgs - Vt);
    gm = beta * (Vgs - Vt) * (1 + lambda * Vds);
  }

  IEQ = id - (Gds * Vds) - (gm * Vgs);

  //CAPACITOR DRAIN-GATE
  equiv.push_back( Component(CAPACITOR, "C_dg_" + nmos.designator, drain, gate, Cgd) );

  //CAPACITOR GATE_SOURCE
  equiv.push_back( Component(CAPACITOR, "C_gs_" + nmos.designator, gate, source, Cgs) );

  if (!isCutoff){
    // VCCS
    equiv.push_back( Component(VCCS, "VCCS_" + nmos.designator, drain, source, gate, source, gm ) );

  // R
    if (Gds != 0.0) // If in cutoff : create infinite resistance by not adding conductance; otherwise : add resistor
    {
      // std::cout << "Gds : "<< Gds << std::endl;
      equiv.push_back( Component(RESISTOR, "R_" + nmos.designator, drain, source, 1.0/Gds ) );
    }
      
    // Current source 
    equiv.push_back( Component(CURRENT_SOURCE, "IEQ__" + nmos.designator, drain, source, IEQ ) ); 
  }
  
  return equiv;
}

std::vector<Component> Linearizer::linearize_PMOS(double Vgs, double Vds, Component pmos)
{
  std::vector<Component> equiv;

  models::PMOS model_pmos;

  std::string drain = pmos.nodes[0];
  std::string gate = pmos.nodes[1];
  std::string source = pmos.nodes[2];


  //double KP = model_nmos.KP;
  double lambda = model_pmos.lambda;
  double Vt = model_pmos.Vt;
  double beta = model_pmos.beta;

  double id = 0;
  double Gds = 0;
  double gm = 0;
  double IEQ = 0;
  bool isCutoff = false;

  // std::cout << "DEBUG: Vgs: " << Vgs << " , Vds: " << Vds<< " , Vt: " << Vt << std::endl;


  //CUT-OFF REGION
  if (Vgs > Vt)
  {
    isCutoff = true; 
    //std::cout << "ENTERING CUT-OFF REGION..." << std::endl;
    id = 0;
    Gds = 0;
    gm = 0;
  }
  //LINEAR REGION
  if ((Vgs <= Vt) && (Vgs - Vt < Vds)) // think about Vds >=0 
  {
    //std::cout << "ENTERING LINEAR REGION..." << std::endl;
    id =  -beta * ( (Vgs - Vt) * Vds - 0.5 * Vds * Vds);
    Gds = -beta * (Vgs - Vt - Vds);
    gm = -beta * Vds; 
  }
  //SATURATION REGION
  if ((Vgs  <= Vt) && (Vgs - Vt >= Vds))
  {
    //std::cout << "ENTERING SATURATION REGION..." << std::endl;
    id =  -0.5 * beta * (Vgs - Vt) * (Vgs - Vt) * (1 + lambda * Vds);
    Gds = 0.5 * beta * lambda * (Vgs - Vt) * (Vgs - Vt);
    gm = -beta * (Vgs - Vt) * (1 + lambda * Vds);
  }

  IEQ = id - (Gds * Vds) - (gm * Vgs);

  //CAPACITOR DRAIN-GATE
  equiv.push_back( Component(CAPACITOR, "C_dg_" + pmos.designator, drain, gate, 1e-6) );

  //CAPACITOR GATE_SOURCE
  equiv.push_back( Component(CAPACITOR, "C_gs_" + pmos.designator, gate, source, 1e-6) );

  if (!isCutoff){
    // VCCS
    equiv.push_back( Component(VCCS, "VCCS_" + pmos.designator, drain, source, gate, source, gm ) );

  // R
    if (Gds != 0.0) // If in cutoff : create infinite resistance by not adding conductance; otherwise : add resistor
    {
      // std::cout << "Gds : "<< Gds << std::endl;
      equiv.push_back( Component(RESISTOR, "R_" + pmos.designator, drain, source, 1.0/Gds ) );
    }
      
    // Current source 
    equiv.push_back( Component(CURRENT_SOURCE, "IEQ__" + pmos.designator, drain, source, IEQ ) ); 
  }
  
  return equiv;
}



//--------------------- SMALL SIGNAL ---------------------



std::vector<Component> Linearizer::small_signal_diode(double VD, Component diode)
{
    std::vector<Component> equiv;

    models::D D_;
    double GD = D_.IS * std::exp(VD/D_.VT) / D_.VT;

    equiv.push_back( Component(RESISTOR, "R" + diode.designator, diode.nodes[0], diode.nodes[1], 1/GD));

    return equiv;
}

std::vector<Component> Linearizer::small_signal_NPN(double Vbe, double Vbc, Component npn)
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

  // WARNING: the order of the following push_backs is important - DO NOT REORDER
  // This is due to the fact that the node order in the components must come up first as CBE.
  // otherwise u risk messing up the order with respect to the parsed circuit.

  // VCCS base-collector
  equiv.push_back( Component(VCCS, "VCCS_bc_" + npn.designator, collector, base, base, emmiter, gce ) ); 

  // Ree
  equiv.push_back( Component(RESISTOR, "Ree" + npn.designator, base, emmiter, 1.0/gee ) ); 
  
  // VCCS base-emmiter
  equiv.push_back( Component(VCCS, "VCCS_be_" + npn.designator, emmiter, base, base, collector, gec ) );

  // Rcc
  equiv.push_back( Component(RESISTOR, "Rcc" + npn.designator, base, collector, 1.0/gcc ) ); 
  
  return equiv;
}

std::vector<Component> Linearizer::small_signal_PNP(double Veb, double Vcb, Component pnp)
{
  std::vector<Component> equiv;

  models::PNP model_pnp;

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

  // WARNING: the order of the following push_backs is important - DO NOT REORDER
  // This is due to the fact that the node order in the components must come up first as CBE.
  // otherwise u risk messing up the order with respect to the parsed circuit.

  // Rcc
  equiv.push_back( Component(RESISTOR, "Rcc" + pnp.designator, collector, base, 1.0/gcc ) ); 

  // VCCS base-emmiter
  equiv.push_back( Component(VCCS, "VCCS_eb_" + pnp.designator, base, emmiter, collector, base, gec ) );

  // VCCS base-collector
  equiv.push_back( Component(VCCS, "VCCS_cb_" + pnp.designator, base, collector, emmiter, base, gce ) ); 

  // Ree
  equiv.push_back( Component(RESISTOR, "Ree" + pnp.designator, base, emmiter, 1.0/gee ) ); 
  
  return equiv;
}

std::vector<Component> Linearizer::small_signal_NMOS(double Vgs, double Vds, Component nmos)
{
  std::vector<Component> equiv;

  models::NMOS model_nmos;

  std::string drain = nmos.nodes[0];
  std::string gate = nmos.nodes[1];
  std::string source = nmos.nodes[2];

  double lambda = model_nmos.lambda;
  double Vt = model_nmos.Vt;
  double beta = model_nmos.beta;
  double Cgd = model_nmos.Cgd;
  double Cgs = model_nmos.Cgs;

  double Gds = 0;
  double gm = 0;
  bool isCutoff = false;

  // std::cout << "DEBUG: Vgs: " << Vgs << " , Vds: " << Vds<< " , Vt: " << Vt << std::endl;

  //CUT-OFF REGION
  if (Vgs < Vt)
  {
    isCutoff = true; 
    // std::cout << "ENTERING CUT-OFF REGION..." << std::endl;
    Gds = 0;
    gm = 0;
  }
  //LINEAR REGION
  if ((Vgs >= Vt) && (Vgs - Vt > Vds)) // think about Vds >=0 
  {
    // std::cout << "ENTERING LINEAR REGION..." << std::endl;
    Gds = beta * (Vgs - Vt - Vds);
    gm = beta * Vds; 
  }
  //SATURATION REGION
  if ((Vgs  >= Vt) && (Vgs - Vt <= Vds))
  {
    // std::cout << "ENTERING SATURATION REGION..." << std::endl;
    Gds = 0.5 * beta * lambda * (Vgs - Vt) * (Vgs - Vt);
    gm = beta * (Vgs - Vt) * (1 + lambda * Vds);
  }

  //CAPACITOR DRAIN-GATE
  equiv.push_back( Component(CAPACITOR, "C_dg_" + nmos.designator, drain, gate, Cgd) );

  //CAPACITOR GATE_SOURCE
  equiv.push_back( Component(CAPACITOR, "C_gs_" + nmos.designator, gate, source, Cgs) );

  if (!isCutoff){
    // VCCS
    equiv.push_back( Component(VCCS, "VCCS_" + nmos.designator, drain, source, gate, source, gm) );

    // R
    if (Gds != 0.0) // If in cutoff : create infinite resistance by not adding conductance; otherwise : add resistor
    {
      // std::cout << "Gds : "<< Gds << std::endl;
      equiv.push_back( Component(RESISTOR, "R_" + nmos.designator, drain, source, 1.0/Gds) );
    }
  }
  
  return equiv;
}

std::vector<Component> Linearizer::small_signal_PMOS(double Vgs, double Vds, Component pmos)
{
  std::vector<Component> equiv;

  models::PMOS model_pmos;

  std::string drain = pmos.nodes[0];
  std::string gate = pmos.nodes[1];
  std::string source = pmos.nodes[2];


  //double KP = model_nmos.KP;
  double lambda = model_pmos.lambda;
  double Vt = model_pmos.Vt;
  double beta = model_pmos.beta;
  double Cgd = model_pmos.Cgd;
  double Cgs = model_pmos.Cgs;

  double Gds = 0;
  double gm = 0;
  bool isCutoff = false;

  // std::cout << "DEBUG: Vgs: " << Vgs << " , Vds: " << Vds<< " , Vt: " << Vt << std::endl;


  //CUT-OFF REGION
  if (Vgs > Vt)
  {
    isCutoff = true; 
    std::cout << "ENTERING CUT-OFF REGION..." << std::endl;
    Gds = 0;
    gm = 0;
  }
  //LINEAR REGION
  if ((Vgs <= Vt) && (Vgs - Vt < Vds)) // think about Vds >=0 
  {
    std::cout << "ENTERING LINEAR REGION..." << std::endl;
    Gds = -beta * (Vgs - Vt - Vds);
    gm = -beta * Vds; 
  }
  //SATURATION REGION
  if ((Vgs  <= Vt) && (Vgs - Vt >= Vds))
  {
    std::cout << "ENTERING SATURATION REGION..." << std::endl;
    Gds = 0.5 * beta * lambda * (Vgs - Vt) * (Vgs - Vt);
    gm = -beta * (Vgs - Vt) * (1 + lambda * Vds);
  }

  //CAPACITOR DRAIN-GATE
  equiv.push_back( Component(CAPACITOR, "C_dg_" + pmos.designator, drain, gate, Cgd) );

  //CAPACITOR GATE_SOURCE
  equiv.push_back( Component(CAPACITOR, "C_gs_" + pmos.designator, gate, source, Cgs) );

  if (!isCutoff){
    // VCCS
    equiv.push_back( Component(VCCS, "VCCS_" + pmos.designator, drain, source, gate, source, gm ) );

  // R
    if (Gds != 0.0) // If in cutoff : create infinite resistance by not adding conductance; otherwise : add resistor
    {
      // std::cout << "Gds : "<< Gds << std::endl;
      equiv.push_back( Component(RESISTOR, "R_" + pmos.designator, drain, source, 1.0/Gds ) );
    }
  }
  
  return equiv;
}

