/*
 Function for perturbing the nominal parameters of the two-state model
*/

/////////////////////////////////////////////////////////////////////////////
// input nominals-noise vector
// output perturbed parametrs
// separate rate and noise parameters from nominals
// Note that the noise for k0 and k1 is the same
/////////////////////////////////////////////////////////////////////////////

#include "benchmark.h"

//// Exceptions ////
struct ExceptionNanInf : public std::exception
{
  std::string s;
  ExceptionNanInf(std::string ss) : s(ss) {}
  ~ExceptionNanInf() throw () {} // Updated
  const char* what() const throw() { 
    cerr << "Exception - numerical error\n";
    return s.c_str(); 
  }
};

//// Functions ////
// Convert k_r and k_f to k_on and k_off
vector<double> rf2onoff(vector<double> nominals_noise) {
  // Check that length of input vector.  This is expected to be 14 and is specific to our 2-state model (hence not using a variable or #define)
  int lenvec = nominals_noise.size();
  if (lenvec != 14) {
    std::ostringstream msg;
    msg << "ERROR (rf2onoff): Expecting input vector to contain 14 elements but found: " << lenvec << endl;
    throw ExceptionVectorSize(msg.str());
  }
  // Do the conversion
  vector<double> converted = nominals_noise;
  double k_r = nominals_noise.at(0);
  if (k_r == 0.0) {
    std::ostringstream msg;
    msg << "ERROR (rf2onoff): k_r == 0.0 ("<< k_r << ")" << endl;
    throw ExceptionNanInf(msg.str());
  }
  double k_f = nominals_noise.at(1);
  double koff = k_f / (2.0 * k_r);
  double kon = koff*k_r / (1.0 - k_r);
  converted.at(0) = kon;
  converted.at(1) = koff;
  return(converted);
}

// Use values from vector of nominal parameters to calculate desired simulation duration
double get_duration(vector<double> nominals_noise) {
  int id_1 = 5; int id_2 = 6;
  // Check that length of input vector.  This is expected to be 14 and is specific to our 2-state model (hence not using a variable or #define)
  int lenvec = nominals_noise.size();
  if (lenvec != 14) {
    std::ostringstream msg;
    msg << "ERROR (extrinsic_noise_S2): Expecting input vector to contain 14 elements but found: " << lenvec << endl;
    throw ExceptionVectorSize(msg.str());
  }
  // Get degradation rates
  double d_1 = nominals_noise.at(id_1);
  double d_2 = nominals_noise.at(id_2);
  if (d_1 == 0.0 || d_2 == 0.0) {
    std::ostringstream msg;
    msg << "ERROR (get_duration): zero values in degradation rates (d_1="<< d_1 << ", d_2="<< d_2 <<")" << endl;
    throw ExceptionNanInf(msg.str());
  }
  // Calculate duration
  return(5.0/d_1 + 5.0/d_2);
}

// Use vector of noise parameters to perturb vector of nominal parameters return vector of perturbed parameters
vector<double> perturb_nominals(vector<double> nominals_noise, gsl_rng*current_gen ) {
  // Check that length of input vector.  This is expected to be 14 and is specific to our 2-state model (hence not using a variable or #define)
  int lenvec = nominals_noise.size();
  if (lenvec != 14) {
    std::ostringstream msg;
    msg << "ERROR (extrinsic_noise_S2): Expecting input vector to contain 14 elements but found: " << lenvec << endl;
    throw ExceptionVectorSize(msg.str());
  }
  // Split the rate and the noise parameters
  vector<double> c_nom (nominals_noise.begin(),nominals_noise.begin()+7);
  vector<double> n_nom (nominals_noise.begin()+7,nominals_noise.end());
  // Perturb nominal parameters with a gaussain - resampling if negative
  vector<double> perturbed;
  int S_col = c_nom.size();
  double c[S_col];
  for (int ip = 0; ip < S_col; ip++) { // cout <<  "Sample from Gaussian above zero" << endl;
    do {
      c[ip] = c_nom[ip] + gsl_ran_gaussian(current_gen, c_nom[ip] * n_nom[ip]); 
      // cout << "c[" << ip << "] = " << c[ip] << endl;      
    } while (c[ip] < 0.0);
    perturbed.push_back(c[ip]);
  } //FILE_perturbed_params << c[ip] << ", ";
  return(perturbed);
}

// Perturb and simulate 'samples' times with chosen function
double extrinsic_noise_S2(vector<double> nominals_kr_kf_noise, int samples, vector<double> &means_mRNA, vector<double> &means_protein, vector<double> &vars_mRNA, vector<double> &vars_protein, gsl_rng*current_gen,
  void (simulator)(double, vector<double>, vector<int> &, vector<int> &, gsl_rng* )
  ) {
  // Check that length of input vector.  This is expected to be 14 and is specific to our 2-state model (hence not using a variable or #define)
  int lenvec = nominals_kr_kf_noise.size();
  if (lenvec != 14) {
    std::ostringstream msg;
    msg << "ERROR (extrinsic_noise_S2): Expecting input vector to contain 14 elements but found: " << lenvec << endl;
    throw ExceptionVectorSize(msg.str());
  }
  // Calculate desired simulation duration
  double duration = get_duration(nominals_kr_kf_noise); 
  // Convert to k_on and k_off
  vector<double> nominals_kon_koff = rf2onoff(nominals_kr_kf_noise); 
  // Initialise vector for holding final simulation states
  vector<int> counts_mRNA, counts_prot;
  double total_simulation_time = 0.0;

  for (int irun=0; irun < samples; irun++){

    // Perturb nominal parameters
    vector<double> perturbed = perturb_nominals(nominals_kon_koff, current_gen);

    // Instantiate timer
    std::clock_t    start;
    start = std::clock(); // start timer
    // Simulate
    simulator(duration, perturbed, counts_mRNA, counts_prot, current_gen );
    // Add time to total simulation time
    total_simulation_time += (std::clock() - start) / (double)(CLOCKS_PER_SEC); // seconds
  }

  // Convert vectors to arrays and calculate means using GSL library function.
  double* arr_counts_mRNA = new double[samples];
  double* arr_counts_prot = new double[samples];

  for (int irun=0; irun < samples; irun++) {
    arr_counts_mRNA[irun] = (double) counts_mRNA.at(irun);
    arr_counts_prot[irun] = counts_prot.at(irun);
  }

  double mean1, mean2, var1, var2;
  mean1 = gsl_stats_mean(arr_counts_mRNA, 1, samples);
  mean2 = gsl_stats_mean(arr_counts_prot, 1, samples);
  var1  = gsl_stats_variance(arr_counts_mRNA, 1, samples);
  var2  = gsl_stats_variance(arr_counts_prot, 1, samples); 

  // cout << "PsamplerSuite - mean_mRNA = " << mean1 << endl;
  means_mRNA.push_back( mean1 );
  // cout << "PsamplerSuite - mean_prot = " << mean2 << endl;
  means_protein.push_back( mean2 );
  // cout << "PsamplerSuite - var_mRNA = " << var1 << endl;
  vars_mRNA.push_back( var1 );
  // cout << "PsamplerSuite - var_prot = " << var2 << endl;
  vars_protein.push_back( var2 ); 

  // cout << "PsamplerSuite - mean_mRNA = " << mean_mRNA << endl;
  // cout << "PsamplerSuite - mean_prot = " << mean_prot << endl;
  // cout << "PsamplerSuite - var_mRNA = " << var_mRNA << endl;
  // cout << "PsamplerSuite - var_prot = " << var_prot << endl;

  return(total_simulation_time);
}




// EOF
