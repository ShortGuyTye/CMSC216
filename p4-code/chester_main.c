
#include "chester.h"

////////////////////////////////////////////////////////////////////////////////
// PROBLEM 5 Functions
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){
    //setup suite
    char *infilename = argv[1];
    suite_t suite;
    int ret = suite_init_from_file_peg(&suite, infilename);
    if(ret == -1){
        printf("ERROR: problems encountered during test run");
        suite_dealloc(&suite);
        return -1;
    }
    //check if run all
    if (argc > 2){
        suite.tests_torun_count = argc - 2;
        for (int i = 0; i < argc - 2; i++){
            suite.tests_torun[i] = atoi(argv[i+2]);
        }
    //set all otherwise
    } else {
        suite.tests_torun_count = suite.tests_count;
        for (int i = 0; i < suite.tests_count; i++){
            suite.tests_torun[i] = i;
        }
    }
    //start tests
    printf("%s : running %d / %d tests\n", infilename, suite.tests_torun_count, suite.tests_count);
    int testret = suite_run_tests_singleproc(&suite);
    if (testret == -1){
        printf("ERROR: problems encountered during test run\n");
        suite_dealloc(&suite);
        return -1;
    }
    //print results
    suite_print_results_table(&suite);
    //find number passed
    int passed = 0;
    for (int i = 0; i < suite.tests_torun_count; i++){
        if (suite.tests[suite.tests_torun[i]].state == TEST_PASSED){
            passed++;
        }
    }
    //print and deallocate
    printf("Overall: %d / %d tests passed\n", passed, suite.tests_torun_count);
    suite_dealloc(&suite);

}

int suite_testnum_with_pid(suite_t *suite, pid_t pid){
    return 0;
}
// MAKEUP CREDIT: Finds the test that has child_pid equal to pid and
// returns that its index. If no test with the pid given is found,
// returns -1. This function is used during
// suite_run_tests_singleproc() to look up the test associated with a
// completed child process.

int suite_run_tests_multiproc(suite_t *suite){
    return 0;
}
// MAKEUP CREDIT: Like suite_run_tests_singleproc() but uses up to
// suite.max_procs concurrent processes to do so. This will speed up
// tests if multiple processors are available or if tests are bounded
// by I/O operations. The general algorithm to achieve concurrent
// execution is described in the project description and should be
// consulted carefully when implementing this function.
