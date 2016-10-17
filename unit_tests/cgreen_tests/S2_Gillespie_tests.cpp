#include "all_tests.h"

Describe(GillespieSuite);
BeforeEach(GillespieSuite) { die_in(10); }
AfterEach(GillespieSuite) {}


////// TETS //////
Ensure(GillespieSuite, psampler_throws_if_input_vector_too_short) {
  gsl_rng * current_gen = gsl_rng_alloc(gsl_rng_mt19937); // func gsl_rng_alloc returns pointer to new instance of generator of type T  
  gsl_rng_set(current_gen, 3); // Seed the generator with value 'seed'

  std::vector<int> f1;
  std::vector<int> f2;

  static const double v_[] = {3.0, 5.0, 1.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  assert_throws(std::exception, S2_Gillespie(500.0, v1, f1, f2, current_gen );)
}
Ensure(GillespieSuite, psampler_throws_if_input_vector_too_long) {
  gsl_rng * current_gen = gsl_rng_alloc(gsl_rng_mt19937); // func gsl_rng_alloc returns pointer to new instance of generator of type T  
  gsl_rng_set(current_gen, 3); // Seed the generator with value 'seed'

  std::vector<int> f1;
  std::vector<int> f2;

  static const double v_[] = {3.0, 5.0, 1.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  assert_throws(std::exception, S2_Gillespie(500.0, v1, f1, f2, current_gen );)
}

Ensure(GillespieSuite, pushes_to_finals_vectors) {
  gsl_rng * current_gen = gsl_rng_alloc(gsl_rng_mt19937); // func gsl_rng_alloc returns pointer to new instance of generator of type T  
  gsl_rng_set(current_gen, 3); // Seed the generator with value 'seed'

  static const double v_[] = {0.18169, 0.064853, 1.0, 4.0, 5.0, 0.6, 0.1};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  std::vector<int> f1;
  std::vector<int> f2;

  S2_Gillespie(500.0, v1, f1, f2, current_gen );
  assert_that(f1.size()==1);
  assert_that(f2.size()==1);

  S2_Gillespie(500.0, v1, f1, f2, current_gen );
  assert_that(f1.size()==2);
  assert_that(f2.size()==2);

  S2_Gillespie(500.0, v1, f1, f2, current_gen );
  assert_that(f1.size()==3);
  assert_that(f2.size()==3);

  S2_Gillespie(500.0, v1, f1, f2, current_gen );
  assert_that(f1.size()==4);
  assert_that(f2.size()==4);

  // Check for changes in output (Note that this is just to check for changes not to check the corectness of these numbers)
  int out1[] = { 11, 10, 6, 9};
  int out2[] = {407, 340, 375, 256};

  int arr1[4]; memcpy( arr1, &f1[0], sizeof( int ) * f1.size() );
  int arr2[4]; memcpy( arr2, &f2[0], sizeof( int ) * f2.size() );

  assert_that(arr1, is_equal_to_contents_of(out1, sizeof(out1)) );
  assert_that(arr2, is_equal_to_contents_of(out2, sizeof(out2)) );

  // cout << "GillespieSuite:" << endl;
  // print_vector_ints(f1);print_vector_ints(f2);
}

Ensure(GillespieSuite, produces_expected_means_and_variances) {
  // Initialise RNG
  gsl_rng * current_gen = gsl_rng_alloc(gsl_rng_mt19937); // func gsl_rng_alloc returns pointer to new instance of generator of type T  
  gsl_rng_set(current_gen, 3); // Seed the generator with value 'seed'

  // Reaction rate parameter vector
  static const double v_[] = {0.18169, 0.064853, 1.0, 4.0, 5.0, 0.6, 0.1};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );

  // Create vectors for storing output
  std::vector<int> counts_mRNA;
  std::vector<int> counts_prot;

  // Run simulations, store mRNA and protein numbers to vects.
  int numRuns = 100;
  for (int irun=0; irun < numRuns; irun++) {
    S2_Gillespie(500.0, v1, counts_mRNA, counts_prot, current_gen );
  }

  assert_that(counts_mRNA.size()==numRuns);
  assert_that(counts_prot.size()==numRuns);

  // Convert vectors to arrays and calculate means using GSL library function.
  double* arr_counts_mRNA = new double[numRuns];
  double* arr_counts_prot = new double[numRuns];
  for (int irun=0; irun < numRuns; irun++) {
    arr_counts_mRNA[irun] = counts_mRNA.at(irun);
    arr_counts_prot[irun] = counts_prot.at(irun);
  }
  double mean_mRNA, mean_prot, var_mRNA, var_prot;
  mean_mRNA = gsl_stats_mean(arr_counts_mRNA, 1, numRuns);
  mean_prot = gsl_stats_mean(arr_counts_prot, 1, numRuns);
  var_mRNA  = gsl_stats_variance(arr_counts_mRNA, 1, numRuns);
  var_prot  = gsl_stats_variance(arr_counts_prot, 1, numRuns); 

  // cout << "GillespieSuite - mean_mRNA = " << mean_mRNA << endl;
  // cout << "GillespieSuite - mean_prot = " << mean_prot << endl;
  // cout << "GillespieSuite - var_mRNA = " << var_mRNA << endl;
  // cout << "GillespieSuite - var_prot = " << var_prot << endl;

  significant_figures_for_assert_double_are(6);
  assert_that_double(mean_mRNA, is_equal_to_double(6.65));
  assert_that_double(mean_prot, is_equal_to_double(337.61));
  assert_that_double(var_mRNA, is_equal_to_double(6.229798));
  assert_that_double(var_prot, is_equal_to_double(2565.270606));
}

//////////////////

// Function for creating test suite (this will be called from all_tests.cpp)
TestSuite *gillespie_tests() {
  TestSuite *suite = create_test_suite();

  add_test_with_context(suite, GillespieSuite, psampler_throws_if_input_vector_too_short);
  add_test_with_context(suite, GillespieSuite, psampler_throws_if_input_vector_too_long);

  add_test_with_context(suite, GillespieSuite, pushes_to_finals_vectors);
  add_test_with_context(suite, GillespieSuite, produces_expected_means_and_variances);
  
  return suite;
}

// EOF
