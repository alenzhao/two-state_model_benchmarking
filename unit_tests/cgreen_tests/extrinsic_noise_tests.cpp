#include "all_tests.h"

Describe(ExtrinsicNoise);
BeforeEach(ExtrinsicNoise) { die_in(10); }
AfterEach(ExtrinsicNoise) {}

////// TETS //////
// Check that rf2onoff ensures vector length 
Ensure(ExtrinsicNoise, rf2onoff_throws_if_input_vector_too_short) {
  static const double v_[] = {3.0, 5.0, 1.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  assert_throws(std::exception, rf2onoff(v1))
}
Ensure(ExtrinsicNoise, rf2onoff_throws_if_input_vector_too_long) {
  static const double v_[] = {3.0, 5.0, 1.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  assert_throws(std::exception, rf2onoff(v1))
}
// Test conversion using rf2onoff
Ensure(ExtrinsicNoise, rf2onoff_converts_correctly) {
  static const double v_[] = {0.18169, 0.064853, 1.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  std::vector<double> converted = rf2onoff(v1);
  assert_that_double(converted.at(0), is_equal_to_double(0.03962618078723222));
  assert_that_double(converted.at(1), is_equal_to_double(0.1784715724585833));
}
Ensure(ExtrinsicNoise, rf2onoff_handles_zeros) {
  static const double v_[] = {0.0, 0.064853, 1.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  assert_throws(std::exception, rf2onoff(v1))
}

// Check that get_duration ensures vector length 
Ensure(ExtrinsicNoise, get_duration_throws_if_input_vector_too_short) {
  static const double v_[] = {3.0, 5.0, 1.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  assert_throws(std::exception, get_duration(v1))
}
Ensure(ExtrinsicNoise, get_duration_throws_if_input_vector_too_long) {
  static const double v_[] = {3.0, 5.0, 1.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  assert_throws(std::exception, get_duration(v1))
}
// Test conversion using get_duration
Ensure(ExtrinsicNoise, get_duration_calculates_correctly) {
  static const double v_[] = {0.18169, 0.064853, 1.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  double duration = get_duration(v1);
  assert_that_double(duration, is_equal_to_double(1.5476190476190477));
}
Ensure(ExtrinsicNoise, get_duration_handles_zeros) {
  static const double v_[] = {0.18169, 0.064853, 1.0, 4.0, 5.0, 0.0, 0.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  assert_throws(std::exception, get_duration(v1))
}

// Check that perturb_nominals ensures vector length 
Ensure(ExtrinsicNoise, perturb_nominals_throws_if_input_vector_too_short) {
  gsl_rng * current_gen = gsl_rng_alloc(gsl_rng_mt19937); // func gsl_rng_alloc returns pointer to new instance of generator of type T  
  gsl_rng_set(current_gen, 1); // Seed the generator with value 'seed'
  static const double v_[] = {3.0, 5.0, 1.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  assert_throws(std::exception, perturb_nominals(v1, current_gen))
}
Ensure(ExtrinsicNoise, perturb_nominals_throws_if_input_vector_too_long) {
  gsl_rng * current_gen = gsl_rng_alloc(gsl_rng_mt19937); // func gsl_rng_alloc returns pointer to new instance of generator of type T  
  gsl_rng_set(current_gen, 1); // Seed the generator with value 'seed'
  static const double v_[] = {3.0, 5.0, 1.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  assert_throws(std::exception, perturb_nominals(v1, current_gen));
}
// Check that perturb_nominals does stuff
Ensure(ExtrinsicNoise, perturb_nominals_functions) {
  gsl_rng * current_gen = gsl_rng_alloc(gsl_rng_mt19937); // func gsl_rng_alloc returns pointer to new instance of generator of type T  
  gsl_rng_set(current_gen, 1); // Seed the generator with value 'seed'
  static const double v_[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  std::vector<double> perturbed = perturb_nominals(v1, current_gen);
  // print_vector_doubles(perturbed);
  assert_that(perturbed.size(), is_equal_to(7));
}

// Test extrinsic_noise_S2 
Ensure(ExtrinsicNoise, extrinsic_noise_S2_throws_if_input_vector_too_short) {
  gsl_rng * current_gen = gsl_rng_alloc(gsl_rng_mt19937); // func gsl_rng_alloc returns pointer to new instance of generator of type T  
  gsl_rng_set(current_gen, 1); // Seed the generator with value 'seed'

  static const double v_[] = {3.0, 5.0, 1.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  std::vector<double> means_mRNA, means_prot, vars_mRNA, vars_prot;

  assert_throws(std::exception, extrinsic_noise_S2(v1, 5, means_mRNA, means_prot, vars_mRNA, vars_prot, current_gen, S2_Gillespie))
}
Ensure(ExtrinsicNoise, extrinsic_noise_S2_throws_if_input_vector_too_long) {
  gsl_rng * current_gen = gsl_rng_alloc(gsl_rng_mt19937); // func gsl_rng_alloc returns pointer to new instance of generator of type T  
  gsl_rng_set(current_gen, 1); // Seed the generator with value 'seed'

  static const double v_[] = {3.0, 5.0, 1.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  std::vector<double> means_mRNA, means_prot, vars_mRNA, vars_prot;

  assert_throws(std::exception, extrinsic_noise_S2(v1, 5, means_mRNA, means_prot, vars_mRNA, vars_prot, current_gen, S2_Gillespie))
}

Ensure(ExtrinsicNoise, extrinsic_noise_S2_returns_value) {
  gsl_rng * current_gen = gsl_rng_alloc(gsl_rng_mt19937); // func gsl_rng_alloc returns pointer to new instance of generator of type T  
  gsl_rng_set(current_gen, 1); // Seed the generator with value 'seed'

  static const double v_[] = {0.18169, 0.064853, 1.0, 4.0, 5.0, 0.6, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  std::vector<double> means_mRNA, means_prot, vars_mRNA, vars_prot;

  cout << "Calling extrinsic_noise_S2" << endl;
  assert_that_double(0.0, is_less_than_double(
    extrinsic_noise_S2(v1, 5, means_mRNA, means_prot, vars_mRNA, vars_prot, current_gen, S2_Gillespie)
  ) );
  
}

// Tests perturb and simulate
Ensure(ExtrinsicNoise, extrinsic_noise_S2_generates_results_Psampler) {
  gsl_rng * current_gen = gsl_rng_alloc(gsl_rng_mt19937); // func gsl_rng_alloc returns pointer to new instance of generator of type T  
  gsl_rng_set(current_gen, 1); // Seed the generator with value 'seed'

  static const double v_[] = {0.18169, 0.064853, 1.0, 4.0, 5.0, 0.6, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  std::vector<double> means_mRNA, means_prot, vars_mRNA, vars_prot;
  // print_vector_doubles(means_mRNA);// print_vector_doubles(means_prot); print_vector_doubles(vars_mRNA); print_vector_doubles(vars_prot);
  extrinsic_noise_S2(v1, 5, means_mRNA, means_prot, vars_mRNA, vars_prot, current_gen, Psampler);
  // print_vector_doubles(means_mRNA);
  extrinsic_noise_S2(v1, 5, means_mRNA, means_prot, vars_mRNA, vars_prot, current_gen, Psampler);
  // print_vector_doubles(means_mRNA);
  extrinsic_noise_S2(v1, 5, means_mRNA, means_prot, vars_mRNA, vars_prot, current_gen, Psampler);
  cout << "after Psampler"<<endl;
  // print_vector_doubles(means_mRNA); print_vector_doubles(means_prot); print_vector_doubles(vars_mRNA); print_vector_doubles(vars_prot);

}

Ensure(ExtrinsicNoise, extrinsic_noise_S2_generates_results_Gillespie) {
  gsl_rng * current_gen = gsl_rng_alloc(gsl_rng_mt19937); // func gsl_rng_alloc returns pointer to new instance of generator of type T  
  gsl_rng_set(current_gen, 1); // Seed the generator with value 'seed'

  static const double v_[] = {0.18169, 0.064853, 1.0, 4.0, 5.0, 0.6, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  std::vector<double> v1 (v_, v_ + sizeof(v_) / sizeof(v_[0]) );
  std::vector<double> means_mRNA, means_prot, vars_mRNA, vars_prot;
  // print_vector_doubles(means_mRNA); //print_vector_doubles(means_prot); print_vector_doubles(vars_mRNA); print_vector_doubles(vars_prot);
  extrinsic_noise_S2(v1, 5, means_mRNA, means_prot, vars_mRNA, vars_prot, current_gen, S2_Gillespie);
  extrinsic_noise_S2(v1, 5, means_mRNA, means_prot, vars_mRNA, vars_prot, current_gen, S2_Gillespie);
  extrinsic_noise_S2(v1, 5, means_mRNA, means_prot, vars_mRNA, vars_prot, current_gen, S2_Gillespie);
  cout << "after S2_Gillespie"<<endl;
  // print_vector_doubles(means_mRNA); print_vector_doubles(means_prot); print_vector_doubles(vars_mRNA); print_vector_doubles(vars_prot);

}

//////////////////

// Function for creating test suite (this will be called from all_tests.cpp)
TestSuite *extrinsic_noise_tests() {
  TestSuite *suite = create_test_suite();

  // Check that rf2onoff ensures vector length 
  add_test_with_context(suite, ExtrinsicNoise, rf2onoff_throws_if_input_vector_too_short);
  add_test_with_context(suite, ExtrinsicNoise, rf2onoff_throws_if_input_vector_too_long);
  
  // Test conversion using rf2onoff
  add_test_with_context(suite, ExtrinsicNoise, rf2onoff_converts_correctly);
  add_test_with_context(suite, ExtrinsicNoise, rf2onoff_handles_zeros);

  // Check that get_duration ensures vector length 
  add_test_with_context(suite, ExtrinsicNoise, get_duration_throws_if_input_vector_too_short);
  add_test_with_context(suite, ExtrinsicNoise, get_duration_throws_if_input_vector_too_long);
  // Test duration calculation
  add_test_with_context(suite, ExtrinsicNoise, get_duration_calculates_correctly);
  add_test_with_context(suite, ExtrinsicNoise, get_duration_handles_zeros);

  // Check that perturb_nominals ensures vector length 
  add_test_with_context(suite, ExtrinsicNoise, perturb_nominals_throws_if_input_vector_too_short);
  add_test_with_context(suite, ExtrinsicNoise, perturb_nominals_throws_if_input_vector_too_long);
  // Check that perturb_nominals does stuff
  add_test_with_context(suite, ExtrinsicNoise, perturb_nominals_functions);

  // Test extrinsic_noise_S2 
  add_test_with_context(suite, ExtrinsicNoise, extrinsic_noise_S2_throws_if_input_vector_too_short);
  add_test_with_context(suite, ExtrinsicNoise, extrinsic_noise_S2_throws_if_input_vector_too_long);
  add_test_with_context(suite, ExtrinsicNoise, extrinsic_noise_S2_returns_value);
  add_test_with_context(suite, ExtrinsicNoise, extrinsic_noise_S2_generates_results_Psampler);
  add_test_with_context(suite, ExtrinsicNoise, extrinsic_noise_S2_generates_results_Gillespie);


  return suite;
}

// EOF
