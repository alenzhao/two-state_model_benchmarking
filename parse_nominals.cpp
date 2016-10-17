/*
 Parse nominal parameters from file
*/
#include "benchmark.h"
#include <fstream> // needed to read file
using namespace std;

// Exceptions
struct ExceptionMissingFile : public std::exception
{
  std::string s;
  ExceptionMissingFile(std::string ss) : s(ss) {}
  ~ExceptionMissingFile() throw () {} // Updated
  const char* what() const throw() { 
    cerr << "Error opening file: ";
    return s.c_str(); 
  }
};

// Read string of delimited numbers into a vector of doubles
vector<double> line2vec_double(string line, string delimiter) { // Read line as a string stream and extract values split by comma
  vector<double> vec_out;
  stringstream streamline(line);
  while( streamline.ios::good() ) {
    string substr;
    getline( streamline, substr, ',' );
    // cout << "substr: " << substr << endl; // string::size_type sz;     // alias of size_t
    // double val = stod(substr);
    // cout << "number: " << val << endl;
    vec_out.push_back( stod(substr) );
  }
  return(vec_out);
}

// Print each element of vector of doubles
void print_vector_ints(vector<int> vecp) {
  cout << "vect: ";
  for (vector<int>::const_iterator i = vecp.begin(); i != vecp.end(); ++i)
    cout << *i << ", ";
  cout << endl;
}

// Print each element of vector of doubles
void print_vector_doubles(vector<double> vecp) {
  cout << "vect: ";
  for (vector<double>::const_iterator i = vecp.begin(); i != vecp.end(); ++i)
    cout << *i << ", ";
  cout << endl;
}

// Print each element in vector of vectors of doubles
void print_vector_of_vectors_doubles(vector< vector<double> > vvd){
  cout << "vector of vectors: " << endl;
  for (vector< vector<double> >::const_iterator irow = vvd.begin(); irow != vvd.end(); ++irow){
    vector<double> vecp = *irow;
    for (vector<double>::const_iterator icol = vecp.begin(); icol != vecp.end(); ++icol){
      cout << *icol << ", ";
    }
    cout << endl;
  }
}

// Read file containing comma delimited matrix of nominal and noise parameters into a vector of vectors
vector< vector<double> > parse_nominals(string fpath) {
  string line, param;
  vector< vector<double> > nominals_noise; // Vector holding nominal parameters of input population
  vector<double> vec_params;
  // Create instance of ifstream class
  ifstream inFile;
  inFile.open(fpath);
  // Check for Error
  if (inFile.fail()) {
    // cerr << "Error opening file: " << fpath << endl;
    throw ExceptionMissingFile(fpath);
  }
  cout << "Reading nominal parameters from file: " << fpath << endl;
  while (getline(inFile,line)) {
    // cout << "line: " << line << endl;
    vec_params = line2vec_double(line, ","); // print_vector_doubles(vec_params);
    nominals_noise.push_back(vec_params);
  }
  // cout << "In parse_nominals return this: " << endl; print_vector_of_vectors_doubles(nominals_noise);
  return(nominals_noise);
}

// Print each element of vector of doubles
void write_vector_doubles_tofile(vector<double> vecp, std::ofstream &file) {
  for (vector<double>::const_iterator i = vecp.begin(); i != vecp.end(); ++i)
    file << *i << ", ";
}

// EOF
