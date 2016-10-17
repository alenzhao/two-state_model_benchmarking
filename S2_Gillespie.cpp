/*
This is a minimalistic implementation of the Gillespie algorithim using functions from the GSL library.
This file includes a hard-coded 2-state model of gene expression intended to be used for benchmarking.
*/

#include "benchmark.h"

// The two-state model of gene expression is hard-coded with the Gillespie algorithm in this function.
void S2_Gillespie(double duration, vector<double> perturbed, vector<int> &finals_mRNA, vector<int> &finals_protein, gsl_rng*current_gen ) {

  // Indices of mRNA molecule count and protein molecule count in the state space array.
  int idx_mRNA = 2;
  int idx_protein = 3;

  double t0 = 0.0; // Initial time
  //// Two-state model ////
  // Input stoichiometry matrix 
  const int S_rows = 4; // NOTE THESE MUST MATCH THE MATRIX
  const int S_cols = 7; // NOTE THESE MUST MATCH THE MATRIX
  // const int t_X_col = S_rows +1; // THESE MUST MATCH THE MATRIX
  int Pre[S_rows*S_cols] = {
    1,  0,  1,  0,  0,  0,  0,  
    0,  1,  0,  1,  0,  0,  0,  
    0,  0,  0,  0,  1,  1,  0,  
    0,  0,  0,  0,  0,  0,  1,  
  };
  int Post[S_rows*S_cols] = {
    0,  1,  1,  0,  0,  0,  0,  
    1,  0,  0,  1,  0,  0,  0,  
    0,  0,  1,  1,  1,  0,  0,  
    0,  0,  0,  0,  1,  0,  0,  
  };
  double Stoic[S_rows*S_cols]; // declare stoichiometry matrix

  for (int irow = 0; irow < S_rows; irow++) {
    for (int icol = 0; icol < S_cols; icol++) {
      int index = icol + S_cols*irow;
      //cout << "index="<< index << endl;
      // Pre[index] = Pre2d[index];
      Stoic[index] = Post[index] - Pre[index];
      //cout << "vec_Pre.at(index) = "<< vec_Pre.at(index) << endl;
      //cout << "vec_Stoic.at(index) = "<< vec_Stoic.at(index) << endl;
    }
  }
/*
  // Print out Pre, Post and stoichiometry matrices
  cout << "Pre: "<< endl;
  for (int irow = 0; irow < S_rows; irow++) {
    for (int icol = 0; icol < S_cols; icol++) {
      cout << Pre[ icol + S_cols*irow ] << ", ";
    }
    cout << endl;
  }
  cout << endl;
  cout << "Post: "<< endl;
  for (int irow = 0; irow < S_rows; irow++) {
    for (int icol = 0; icol < S_cols; icol++) {
      cout << Post[ icol + S_cols*irow ] << ", ";
    }
    cout << endl;
  }
  cout << endl;
  cout << "Stoic: "<< endl;
  for (int irow = 0; irow < S_rows; irow++) {
    for (int icol = 0; icol < S_cols; icol++) {
      cout << Stoic[ icol + S_cols*irow ] << ", ";
    }
    cout << endl;
  }
  cout << endl;
*/
  /////////////////////////

  // Check perturbed vector size is 7
  int lenvec = perturbed.size();
  if (lenvec != S_cols) {
    std::ostringstream msg;
    msg << "ERROR (in function Psamper): Expecting input vector to contain "<< S_cols << " elements but found: " << lenvec << endl;
    throw ExceptionVectorSize(msg.str());
  }

  // Definition of array of rate parameters
  double c_rxn[S_cols];
  memcpy( c_rxn, &perturbed[0], sizeof( double ) * perturbed.size() ); // becasue too lazy to rewrite the function to use a vector

  double TotalHaz, TempHaz, RanHaz;
  double Haz[S_cols];
  double SimTime = 0.0;
  
  int SimState[] = {0, 0, 0, 0};
  int lengthSimState = 4; // Note: this must match the above line
  // Randomly select initial condition / state (Note t_X[0] is time)   
  if ( gsl_rng_uniform(current_gen) <= c_rxn[0]/(c_rxn[0] + c_rxn[1]) ) {
    SimState[1] = 1.0;
  }
  else {  SimState[0] = 1.0; };

  // Gillespie algorithm in while loop
  while (SimTime <= duration) { // Actually the condition for ending this should not be reached as the loop should be terminated by a break statement after checking the time to the next reaction.
     
    // INPUT RATE LAWS
    Haz[0] = c_rxn[0]*(SimState[0]);
    Haz[1] = c_rxn[1]*(SimState[1]);
    Haz[2] = c_rxn[2]*c_rxn[3]*(SimState[0]); // This is because the parameter describing the inactive transcription rate is passed in as a fraction of the active transcription rate (rather then an absolute rate)
    Haz[3] = c_rxn[3]*(SimState[1]);
    Haz[4] = c_rxn[4]*(SimState[2]);
    Haz[5] = c_rxn[5]*(SimState[2]);
    Haz[6] = c_rxn[6]*(SimState[3]);
    
    // Check that there are enough molecules for a reaction to occur, if not, set its hazard to zero.
    for(int jS_col=0; jS_col < S_cols; jS_col++){ // each column of Pre
      for(int iS_row=0; iS_row < S_rows; iS_row++){ // each row of Pre
        //cout << Pre[ jS_col + iS_row*S_cols ] <<"/"<< SimState[iS_row] <<", ";
        if ( SimState[iS_row] < Pre[ jS_col + iS_row*S_cols ] ) {
          //cout << "|Pre[" << jS_col + iS_row*S_cols << "] = " << Pre[ jS_col + iS_row*S_cols ] <<"|  ";
          Haz[jS_col] = 0.0;
        }
      }
    }
    
    // Calculate total hazard
    TotalHaz = 0.0;
    for (int ih=0; ih < S_cols; ih++) {
      TotalHaz += Haz[ih];
    }

    // Get out clause in case no reactions are occuring 
    if (TotalHaz == 0.0) {     
      // Write final mRNA and protein counts to vectors outside this function
      finals_mRNA.push_back(SimState[idx_mRNA]);
      finals_protein.push_back(SimState[idx_protein]);
      break; // infinitely long wait until next reaction (avoid divide by zero)
    }
    
    // Choose time to the next reaction using exponential distribution with parameter 1/TotalHaz
    double tau = gsl_ran_exponential(current_gen, (1.0 / TotalHaz) );
    
    // Update current system time and check if it exceeds the final time point
    SimTime += tau;
    if (SimTime >= duration) {
      SimTime = SimTime - tau; // Reset time back to that of the previous reaction!
      // Write final mRNA and protein counts to vectors outside this function
      finals_mRNA.push_back(SimState[idx_mRNA]);
      finals_protein.push_back(SimState[idx_protein]);
      break; // The next reaction crosses the duration point so stop here.
    }
    
    // Choose which reaction occurs according to relative hazards
    TempHaz = 0.0; // temporary sum of hazards used to select reaction
    RanHaz = TotalHaz*gsl_rng_uniform(current_gen); // random number 0 <= ra < 1 to select reaction
    int irxn = -1;
    while (RanHaz >= TempHaz) {
      irxn++;
      TempHaz += Haz[irxn];
    }
    
    // Change the number of molecules according to the stoichiometry matrix
    for (int isp=0; isp < S_rows; isp++) { // loop over species
      SimState[isp] += Stoic[ irxn + isp*S_cols ];
    }
  } // end trajectory
}

