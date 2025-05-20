#pragma once

#include <stdio.h>              // printing
#include <stdlib.h>             // malloc
#include <string.h>             // strcmp
#include <stdarg.h>             // vararg
#include <ctype.h>              // isspace
#include <unistd.h>             // fork/read/write
#include <fcntl.h>              // open
#include <sys/stat.h>           // stat
#include <sys/wait.h>           // wait
#include <limits.h>             // INT_MIN

#define MAX_TESTS    1024       // max number of tests in a suite
#define INIT_STRTAB  128        // initial length the suite string table
#define MAX_FILENAME 2048       // maximum length of a filename
#define MAX_ARGS     1024       // maximum length of an argv[] array for exec()
// #define MAX_STRTAB   4096       // max size of the string table

#define TEST_DIFFWIDTH 40       // width in characters to print around differing output 


#define TESTFAIL_INPUT  20      // input setup failed for a test
#define TESTFAIL_OUTPUT 21      // output setup failed for a test
#define TESTFAIL_EXEC   22      // exec failed for a test

typedef enum {                  // states of a test case stored in test->state field
  TEST_NOT_RUN = 0,             // test has not been run yet, not pid
  TEST_RUNNING = 1,             // test is running, pid is set
  TEST_PASSED  = 2,             // test completed and passed all checks
  TEST_FAILED  = 3,             // test completed but failed some checks
} test_state_t;

typedef struct {                // test case: program to run and output
  char *title;                  // title of the test
  char *description;            // description of the test
  char *program;                // program to run for the test
  char *input;                  // input for the test, NULL if no input
  char *infile_name;            // file which contains input for test, NULL if no input
  char *output_expect;          // expected output for the test, NULL if no expected output
  char *output_actual;          // actual output produced by the program
  char *outfile_name;           // name of file containing actual output
  char *resultfile_name;        // name of file containing results
  int exit_code_expect;         // expected exit code, defaults to 0
  int exit_code_actual;         // actual exit code, negative for signal numbers
  test_state_t state;           // state of the test like RUNNING or PASSED
  pid_t child_pid;              // pid of the child executing the test program OR 0 if state is NOT_RUN
} test_t;

// NOTE: String fields (type char *) for test_t and suite_t structs
// are all heap-allocated, primarily with strdup(str) calls. This
// means that when deallocating a test_t or suite_t, all string fields
// must be free()'d.

typedef struct {                // suite of tests to run including options
  FILE *infile;                 // file to use during reading; NULL otherwise
  char *infile_name;            // name of file from which the suite is read, pointer into strtab
  char *prefix;                 // prefix for test results, pointer into strtab
  char *testdir;                // name of directory for test results / output files, pointer into strtab
  test_t tests[MAX_TESTS];      // array of tests to run
  int tests_count;              // count of the tests in tests[] array
  int tests_torun[MAX_TESTS];   // indices of tests to run; defaults to all tests
  int tests_torun_count;        // length of the tests to run array
  int tests_passed;             // total tests that have passed
  int max_procs;                // maximum number of processes to run in parallel
} suite_t;

// chester_util.c
void Dprintf(const char* format, ...);
int split_into_argv(char *line, char *argv[], int *argc_ptr);
char *test_state_str(test_state_t state);
void suite_init(suite_t *suite);
int suite_init_from_file_peg(suite_t *suite, char *fname);
void suite_dealloc(suite_t *suite);
int suite_do_global_directive(suite_t *suite, const char *key, const char *val);
int suite_do_local_directive(suite_t *suite, const char *key, const char *val);

// chester_funcs.c
int suite_create_testdir(suite_t *suite);
int suite_test_set_outfile_name(suite_t *suite, int testnum);
int suite_test_create_infile(suite_t *suite, int testnum);
int suite_test_read_output_actual(suite_t *suite, int testnum);
int suite_test_start(suite_t *suite, int testnum);
int suite_test_finish(suite_t *suite, int testnum, int status);
void print_window(FILE *out, char *str, int center, int lrwidth);
int differing_index(char *strA, char *strB);
int suite_test_make_resultfile(suite_t *suite, int testnum);
int suite_run_tests_singleproc(suite_t *suite);
void suite_print_results_table(suite_t *suite);
int main(int argc, char *argv[]);
int suite_testnum_with_pid(suite_t *suite, pid_t pid);

// MAKEUP PROBLEMS
int suite_run_tests_multiproc(suite_t *suite);
int suite_init_from_file_manual(suite_t *suite, char *fname);
