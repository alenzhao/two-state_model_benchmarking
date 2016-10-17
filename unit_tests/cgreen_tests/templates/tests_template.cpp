#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

using namespace cgreen;

Describe(Cgreen);
BeforeEach(Cgreen) {}
AfterEach(Cgreen) {}

Ensure(Cgreen, passes_this_test) {
    assert_that(1 == 1);
}

Ensure(Cgreen, fails_this_test) {
    assert_that(0 == 1);
}

TestSuite *example_cgreen_tests() {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, Cgreen, passes_this_test);
    add_test_with_context(suite, Cgreen, fails_this_test);
    return suite;
}

// int main(int argc, char **argv) {
//     TestSuite *suite = create_test_suite();
//     add_test_with_context(suite, Cgreen, passes_this_test);
//     add_test_with_context(suite, Cgreen, fails_this_test);
//     return run_test_suite(suite, create_text_reporter());
// }

/*
////// TETS //////
Describe(ParseNominals);
BeforeEach(ParseNominals) { die_in(10); }
AfterEach(ParseNominals) {}



TestSuite *parse_nominals_tests() {
  TestSuite *suite = create_test_suite();
  add_test_with_context(suite, ParseNominals, vectors_equal);

  return suite;
}

// EOF
*/