// Scraps of old code:

      /*/ Save current time and state space to vectors
      vec_traj_time.push_back(duration);
      for (int is=0; is < S_rows; is++) {
        vec_traj_space.push_back(SimState[is]);
      }
      vec_traj_space.push_back(tele_state_vec.at(itp));
      */

      /*/ Update averages for each species
      double tn = t_final-SimTime;
      double tnm1 = t_final-SimTime-tau;
      if (tn >= ExpAvgTmin) { // this is to avoid trying to calculate extreme exp values
        for (int is=0; is < S_rows; is++) {
          // Update ExpAvg 
          for (int iexp = 0; iexp < lengthExpAvg; iexp++){
            ExpAvg[is][iexp] += SimState[is]*(1/ExpNorm[iexp])*(1/Lambda[iexp])*(-exp(-Lambda[iexp]*tnm1) + exp(-Lambda[iexp]*tn) );
          }
        }
      }
      // Update UnifAvg
      for (int is=0; is < S_rows; is++) {
        for (int iuni = 0; iuni < lengthUnifAvg; iuni++){
          if (tn >= tUnifAvg[iuni] && tnm1 >= tUnifAvg[iuni]){
            UnifAvg[is][iuni] += SimState[is]*(tn-tnm1)/tUnifAvg[iuni]  ;
          }
          else if (tn >= tUnifAvg[iuni]){ // in case the threshold is only crossed by the next rxn
            UnifAvg[is][iuni] += SimState[is]*(tn-(t_final-tUnifAvg[iuni]))/tUnifAvg[iuni]  ;
          }
        }
      }
      */ 

      // Write time and space trajectory to binary output files
      //FILE_time.write((const char *) &vec_traj_time[0], vec_traj_time.size() * sizeof (double));
      //FILE_traj.write((const char *) &vec_traj_space[0], vec_traj_space.size() * sizeof (int));
      
      //std::copy(vec_traj_space.begin(), vec_traj_space.end(), std::ostreambuf_iterator<char>(FILE_traj));
      //std::copy(vec_traj_time.begin(), vec_traj_time.end(), std::ostreambuf_iterator<char>(FILE_time));
      // } end if (duration == t_final)    

/*
    if (SimTime >= duration) {
      SimTime = SimTime - tau; // Reset time back to that of the previous reaction!

      // if (duration == t_final){ // if this is the last segment of the piecewise simulation
      // Update the trajectory and final-state output files.
      for (int ip = 0; ip < S_cols; ip++) {
        FILE_finals << c_rxn[ip] << ", " ;
      }
      FILE_finals << SimTime << ", ";
      for (int is=0; is < S_rows; is++) {
        //traj << t_X[is] << ", " ;
        FILE_finals << SimState[is] << ", " ;
        // for each averaging time period or lambda parameter
        for (int iexp = 0; iexp < lengthExpAvg; iexp++ ){
          FILE_ExpAvg << ExpAvg[is][iexp] << ", " ;
        }
        for (int iuni = 0; iuni < lengthUnifAvg; iuni++) {
          FILE_UnifAvg << UnifAvg[is][iuni] << ", " ;
        }
      }

      break; // The next reaction crosses the duration point so stop here.
    }
*/

// EOF
