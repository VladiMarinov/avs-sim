#include "avs-sim.h"
#include "util.h"
#include <iomanip>
#include <utility>


AVS_sim::AVS_sim(std::string inputfile_name)
{
    parser = std::unique_ptr<Parser> (new Parser (std::move(inputfile_name)));
    parser->parse();
    circuit = std::unique_ptr<Circuit> (new Circuit (parser->components));
    ac_dir = parser->ac_dir;
    has_AC_directive = parser->has_found_AC_directive() ;
}

void AVS_sim::simulate()
{
    OP_Point_Solver op_point_solver(circuit->get_DC_Equivalent_Circuit());
    op_point_solver.solve();
    DC_voltages = op_point_solver.curr_voltages;
    if (has_AC_directive)
    {
      itterate_over_ac();
    }
}

void AVS_sim::itterate_over_ac()
{
    std::ofstream output_file;
    output_file.open("output.txt");

    output_file << "Freq, Value\n";

    uint32_t points_per_decade = ac_dir.points_per_dec;
    double current_frequency = 0;
    double start_freq = ac_dir.start_freq.numeric_value;
    double stop_frequency = ac_dir.stop_freq.numeric_value;

    for (int n = 0; current_frequency < stop_frequency; n++ )
    {
        current_frequency = std::pow(10.0,  ((double)n) / points_per_decade ) * start_freq ;
        if (current_frequency > stop_frequency)
        {
            current_frequency = stop_frequency;
        }
        
        AC_Simulator ac_sim(circuit->get_AC_Equivalent_Circuit(), DC_voltages, current_frequency);
        output_file << std::setprecision(15);
        output_file << current_frequency << ", ";
        output_file << 20*std::log10(abs(util::voltage_at_node(*circuit, "out", ac_sim.get_voltage_vector()))); 
        output_file << std::endl;
    }
    
    output_file.close();
}



