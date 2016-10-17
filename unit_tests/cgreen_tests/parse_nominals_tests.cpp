// Create tests for the functions in parse_nominals.cpp.
#include "all_tests.h"

Describe(ParseNominals);
BeforeEach(ParseNominals) { die_in(10); }
AfterEach(ParseNominals) {}

//// Basic tests to make sure comparisons of vectors work
Ensure(ParseNominals, vectors_equal) {
  static const double v_[] = {3.0,5.0,1.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  std::vector<double> v2 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  assert_that(v1 == v2);
  assert_that(v1.capacity() == v2.capacity());
}

Ensure(ParseNominals, vectors_not_equal) {
  static const double v_1[] = {3.0,5.0,1.0};
  static const double v_2[] = {3.0,5.0,1.0,4.0};
  std::vector<double> v1 (v_1, v_1 + sizeof(v_1) / sizeof(v_1[0]) );
  std::vector<double> v2 (v_2, v_2 + sizeof(v_2) / sizeof(v_2[0]) );
  assert_that(v1 != v2);
  assert_that(v1.capacity() != v2.capacity());
}

Ensure(ParseNominals, vectors_of_vectors_equal) {
  static const double v_1[] = {3.0,5.0,1.0};
  static const double v_2[] = {3.0,5.0,1.0};
  std::vector<double> v1 (v_1, v_1 + sizeof(v_1) / sizeof(v_1[0]) );
  std::vector<double> v2 (v_2, v_2 + sizeof(v_2) / sizeof(v_2[0]) );
  std::vector< vector<double> > v12, v21;
  v12.push_back(v1); v12.push_back(v2);
  v21.push_back(v2); v21.push_back(v1);
  assert_that(v12 == v21);
  assert_that(v12.capacity() == v21.capacity());
}

Ensure(ParseNominals, vectors_of_vectors_not_equal) {
  static const double v_1[] = {3.0,5.0,1.0};
  static const double v_2[] = {3.0,5.0,1.0,0.0};
  std::vector<double> v1 (v_1, v_1 + sizeof(v_1) / sizeof(v_1[0]) );
  std::vector<double> v2 (v_2, v_2 + sizeof(v_2) / sizeof(v_2[0]) );
  std::vector< vector<double> > v12, v21;
  v12.push_back(v1); v12.push_back(v2);
  v21.push_back(v2); v21.push_back(v1);
  assert_that(v12 != v21);
}

//// Test functions from parse_nominals.cpp
// vector<double> line2vec_double(string line, string delimiter)
Ensure(ParseNominals, line2vec_double_converts_string_to_vector_test1) {
  std::vector<double> s2v;
  static const double v_1[] = {3.0,5.0,1.0};
  std::vector<double> v1 (v_1, v_1 + sizeof(v_1) / sizeof(v_1[0]) ); // print_vector_doubles(v1);
  string s1 = "   3.00, 5, 1.0 "; // cout << "s1  = " << s1 << endl;
  s2v = line2vec_double(s1, ","); // print_vector_doubles(s2v);
  assert_that(s2v == v1); // print_vector_doubles(v1);
}
Ensure(ParseNominals, line2vec_double_converts_string_to_vector_test2) {
  std::vector<double> s2v;
  static const double v_1[] = {3.0,5.0,1.0};
  std::vector<double> v1 (v_1, v_1 + sizeof(v_1) / sizeof(v_1[0]) ); // print_vector_doubles(v1);
  string s1 = "3.00, 5, 1.0 "; // cout << "s1  = " << s1 << endl;
  s2v = line2vec_double(s1, ","); // print_vector_doubles(s2v);
  assert_that(s2v == v1); // print_vector_doubles(v1);
}
// Consider case of trailing comma (e.g. s1 = "   3.00, 5, 1.0, ";)
// Consider case of non-numeric and non-delimiter characters in string

Ensure(ParseNominals, parse_nominals_reads_from_file) { // Check that doubles read from file match what is expected
  std::vector< vector<double> > vv, noms;
  double arr1[] = {1.01, 1.02, 1.03, 1.04, 1.05, 1.06, 1.07, 1.08, 1.09, 1.10, 1.11, 1.12, 1.13, 1.14, 1.15, 1.16, 1.17, 1.18, 1.19};
  double arr2[] = {2.01, 2.02, 2.03, 2.04, 2.05, 2.06, 2.07, 2.08, 2.09, 2.10, 2.11, 2.12, 2.13, 2.14, 2.15, 2.16, 2.17, 2.18, 2.19};
  double arr3[] = {3.01, 3.02, 3.03, 3.04, 3.05, 3.06, 3.07, 3.08, 3.09, 3.10, 3.11, 3.12, 3.13, 3.14, 3.15, 3.16, 3.17, 3.18, 3.19};
  double arr4[] = {4.01, 4.02, 4.03, 4.04, 4.05, 4.06, 4.07, 4.08, 4.09, 4.10, 4.11, 4.12, 4.13, 4.14, 4.15, 4.16, 4.17, 4.18, 4.19};
  double arr5[] = {5.01, 5.02, 5.03, 5.04, 5.05, 5.06, 5.07, 5.08, 5.09, 5.10, 5.11, 5.12, 5.13, 5.14, 5.15, 5.16, 5.17, 5.18, 5.19};
  double arr6[] = {6.01, 6.02, 6.03, 6.04, 6.05, 6.06, 6.07, 6.08, 6.09, 6.10, 6.11, 6.12, 6.13, 6.14, 6.15, 6.16, 6.17, 6.18, 6.19};
  double arr7[] = {7.01, 7.02, 7.03, 7.04, 7.05, 7.06, 7.07, 7.08, 7.09, 7.10, 7.11, 7.12, 7.13, 7.14, 7.15, 7.16, 7.17, 7.18, 7.19};
  vv = push_vector(vv, arr1, 19);
  vv = push_vector(vv, arr2, 19);
  vv = push_vector(vv, arr3, 19);
  vv = push_vector(vv, arr4, 19);
  vv = push_vector(vv, arr5, 19);
  vv = push_vector(vv, arr6, 19);
  vv = push_vector(vv, arr7, 19);
  // cout << "HERE:" << endl; print_vector_of_vectors_doubles(vv);
  noms = parse_nominals("../data/PopLast.txt");
  // cout << "NOMS:" << endl; print_vector_of_vectors_doubles(noms);
  assert_that(vv == noms);
}

Ensure(ParseNominals, parse_nominals_reads_from_file_noSpaces) { // Same as above but input file has no spaces after comma separators
  std::vector< vector<double> > vv, noms;
  double arr1[] = {1.01, 1.02, 1.03, 1.04, 1.05, 1.06, 1.07, 1.08, 1.09, 1.10, 1.11, 1.12, 1.13, 1.14, 1.15, 1.16, 1.17, 1.18, 1.19};
  double arr2[] = {2.01, 2.02, 2.03, 2.04, 2.05, 2.06, 2.07, 2.08, 2.09, 2.10, 2.11, 2.12, 2.13, 2.14, 2.15, 2.16, 2.17, 2.18, 2.19};
  double arr3[] = {3.01, 3.02, 3.03, 3.04, 3.05, 3.06, 3.07, 3.08, 3.09, 3.10, 3.11, 3.12, 3.13, 3.14, 3.15, 3.16, 3.17, 3.18, 3.19};
  double arr4[] = {4.01, 4.02, 4.03, 4.04, 4.05, 4.06, 4.07, 4.08, 4.09, 4.10, 4.11, 4.12, 4.13, 4.14, 4.15, 4.16, 4.17, 4.18, 4.19};
  double arr5[] = {5.01, 5.02, 5.03, 5.04, 5.05, 5.06, 5.07, 5.08, 5.09, 5.10, 5.11, 5.12, 5.13, 5.14, 5.15, 5.16, 5.17, 5.18, 5.19};
  double arr6[] = {6.01, 6.02, 6.03, 6.04, 6.05, 6.06, 6.07, 6.08, 6.09, 6.10, 6.11, 6.12, 6.13, 6.14, 6.15, 6.16, 6.17, 6.18, 6.19};
  double arr7[] = {7.01, 7.02, 7.03, 7.04, 7.05, 7.06, 7.07, 7.08, 7.09, 7.10, 7.11, 7.12, 7.13, 7.14, 7.15, 7.16, 7.17, 7.18, 7.19};
  vv = push_vector(vv, arr1, 19);
  vv = push_vector(vv, arr2, 19);
  vv = push_vector(vv, arr3, 19);
  vv = push_vector(vv, arr4, 19);
  vv = push_vector(vv, arr5, 19);
  vv = push_vector(vv, arr6, 19);
  vv = push_vector(vv, arr7, 19);
  // cout << "HERE:" << endl; print_vector_of_vectors_doubles(vv);
  noms = parse_nominals("../data/PopLast_noSpaces.txt");
  // cout << "NOMS:" << endl; print_vector_of_vectors_doubles(noms);
  assert_that(vv == noms);
}

Ensure(ParseNominals, parse_nominals_reads_from_missing_file) {
  std::vector< vector<double> > vv, noms;
  double arr1[] = {1.01, 1.02, 1.03, 1.04, 1.05, 1.06, 1.07, 1.08, 1.09, 1.10, 1.11, 1.12, 1.13, 1.14, 1.15, 1.16, 1.17, 1.18, 1.19};
  vv = push_vector(vv, arr1, 19);
  // noms = parse_nominals("../data/PopLast.txt.missing");
  assert_throws(std::exception, noms = parse_nominals("../data/PopLast.txt.missing");)
}




////////////////////////////////////////////////////////

// Function for creating test suite (this will be called from all_tests.cpp)
TestSuite *parse_nominals_tests() {
  TestSuite *suite = create_test_suite();
  //// Basic tests to make sure comparisons of vectors work
  add_test_with_context(suite, ParseNominals, vectors_equal);
  add_test_with_context(suite, ParseNominals, vectors_not_equal);
  add_test_with_context(suite, ParseNominals, vectors_of_vectors_equal);
  add_test_with_context(suite, ParseNominals, vectors_of_vectors_not_equal);
  //// Test functions from parse_nominals.cpp
  add_test_with_context(suite, ParseNominals, line2vec_double_converts_string_to_vector_test1);
  add_test_with_context(suite, ParseNominals, line2vec_double_converts_string_to_vector_test2);
  add_test_with_context(suite, ParseNominals, parse_nominals_reads_from_file);
  add_test_with_context(suite, ParseNominals, parse_nominals_reads_from_file_noSpaces);
  add_test_with_context(suite, ParseNominals, parse_nominals_reads_from_missing_file);
  return suite;
}

// EOF


