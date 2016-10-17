/*
Main benchmarking function that calls S2_Gillespie and Psampler.
*/
#include "benchmark.h"

/// Hard coded inputs ///
string file_input_nominals = "data/Mar21_S2N_05x_00_04_04_04_04_nominal_Objective1_PopLast_14cols.txt";
string file_output_nominals = "output_1.txt";

int seed  = 1;
int samples = 1000;
int max_rows = 1000;


/////////////////////////

// Forward function declarations
// parse command line arguments

int main() {
  cout << "Start of the main function." << endl;

  // INITIALIZATION OF THE GENERATOR ///////////////////////////////////////////////////////
  gsl_rng * current_gen = gsl_rng_alloc(gsl_rng_mt19937); // func gsl_rng_alloc returns pointer to new instance of generator of type T  
  gsl_rng_set(current_gen, seed); // Seed the generator with value 'seed'
  //void gsl_rng_free (gsl_rng * rother); // frees memory associated with r
  // Print information about the generator
  printf("Current pseudo-random generator is '%s' \n", gsl_rng_name(current_gen));
  cout << "The seed value is " << seed << "\n";
  ///////////////////////////////////////////////////////////////////////////////////////////


  // Open output file
  ostringstream conv_outfile;   // stream used for the conversion
  conv_outfile << file_output_nominals;
  string str_outfile = conv_outfile.str(); 
  ofstream FILE_nominal_params(str_outfile.c_str(), ios_base::out);
  if(!FILE_nominal_params) { 
    cout << "failed to open file.\n"; 
    return 1;
  }

  // File header

  // Read nominal parameters file and extract nominals-noise vector
  std::vector< std::vector<double> > vec_vec_nominals = parse_nominals(file_input_nominals); // print_vector_of_vectors_doubles(vec_vec_nominals);
  int rows = vec_vec_nominals.size();
  cout << "Number of nominal parameter rows in input file: " << rows << endl;
  //for (vector< vector<double> >::const_iterator irow = vec_vec_nominals.begin(); irow != vec_vec_nominals.end(); ++irow) {
  int max_idx = min(rows, max_rows);
  std::vector<double> means_mRNA_Psampler, means_prot_Psampler, vars_mRNA_Psampler, vars_prot_Psampler;
  std::vector<double> means_mRNA_Gillespie, means_prot_Gillespie, vars_mRNA_Gillespie, vars_prot_Gillespie;

  int count_row = 0;
  for (vector< vector<double> >::const_iterator irow = vec_vec_nominals.begin(); irow != vec_vec_nominals.begin()+max_idx; ++irow) {
    count_row++;
    vector<double> nominals_kr_kf_noise = *irow;
    cout << "Processing (row: "<< count_row << ") nominals-noise vector: " << endl;
    print_vector_doubles(nominals_kr_kf_noise);

    // Simulate 1k times with Psampler and get duration
    double runtime_Psampler = extrinsic_noise_S2(nominals_kr_kf_noise, samples, means_mRNA_Psampler, means_prot_Psampler, vars_mRNA_Psampler, vars_prot_Psampler, current_gen, Psampler);
    cout << "runtime_Psampler = " << runtime_Psampler << endl;

    // Simulate 1k times with Gillespie and get duration
    double runtime_Gillespie = extrinsic_noise_S2(nominals_kr_kf_noise, samples, means_mRNA_Gillespie, means_prot_Gillespie, vars_mRNA_Gillespie, vars_prot_Gillespie, current_gen, S2_Gillespie);
    cout << "runtime_Gillespie = " << runtime_Gillespie << endl;

    // Generate output vector and write it to a file
    write_vector_doubles_tofile(nominals_kr_kf_noise, FILE_nominal_params); // Write nominals to file
    // Write summary stats to file
    FILE_nominal_params << means_mRNA_Psampler.back() << ", " <<  means_prot_Psampler.back() << ", " <<  vars_mRNA_Psampler.back() << ", " <<  vars_prot_Psampler.back() << ", ";
    FILE_nominal_params << means_mRNA_Gillespie.back() << ", " <<  means_prot_Gillespie.back() << ", " <<  vars_mRNA_Gillespie.back() << ", " <<  vars_prot_Gillespie.back() << ", ";
    // Write simulation times to file
    FILE_nominal_params << runtime_Psampler << ", ";
    FILE_nominal_params << runtime_Gillespie <<
    FILE_nominal_params << endl;
  }

  // Close output file
  FILE_nominal_params.close();  

  return(0);
}

// Function declarations
