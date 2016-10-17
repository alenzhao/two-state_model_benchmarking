//Psampler_headers.h
#pragma once

#include <iostream> // needed for cout
#include <cstdlib> // some basic functions?
#include <fstream> // needed to write to file?

#include <stdio.h> // some more basic function?
#include <gsl/gsl_rng.h> // random number generator
#include <gsl/gsl_randist.h> // sample from exponential distribution
#include <gsl/gsl_sf_gamma.h> // gamma (and beta) function
#include <gsl/gsl_sf_hyperg.h> // hypergeometric function
#include <gsl/gsl_statistics.h> // for the mean and variance functions

#include <time.h> // timing functions used to determine duration of runs
#include <cmath> // needed for the floor function
#include <algorithm> // std::max // std::sort

#include <stdlib.h>    // for atoi
#include <assert.h>    // for assert
#include <math.h> // needed for power operator pow

#include <vector>
#include <sstream> // needed to convert int to string

#include <iomanip> // used for padding argument strings
#include <string> // used for padding argument strings

#include <iterator> // used when writing vector to file

// clock/timer
#include <ctime>

using namespace std;
// using std::cout;
// using std::endl;
// using std::vector;
// using std::ostringstream;
// using std::string;
// using std::ofstream;
// using std::ios;
// using std::ios_base;
// using std::max;

// Functions from S2_Gillespie.cpp
void S2_Gillespie(double, vector<double>, vector<int> &, vector<int> &, gsl_rng* );

// Functions from Psampler.cpp
void Psampler(double, vector<double>, vector<int> &, vector<int> &, gsl_rng* );

// Functions from parse_nominals.cpp (Note: these declarations should be left in as they are used for unit tests)
void print_vector_ints(vector<int>);
void print_vector_doubles(vector<double>);
void print_vector_of_vectors_doubles(vector< vector<double> >);
vector<double> line2vec_double(string, string);
vector< vector<double> >  parse_nominals(string);
void write_vector_doubles_tofile(vector<double>, std::ofstream &);

// Functions from extrinsic_noise.cpp
vector<double> rf2onoff(vector<double>);
double get_duration(vector<double>);
vector<double> perturb_nominals(vector<double>, gsl_rng*);
double extrinsic_noise_S2(vector<double>, int, vector<double> &, vector<double> &, vector<double> &, vector<double> &, gsl_rng*,
  void (simulator)(double, vector<double>, vector<int> &, vector<int> &, gsl_rng* )
  );

// Struct definitions (probably should only declare and put definition in cpp file)
struct ExceptionVectorSize : public std::exception
{
  std::string s;
  ExceptionVectorSize(std::string ss) : s(ss) {}
  ~ExceptionVectorSize() throw () {} // Updated
  const char* what() const throw() { 
    cerr << "Exception - unexpected vector length\n";
    return s.c_str(); 
  }
};



// Old stuff
// Functions from Model_*.cpp
/*
int Sampling_mRNA_Analytically(int runs_done, int num_runs, vector<double> &c_nom, vector<double> &n_nom, int arr_Analytical_mRNA[], gsl_rng*current_gen );

int getThetaEta(std::vector<double> &c_nom, std::vector<double> &n_nom, double &t0, double &tm, gsl_rng*current_gen, double n_nom_min[], double n_nom_max[]  );

int SamplingRound( int runs_done, int num_runs, std::vector<double> &c_nom, std::vector<double> &n_nom, int arr_X1[], int arr_X2[], gsl_rng*current_gen );
*/





