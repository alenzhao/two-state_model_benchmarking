#include "all_tests.h"

TestSuite *parse_nominals_tests();
TestSuite *extrinsic_noise_tests();
TestSuite *psampler_tests();
TestSuite *gillespie_tests();


int main(int argc, char **argv) {
  TestReporter *main_reporter = create_text_reporter();

  TestSuite *suite_parse_nominals = create_test_suite();
  TestSuite *suite_extrinsic_noise = create_test_suite();
  TestSuite *suite_psampler = create_test_suite();
  TestSuite *suite_gillespie = create_test_suite();

  add_suite(suite_parse_nominals, parse_nominals_tests());
  add_suite(suite_extrinsic_noise, extrinsic_noise_tests());
  add_suite(suite_psampler, psampler_tests());
  add_suite(suite_gillespie, gillespie_tests());
/*
  if (argc > 1) {
      return run_single_test(suite_parse_nominals, argv[1], create_text_reporter());
  }
*/
  run_test_suite(suite_parse_nominals, main_reporter);
  run_test_suite(suite_extrinsic_noise, main_reporter);
  run_test_suite(suite_psampler, main_reporter);
  run_test_suite(suite_gillespie, main_reporter);
  return 0;
}



