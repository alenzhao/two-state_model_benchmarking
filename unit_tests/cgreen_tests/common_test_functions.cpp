#include "all_tests.h"

////// FUNCTIONS //////
// Convert array to vector and append the result to a vector of vectors.
std::vector< vector<double> > push_vector(std::vector< vector<double> > vv, double array[], int size) {
  std::vector<double> vec (array, array + size );
  vv.push_back(vec);
  return(vv);
}

// EOF
