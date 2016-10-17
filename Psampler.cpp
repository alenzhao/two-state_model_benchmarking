/*
 Psampler
*/

#include "benchmark.h"

void Psampler(double duration, vector<double> perturbed, vector<int> &finals_mRNA, vector<int> &finals_protein, gsl_rng*current_gen ) {
  int S_col = 7; // number of rate parameters
  double t0 = 0.0; // Initial time
  double tm = duration;

  // Check perturbed vector size is 7
  int lenvec = perturbed.size();
  if (lenvec != S_col) {
    std::ostringstream msg;
    msg << "ERROR (in function Psampler): Expecting input vector to contain "<< S_col << " elements but found: " << lenvec << endl;
    throw ExceptionVectorSize(msg.str());
  }

  // Definition of array of rate parameters
  double c[S_col];
  memcpy( c, &perturbed[0], sizeof( double ) * perturbed.size() ); // becasue too lazy to rewrite the function to use a vector

  ///////// TWO-STATE: Sample a single realisation of mRNA and Protein number at time tm. /////////////////////
  
  ///////// Sample a single realisation of mRNA and Protein number at time tm. ////////////////////////////////
  // Obtain switching event times based on the telegraph process         
  // {kon, koff, k0, k1, k2, d1, d2,};
  //   0    1    2   3   4   5   6
  
  // Initialise counters
  vector<double> vec_birth_times; // vector for storing mRNA production times
  
  double tele_hazards[] = { c[0], 0, c[1] }; // intialise hazards vector which is used to pick the appropriate switching rate 
  double tele_signals[] = { c[2]*c[3], 0, c[3] }; // signal strength vector used for picking signal strength
  
  // Select initial state
  int tele_state;
  if ( gsl_rng_uniform(current_gen) <= c[0]/(c[0] + c[1]) ) {
  tele_state = 1;
  }
  else {  tele_state = -1; };
  
  double tele_time = t0;      
  int tele_it = 0; // initialise time point counter
  while ( tele_time < tm ) {
    // Choose tau according to an exponential with parameter equal to 1/hazard
    double tele_tau = gsl_ran_exponential(current_gen, 1.0/(tele_hazards[1+tele_state]) ); 
    
    // Restrict t+tau to be below measurement time (tm)
    if (tele_time + tele_tau > tm) { tele_tau = tm - tele_time; };
    
    // Sample the number of production reactions occuring during tau
    int tau_num_mRNA_produced = gsl_ran_poisson(current_gen, tele_tau*tele_signals[ tele_state + 1 ]);
    
    // Obtain and store production times
    for (int ib = 1; ib <= tau_num_mRNA_produced; ib++) {
      double prod_time = tele_time + tele_tau * gsl_rng_uniform(current_gen);
      vec_birth_times.push_back(prod_time);
    }
    tele_time = tele_time + tele_tau; // and update current time
    tele_state = tele_state * -1; // switch to other state
  } // now vec_birth_times may be used as before to obtain protein production distributions
    
  int Pois_mRNA = 0; // mRNA at measurement time
  int Prot_tm = 0; // protein at measurement time
  for (int ipt = 0; ipt < (int)vec_birth_times.size(); ipt++) {
    // {kon, koff, k0, k1, k2, d1, d2,};
    //   0    1    2   3   4   5   6
    
    // Sample lifetimes
    double sampled_lifetime = gsl_ran_exponential(current_gen, 1.0 / (c[5]));
    
    // Use production times and lifetimes to obtain decay times
    double decay_time = vec_birth_times.at(ipt) + sampled_lifetime; 
    double effective_lifetime = sampled_lifetime;// altered in the next loop if it exceeds tm
    if (decay_time > tm) // mRNA survives until measurement time
    {
      Pois_mRNA++; 
      decay_time = tm;
      effective_lifetime = tm - vec_birth_times.at(ipt);
    }       
    
    // Obtain times since death
    double Tsd = (tm - decay_time);
    
    // Calculate p and the Poisson parameter for each mRNA
    double p = (1.0 / (c[6] * effective_lifetime))*(1 - exp(-c[6] * effective_lifetime));
    double PP = p * c[4] * effective_lifetime;
    
    // Sample protein produced by each mRNA from the Poisson distribution with corresponding parameter
    int PR = gsl_ran_poisson(current_gen, PP);
    
    // Obtain survival probability
    double ps = exp(-c[6] * Tsd); 
    
    // Sample number of surviving proteins at tm
    int PS_tm = gsl_ran_binomial(current_gen, ps, PR);
    
    // Update total number of surviving proteins at measurement time;
    Prot_tm = Prot_tm + PS_tm;  
  } //!
  // Write final mRNA and protein counts to vectors outside this function
  finals_mRNA.push_back(Pois_mRNA); // arr_X1[runs_done] = Pois_mRNA;
  finals_protein.push_back(Prot_tm); // arr_X2[runs_done] = Prot_tm;
  // return (0); // runs_done++;
} // end function SamplingRound

// EOF
