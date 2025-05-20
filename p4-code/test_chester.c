// test_chester.c: unit tests for chester functions and data
// UPDATED: Thu Nov 14 05:20:52 PM EST 2024
#include "chester.h"

// macro to set up a test with given name, print the source of the
// test; very hacky, fragile, but useful
#define IF_TEST(TNAME) \
  if( RUNALL ) { printf("\n"); } \
  if( RUNALL || strcmp( TNAME, test_name)==0 ) { \
    sprintf(sysbuf,"awk 'NR==(%d){P=1;gsub(\"^ *\",\"\");} P==1 && /ENDTEST/{P=0; print \"}\\n---OUTPUT---\"} P==1{print}' %s", __LINE__, __FILE__); \
    system(sysbuf); nrun++;  \
  } \
  if( RUNALL || strcmp( TNAME, test_name)==0 )

char sysbuf[1024];
int RUNALL = 0;
int nrun = 0;
char cmdbuf[1024];

void suite_test_print1(suite_t *suite, int testnum){
  test_t *test = &suite->tests[testnum];
  printf("test->program: %s\n", test->program);
  printf("test->input:\n---\n%s---\n", test->input);
  printf("test->output_actual:\n---\n%s---\n", test->output_actual);
  printf("test->output_expect:\n---\n%s---\n", test->output_expect);
  printf("test->exit_code_expect: %d\n", test->exit_code_expect);
  printf("test->exit_code_actual: %d\n", test->exit_code_actual);
  printf("test->state: %s (%d)\n", test_state_str(test->state),test->state);
  printf("suite->tests_passed: %d\n",suite->tests_passed);
}

void suite_test_setup(suite_t *suite, char *filename, char *key){
  suite_init_from_file_peg(suite,filename);
  free(suite->testdir);
  free(suite->prefix);
  sprintf(cmdbuf,"test-results/testdir%s",key);
  suite->testdir = strdup(cmdbuf);
  sprintf(cmdbuf,"pref%s",key);
  suite->prefix = strdup(cmdbuf);
  sprintf(cmdbuf,"rm -rf %s",suite->testdir);
  system(cmdbuf);
}

void suite_test_print_all(suite_t *suite){
  printf("\nCOMPLETED SUITE\n");
  printf("suite->infile_name: %s\n",suite->infile_name);
  printf("suite->tests_torun_count: %d\n",suite->tests_torun_count);
  printf("suite->tests_passed:      %d\n",suite->tests_passed);
  printf("\n");
  printf("CONTENTS OF TESTDIR\n");
  sprintf(cmdbuf,"ls %s",suite->testdir);
  system(cmdbuf);
  printf("\n");
  printf("TEST RESULTS\n");
  for(int i=0; i<suite->tests_torun_count; i++){
    int testnum = suite->tests_torun[i];
    test_t *test = &suite->tests[testnum];
    printf("tests[%d]->resultfile_name:\n%s\n",
           testnum,test->resultfile_name);
    printf("---Resultfile Contents---\n");
    sprintf(cmdbuf,"cat %s",test->resultfile_name);
    system(cmdbuf);
    printf("------\n\n");
  }      
}  

  
int main(int argc, char *argv[]){
  if(argc < 2){
    printf("usage: %s <test_name>\n", argv[0]);
    printf("       %s ALL\n", argv[0]);
    return 1;
  }
  char *test_name = argv[1];
  char sysbuf[1024];
  int ret, status;

  RUNALL = strcmp(test_name,"ALL")==0; // possibly run all tests

  system("mkdir -p test-results"); // ensure a subdirectory for data is present
    
  ////////////////////////////////////////////////////////////////////////////////
  // PROBLEM 1 TESTS
  ////////////////////////////////////////////////////////////////////////////////

  IF_TEST("//////////////////// PROBLEM 1 ////////////////////") {
  } // ENDTEST
  IF_TEST("suite_create_testdir1") { 
    // Check that testing directories are created properly. Command
    // `ls -Fd` will list a file and append a / if it is a directory.
    system("rm -rf test-results/testdir1");
    suite_t suite = {.testdir = "test-results/testdir1"};
    ret = suite_create_testdir(&suite);
    printf("ret: %d\n",ret);
    system("ls -Fd test-results/testdir1");
  } // ENDTEST

  IF_TEST("suite_create_testdir2") { 
    // Checks cases associated with successful new directory creation
    // and detecting a pre-existing directory and not trying to
    // re-create it.
    system("rm -rf test-results/testdir2");
    printf("SUCCESSFUL DIRECTORY CREATION\n");
    suite_t suite = {.testdir = "test-results/testdir2"};
    ret = suite_create_testdir(&suite);
    printf("ret: %d\n",ret);
    system("ls -Fd test-results/testdir2");

    printf("DETECTION OF EXISTING DIRECTORY\n");
    ret = suite_create_testdir(&suite);
    printf("ret: %d\n",ret);
    system("ls -Fd test-results/testdir2");
  } // ENDTEST

  IF_TEST("suite_create_testdir3") { 
    // Checks for test directory failure due to an existing regular
    // file that should not be overwritten.
    printf("FAILURE DUE TO EXISTING REGUALR FILE\n");
    system("rm -rf test-results/regular_file.txt");
    system("echo Dont tread on me > test-results/regular_file.txt");
    suite_t suite = {.testdir = "test-results/regular_file.txt"};
    ret = suite_create_testdir(&suite);
    printf("ret: %d\n",ret);
    system("ls -Fd test-results/regular_file.txt");
  } // ENDTEST

  IF_TEST("suite_test_set_outfile_name1") { 
    // Checks that the outfile_name is correctly set. Format of the
    // outfile name must match and the string associated with the
    // field must be heap-allocated and therefore free()-able.
    suite_t suite = {
      .testdir = "test-results/testdirX",
      .prefix = "testprob1",
      .tests_count = 4,
    };
    ret = suite_test_set_outfile_name(&suite, 0);
    printf("ret: %d\n",ret);
    printf("suite.tests[0].outfile_name: %s\n",suite.tests[0].outfile_name);
    free(suite.tests[0].outfile_name);
  } // ENDTEST

  IF_TEST("suite_test_set_outfile_name2") { 
    // Checks that the outfile_name is correctly set. Format of the
    // outfile name must match and the string associated with the
    // field must be heap-allocated and therefore free()-able.
    suite_t suite = {
      .testdir = "test-results/testdirY",
      .prefix = "another_prefix",
      .tests_count = 30,
    };
    ret = suite_test_set_outfile_name(&suite, 21);
    printf("ret: %d\n",ret);
    printf("suite.tests[21].outfile_name: %s\n",suite.tests[21].outfile_name);
    free(suite.tests[21].outfile_name);
  } // ENDTEST

  IF_TEST("suite_test_create_infile1") { 
    // Checks that when a test `input` field is non-null, the
    // `infile_name` is set correctly to reflect the testdir and
    // prefix of the suite. Also checks that the file is created and
    // the contents of `input` are placed within it.
    system("mkdir -p test-results/testdirZ");
    suite_t suite = {
      .testdir = "test-results/testdirZ",
      .prefix = "prefZ",
      .tests_count = 5,
    };
    suite.tests[3].input = "This is the\ninput for the test\n";
    ret = suite_test_create_infile(&suite, 3);
    printf("ret: %d\n",ret);
    printf("suite.tests[3].infile_name: %s\n",suite.tests[3].infile_name);
    printf("CONTENTS OF test-results/testdirZ/prefZ-input-03.txt\n"); 
    system("cat test-results/testdirZ/prefZ-input-03.txt");
    free(suite.tests[3].infile_name);
  } // ENDTEST

  IF_TEST("suite_test_create_infile2") { 
    // Checks for repeated creation of input files overwrites existing
    // ones using options to the open() system call.  Also checks that
    // on failures to create the input file, an appropriate error
    // message is produced using the perror() function which will
    // report why system calls like open() fail.
    system("mkdir -p test-results/testdirW");
    system("rm -rf test-results/testdirW/prefW-input-12.txt");
    suite_t suite = {
      .testdir = "test-results/testdirW",
      .prefix = "prefW",
      .tests_count = 18,
    };
    printf("\nSUCCESS INITIAL CREATION CASE\n");
    suite.tests[12].input = "More input\nfor the test\nthis time\n";
    ret = suite_test_create_infile(&suite, 12);
    printf("ret: %d\n",ret);
    printf("suite.tests[12].infile_name: %s\n",suite.tests[12].infile_name);
    printf("CONTENTS OF test-results/testdirW/prefW-input-12.txt\n"); 
    system("cat test-results/testdirW/prefW-input-12.txt");
    free(suite.tests[12].infile_name);
    suite.infile_name = NULL;

    printf("\nSUCCESS OVERWRITE CASE\n");
    suite.tests[12].input = "Different input\nThis time around\n";
    ret = suite_test_create_infile(&suite, 12);
    printf("ret: %d\n",ret);
    printf("suite.tests[12].infile_name: %s\n",suite.tests[12].infile_name);
    printf("CONTENTS OF test-results/testdirW/prefW-input-12.txt\n"); 
    system("cat test-results/testdirW/prefW-input-12.txt");
    free(suite.tests[12].infile_name);
    suite.infile_name = NULL;
  
    printf("\nFAIL CREATE DUE EXISTING DIRECTORY\n");
    system("rm -rf test-results/testdirW/prefW-input-12.txt");
    system("mkdir test-results/testdirW/prefW-input-12.txt");
    suite.tests[12].input = "Access denied!!";
    ret = suite_test_create_infile(&suite, 12);
    printf("ret: %d\n",ret);
    printf("suite.tests[12].infile_name: %s\n",suite.tests[12].infile_name);
    printf("CONTENTS OF test-results/testdirW/prefW-input-12.txt\n"); 
    system("cat test-results/testdirW/prefW-input-12.txt");
    free(suite.tests[12].infile_name);
    suite.infile_name = NULL;

    printf("\nFAIL OVERWRITE DUE TO PERMISSIONS\n");
    suite.testdir = "/etc";     // root directory access forbidden
    suite.tests[12].input = "Access denied?!?!";
    ret = suite_test_create_infile(&suite, 12);
    printf("ret: %d\n",ret);
    printf("suite.tests[12].infile_name: %s\n",suite.tests[12].infile_name);
    printf("CONTENTS OF /etc/prefW-input-12.txt\n"); 
    system("cat /etc/prefW-input-12.txt");
    free(suite.tests[12].infile_name);
    suite.infile_name = NULL;

  } // ENDTEST

  // This case does not work on GRACE's asinine AFS as the chmod
  // command does not set permissions which are honored
    // printf("\nFAIL OVERWRITE DUE TO PERMISSIONS\n");
    // system("chmod u-w test-results/testdirW/prefW-input-12.txt");
    // suite.tests[12].input = "Access denied!!";
    // ret = suite_test_create_infile(&suite, 12);
    // printf("ret: %d\n",ret);
    // printf("suite.tests[12].infile_name: %s\n",suite.tests[12].infile_name);
    // printf("CONTENTS OF test-results/testdirW/prefW-input-12.txt\n"); 
    // system("cat test-results/testdirW/prefW-input-12.txt");
    // free(suite.tests[12].infile_name);
    // suite.infile_name = NULL;

  IF_TEST("suite_test_read_output_actual1") { 
    system("mkdir -p test-results/testdirA");
    suite_t suite = {
      .testdir = "test-results/testdirA",
      .prefix = "prefA",
      .tests_count = 10,
    };
    system("seq 15 > test-results/testdirA/prefA-output-08.txt");
    suite_test_set_outfile_name(&suite, 8);
    ret = suite_test_read_output_actual(&suite, 8);
    printf("ret: %d\n",ret);
    printf("suite.tests[8].output_actual:\n---\n%s---\n",suite.tests[8].output_actual);
    free(suite.tests[8].outfile_name);
    free(suite.tests[8].output_actual);
  } // ENDTEST

  IF_TEST("suite_test_read_output_actual2") { 
    // Checks that larger more complex files are correctly read during
    // input accumulation.
    system("mkdir -p test-results/testdirB");
    suite_t suite = {
      .testdir = "test-results/testdirB",
      .prefix = "prefB",
      .tests_count = 20,
    };  
    printf("FIRST CASE OF MULTILINE TEXT\n");
    system("rm -rf file.txt");
    system("echo Mauris ac felis vel velit tristique     >> file.txt");
    system("echo imperdiet.  Nullam eu ante vel est      >> file.txt");
    system("echo convallis dignissim.  Fusce suscipit,   >> file.txt");
    system("echo wisi nec facilisis facilisis, est dui   >> file.txt");
    system("echo fermentum leo, quis tempor ligula erat  >> file.txt");
    system("echo quis odio.  Nunc porta vulputate tellus.>> file.txt");
    system("echo Nunc rutrum turpis sed pede.  Sed       >> file.txt");
    system("echo bibendum.  Aliquam posuere.  Nunc       >> file.txt");
    system("echo aliquet, augue nec adipiscing interdum, >> file.txt");
    system("echo lacus tellus malesuada massa, quis      >> file.txt");
    system("echo varius mi purus non odio.               >> file.txt");
    system("mv file.txt test-results/testdirB/prefB-output-18.txt");
    suite_test_set_outfile_name(&suite, 18);
    ret = suite_test_read_output_actual(&suite, 18);
    printf("ret: %d\n",ret);
    printf("suite.tests[18].output_actual:\n---\n%s---\n",suite.tests[18].output_actual);
    free(suite.tests[18].outfile_name);
    free(suite.tests[18].output_actual);

    printf("\nSECOND CASE OF LARGE NUMERIC VALUES\n");
    system("rm -rf file.txt");
    system("seq -w 500400300200100000 500400300200102500 > file.txt");
    system("mv file.txt test-results/testdirB/prefB-output-12.txt");
    suite_test_set_outfile_name(&suite, 12);
    ret = suite_test_read_output_actual(&suite, 12);
    printf("ret: %d\n",ret);
    // printf("suite.tests[12].output_actual:\n---\n%s---\n",suite.tests[12].output_actual);
    // 
    // printing the entire string is a LOT to check, so print a few
    // sample locations, 19 chars per line, print sprinkling of lines.
    char *out = suite.tests[12].output_actual;
    int len = strlen(out);
    for(int i=0; i < len; i+=19*25){
      printf("[%5d]: %.19s",i,out+i);
    }
    printf("END:\n%s",out+len-19*2);

    free(suite.tests[12].outfile_name);
    free(suite.tests[12].output_actual);
  } // ENDTEST

  IF_TEST("suite_test_read_output_actual3") { 
    // Checks that edge cases like absence of outfile_name or
    // inability to read it are reported via error mesages.
    system("mkdir -p test-results/testdirC");
    suite_t suite = {
      .testdir = "test-results/testdirC",
      .prefix = "prefC",
      .tests_count = 50,
    };

    printf("\nEXISTING BUT EMPTY OUTPUT FILE\n");
    suite_test_set_outfile_name(&suite, 32);
    system("touch test-results/testdirC/prefC-output-32.txt");
    ret = suite_test_read_output_actual(&suite, 32);
    printf("ret: %d\n",ret);
    printf("suite.tests[32].output_actual:\n---\n%s---\n",suite.tests[32].output_actual);
    free(suite.tests[32].outfile_name);
    free(suite.tests[32].output_actual);

    printf("\nMISSING OUTPUT FILE\n");
    suite_test_set_outfile_name(&suite, 36);
    ret = suite_test_read_output_actual(&suite, 36);
    printf("ret: %d\n",ret);
    printf("suite.tests[36].output_actual:\n---\n%s---\n",suite.tests[36].output_actual);
    free(suite.tests[36].outfile_name);
    /// free(suite.tests[36].output_actual);  // should still be NULL

    printf("\nOUTPUT FILE CANNOT BE OPENED FOR READING\n");
    suite_test_set_outfile_name(&suite, 45);
    system("touch test-results/testdirC/prefC-output-35.txt");
    system("chmod ugo-r test-results/testdirC/prefC-output-35.txt");
    ret = suite_test_read_output_actual(&suite, 45);
    printf("ret: %d\n",ret);
    printf("suite.tests[45].output_actual:\n---\n%s---\n",suite.tests[45].output_actual);
    free(suite.tests[45].outfile_name);
    /// free(suite.tests[45].output_actual);  // should still be NULL
  } // ENDTEST

  IF_TEST("//////////////////// PROBLEM 2 ////////////////////") {
  } // ENDTEST

  ////////////////////////////////////////////////////////////////////////////////
  // PROBLEM 2 TESTS
  ////////////////////////////////////////////////////////////////////////////////

  IF_TEST("suite_test_start1") {
    // Checks whether starting a test sets the outfile_name, creates a
    // child process to run the tested program. The program has no
    // output so output redirection is not checked.
    suite_t suite = {
      .testdir = strdup("test-results/testdirS1"),
      .prefix = strdup("prefS1"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[7];
    test->program = strdup("true");
    ret = suite_test_start(&suite, 7);
    test_state_t state = test->state;
    printf("test state: %s (%d)\n",test_state_str(state),state);
    if(ret != 0){ printf("Non-zero wait() return\n"); }
    pid_t child_pid = test->child_pid;
    printf("child_pid is %s\n",
           child_pid>0 ? "positive":"ZERO or NEGATIVE");
    ret = waitpid(child_pid, &status, 0);
    printf("child process %s\n",
           ret==child_pid ? "completed":"DID NOT complete");
    printf("exit_code: %d\n",WEXITSTATUS(status));
    suite_dealloc(&suite);
  } // ENDTEST

  IF_TEST("suite_test_start2") { 
    // Checks whether starting a test sets the outfile_name, creates a
    // child process to run the tested program, and that program
    // output is redirected into outfile_name. The program has a
    // command line parameter so as the child is about exec() a
    // function like split_into_argv() must be used to split up the
    // program line into an argv[] array.
    suite_t suite = {
      .testdir = strdup("test-results/testdirS2"),
      .prefix = strdup("prefS2"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[5];
    test->program = strdup("seq 7");
    ret = suite_test_start(&suite, 5);
    test_state_t state = test->state;
    printf("test state: %s (%d)\n",test_state_str(state),state);
    if(ret != 0){ printf("Non-zero wait() return\n"); }
    pid_t child_pid = test->child_pid;
    printf("child_pid is %s\n",
           child_pid>0 ? "positive":"ZERO or NEGATIVE");
    ret = waitpid(child_pid, &status, 0);
    printf("child process %s\n",
           ret==child_pid ? "completed":"DID NOT complete");
    printf("exit_code: %d\n",WEXITSTATUS(status));
    printf("PROGRAM OUTPUT\n");
    system("cat test-results/testdirS2/prefS2-output-05.txt");
    suite_dealloc(&suite);
  } // ENDTEST

  IF_TEST("suite_test_start3") { 
    // Checks that both stdout and stderr streams are redirected when
    // starting the child process. The test program prints messages to
    // both stdout and stderr but they should appear in the designated
    // output file as both streams are redirected into outfile_name.
    suite_t suite = {
      .testdir = strdup("test-results/testdirS3"),
      .prefix = strdup("prefS3"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[2];
    test->program = strdup("bash data/print_stdout_stderr.sh");
    ret = suite_test_start(&suite, 2);
    test_state_t state = test->state;
    printf("test state: %s (%d)\n",test_state_str(state),state);
    if(ret != 0){ printf("Non-zero wait() return\n"); }
    pid_t child_pid = test->child_pid;
    printf("child_pid is %s\n",
           child_pid>0 ? "positive":"ZERO or NEGATIVE");
    ret = waitpid(child_pid, &status, 0);
    printf("child process %s\n",
           ret==child_pid ? "completed":"DID NOT complete");
    printf("exit_code: %d\n",WEXITSTATUS(status));
    printf("PROGRAM OUTPUT\n");
    system("cat test-results/testdirS3/prefS3-output-02.txt");
    suite_dealloc(&suite);
  } // ENDTEST

  // Be cautios with using certain commands like ls as they are known
  // to vary between systems on errors:
  //
  // # home system, file in single quotes
  // val [data]% ls no-such-file.txt
  // ls: cannot access 'no-such-file.txt': No such file or directory
  //
  // # GRACE, not quotes used
  // grace7:~/216/solution-p4-216: ls no-such-file.txt
  // ls: cannot access no-such-file.txt: No such file or directory 
  IF_TEST("suite_test_start4") { 
    // Checks that a child process that returns a nonzero exit code is
    // handled properly.
    suite_t suite = {
      .testdir = strdup("test-results/testdirS4"),
      .prefix = strdup("prefS4"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[0];
    test->program = strdup("wc no-such-file.txt");
    ret = suite_test_start(&suite, 0);
    test_state_t state = test->state;
    printf("test state: %s (%d)\n",test_state_str(state),state);
    if(ret != 0){ printf("Non-zero wait() return\n"); }
    pid_t child_pid = test->child_pid;
    printf("child_pid is %s\n",
           child_pid>0 ? "positive":"ZERO or NEGATIVE");
    ret = waitpid(child_pid, &status, 0);
    printf("child process %s\n",
           ret==child_pid ? "completed":"DID NOT complete");
    printf("exit_code: %d\n",WEXITSTATUS(status));
    printf("PROGRAM OUTPUT\n");
    system("cat test-results/testdirS4/prefS4-output-00.txt");
    suite_dealloc(&suite);
  } // ENDTEST

  IF_TEST("suite_test_start5") { 
    // Checks that input redirection is implemented
    suite_t suite = {
      .testdir = strdup("test-results/testdirS5"),
      .prefix = strdup("prefS5"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    char *input =
      "November 19, 1863\n"
      "Abraham Lincoln\n"
      "\n"
      "all men are created equal.\n"
      "in Liberty, and dedicated to the proposition that\n"
      "forth on this continent, a new nation, conceived\n"
      "Four score and seven years ago our fathers brought\n";
    test_t *test = &suite.tests[0];
    test->input = strdup(input);
    test->program = strdup("tac"); // reverses line order
    ret = suite_test_start(&suite, 0);
    test_state_t state = test->state;
    printf("test state: %s (%d)\n",test_state_str(state),state);
    if(ret != 0){ printf("Non-zero wait() return\n"); }
    pid_t child_pid = test->child_pid;
    printf("child_pid is %s\n",
           child_pid>0 ? "positive":"ZERO or NEGATIVE");
    ret = waitpid(child_pid, &status, 0);
    printf("child process %s\n",
           ret==child_pid ? "completed":"DID NOT complete");
    printf("exit_code: %d\n",WEXITSTATUS(status));
    printf("PROGRAM OUTPUT\n");
    system("cat test-results/testdirS5/prefS5-output-00.txt");
    suite_dealloc(&suite);
  } // ENDTEST

  
  IF_TEST("suite_test_finish1") { 
    // Checks that suite_test_finish() sets properties of the test and
    // suite after the test is finished. This includes reading the
    // output from the test from outfile_name and storing it in the
    // output_actual field, setting the exit_code for the tests,
    // setting the state to PASS / FAIL after comparing the expected
    // vs actual output/exit_codes, and incrementing the suite's
    // tests_passed if the test passes. This test has no output and
    // passes.
    suite_t suite = {
      .testdir = strdup("test-results/testdirF1"),
      .prefix = strdup("prefF1"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[0];
    test->program = strdup("true");
    test->output_expect = strdup(""); // output should be empty
    ret = suite_test_start(&suite, 0);
    if(ret != 0){ printf("Non-zero wait() return\n"); }
    pid_t child_pid = test->child_pid;
    printf("child_pid is %s\n",
           child_pid>0 ? "positive":"ZERO or NEGATIVE");
    ret = waitpid(child_pid, &status, 0);
    printf("child process %s\n",
           ret==child_pid ? "completed":"DID NOT complete");
    ret = suite_test_finish(&suite,0,status);
    suite_test_print1(&suite, 0);
    suite_dealloc(&suite);
  } // ENDTEST


  IF_TEST("suite_test_finish2") { 
    // Checks that suite_test_finish() sets properties of the test and
    // suite after the test is finished.This test has some output and
    // passes as the output / exit_code match expected values.
    suite_t suite = {
      .testdir = strdup("test-results/testdirF2"),
      .prefix = strdup("prefF2"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[0];
    test->program = strdup("seq 10 50 300");
    char *output_expect =
      "10\n"
      "60\n"
      "110\n"
      "160\n"
      "210\n"
      "260\n";
    test->output_expect = strdup(output_expect);
    ret = suite_test_start(&suite, 0);
    if(ret != 0){ printf("Non-zero wait() return\n"); }
    pid_t child_pid = test->child_pid;
    printf("child_pid is %s\n",
           child_pid>0 ? "positive":"ZERO or NEGATIVE");
    ret = waitpid(child_pid, &status, 0);
    printf("child process %s\n",
           ret==child_pid ? "completed":"DID NOT complete");
    ret = suite_test_finish(&suite,0,status);
    suite_test_print1(&suite, 0);
    suite_dealloc(&suite);
  } // ENDTEST


  IF_TEST("suite_test_finish3") { 
    // Checks that suite_test_finish() sets properties of the test and
    // suite after the test is finished. This test FAILS as the
    // expected and actual output do not match. The EXPECTED output is
    // intentionally wrong to create this situation. The test result should
    // be TEST_FAILED and the tests_passed should NOT increment.
    suite_t suite = {
      .testdir = strdup("test-results/testdirF3"),
      .prefix = strdup("prefF3"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[0];
    test->program = strdup("seq 10 20");
    char *output_expect =
      "21\n"
      "22\n"
      "23\n"
      "Intentionally wrong\n";
    test->output_expect = strdup(output_expect);
    ret = suite_test_start(&suite, 0);
    if(ret != 0){ printf("Non-zero wait() return\n"); }
    pid_t child_pid = test->child_pid;
    printf("child_pid is %s\n",
           child_pid>0 ? "positive":"ZERO or NEGATIVE");
    ret = waitpid(child_pid, &status, 0);
    printf("child process %s\n",
           ret==child_pid ? "completed":"DID NOT complete");
    ret = suite_test_finish(&suite,0,status);
    suite_test_print1(&suite, 0);
    suite_dealloc(&suite);
  } // ENDTEST

  IF_TEST("suite_test_finish4") { 
    // Checks that suite_test_finish() sets properties of the test and
    // suite after the test is finished. The test that runs in this
    // case experiences a segmentation fault which should be
    // identified and reported via negative exit_code_actual (-11 in
    // this case). This should cause the test to fail. The expected
    // and actual output will match BUT the expected exit code is 0
    // and this should cause the test to fail.
    suite_t suite = {
      .testdir = strdup("test-results/testdirF4"),
      .prefix = strdup("prefF4"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[0];
    test->program = strdup("bash data/raise_sigsegv.sh");
    char *output_expect =
      "This script raises a SIGSEGV\n"
      "About the raise the signal which\n"
      "should end the program\n";
    test->output_expect = strdup(output_expect);
    test->exit_code_expect = 0;  // will not match causing FAIL
    ret = suite_test_start(&suite, 0);
    if(ret != 0){ printf("Non-zero wait() return\n"); }
    pid_t child_pid = test->child_pid;
    printf("child_pid is %s\n",
           child_pid>0 ? "positive":"ZERO or NEGATIVE");
    ret = waitpid(child_pid, &status, 0);
    printf("child process %s\n",
           ret==child_pid ? "completed":"DID NOT complete");
    ret = suite_test_finish(&suite,0,status);
    suite_test_print1(&suite, 0);
    suite_dealloc(&suite);
  } // ENDTEST
  
  IF_TEST("suite_test_finish5") { 
    // Checks that suite_test_finish() sets properties of the test and
    // suite after the test is finished. This test expects input so
    // must have proper stdin redirection and ends via a signal but
    // this is expected. The test should PASS as the expected output
    // will match and the -15 (SIGTERM) is also expected.
    suite_t suite = {
      .testdir = strdup("test-results/testdirF5"),
      .prefix = strdup("prefF5"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[0];
    test->program = strdup("bash data/count_terminate.sh apateu");
    char *input = 
      "Apateu, apateu\n"
      "Apateu, apateu\n"
      "Apateu, apateu\n"
      "Uh, uh-huh, uh-huh\n"
      "Apateu, apateu\n"
      "Apateu, apateu\n"
      "Apateu, apateu\n"
      "Uh, uh-huh, uh-huh\n";
    test->input = strdup(input);
    char *output_expect =
      "The word 'apateu' appeared on 6 lines\n"
      "Terminating now\n";
    test->output_expect = strdup(output_expect);
    test->exit_code_expect = -15;  // signal expected 
    ret = suite_test_start(&suite, 0);
    if(ret != 0){ printf("Non-zero wait() return\n"); }
    pid_t child_pid = test->child_pid;
    printf("child_pid is %s\n",
           child_pid>0 ? "positive":"ZERO or NEGATIVE");
    ret = waitpid(child_pid, &status, 0);
    printf("child process %s\n",
           ret==child_pid ? "completed":"DID NOT complete");
    ret = suite_test_finish(&suite,0,status);
    suite_test_print1(&suite, 0);
    suite_dealloc(&suite);
  } // ENDTEST
  
  IF_TEST("suite_test_finish6") { 
    // Checks that suite_test_finish() sets properties of the test and
    // suite after the test is finished. The test run requires special
    // handling to set up its argv[] array as there is quoting
    // involved. This is best handled via the provided
    // split_into_argv() function. The program returns a non-zero exit
    // code which is expected so that the test should still PASS.
    suite_t suite = {
      .testdir = strdup("test-results/testdirF6"),
      .prefix = strdup("prefF6"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[0];
    test->program = strdup("bash -c 'echo Hello world && exit 5'");
    char *output_expect =
      "Hello world\n";
    test->output_expect = strdup(output_expect);
    test->exit_code_expect = 5;
    ret = suite_test_start(&suite, 0);
    if(ret != 0){ printf("Non-zero wait() return\n"); }
    pid_t child_pid = test->child_pid;
    printf("child_pid is %s\n",
           child_pid>0 ? "positive":"ZERO or NEGATIVE");
    ret = waitpid(child_pid, &status, 0);
    printf("child process %s\n",
           ret==child_pid ? "completed":"DID NOT complete");
    ret = suite_test_finish(&suite,0,status);
    suite_test_print1(&suite, 0);
    suite_dealloc(&suite);
  } // ENDTEST
  
  IF_TEST("suite_test_finish7") { 
    // Checks that suite_test_finish() sets properties of the test and
    // suite after the test is finished. Checks the special case where
    // the test's output_expect is NULL so that checking output should
    // be skipped; the test should PASS. Ensures that the
    // suite_test_finish() function does not access a NULL during its
    // checks for pass/fail.
    suite_t suite = {
      .testdir = strdup("test-results/testdirF7"),
      .prefix = strdup("prefF7"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[0];
    test->program = strdup("seq 9");
    test->output_expect = NULL; // skip output check
    test->exit_code_expect = 0;
    ret = suite_test_start(&suite, 0);
    if(ret != 0){ printf("Non-zero wait() return\n"); }
    pid_t child_pid = test->child_pid;
    printf("child_pid is %s\n",
           child_pid>0 ? "positive":"ZERO or NEGATIVE");
    ret = waitpid(child_pid, &status, 0);
    printf("child process %s\n",
           ret==child_pid ? "completed":"DID NOT complete");
    ret = suite_test_finish(&suite,0,status);
    suite_test_print1(&suite, 0);
    suite_dealloc(&suite);
  } // ENDTEST
    
  IF_TEST("suite_test_finish8") { 
    // Checks that suite_test_finish() sets properties of the test and
    // suite after the test is finished. Requires input redirection to
    // work properly and that the expected exit_code be honored. The
    // test should PASS as the expected exit_code and output should
    // match.
    suite_t suite = {
      .testdir = strdup("test-results/testdirF8"),
      .prefix = strdup("prefF8"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[0];
    test->program = strdup("bash");
    char *input =
      "echo First line\n"
      "echo Second line\n"
      "seq 200|wc\n"
      "echo Done\n"
      "exit 3\n";
    test->input = strdup(input);
    char *output_expect =
      "First line\n"
      "Second line\n"
      "    200     200     692\n"
      "Done\n";
    test->output_expect = strdup(output_expect);
    test->exit_code_expect = 3;
    ret = suite_test_start(&suite, 0);
    if(ret != 0){ printf("Non-zero wait() return\n"); }
    pid_t child_pid = test->child_pid;
    printf("child_pid is %s\n",
           child_pid>0 ? "positive":"ZERO or NEGATIVE");
    ret = waitpid(child_pid, &status, 0);
    printf("child process %s\n",
           ret==child_pid ? "completed":"DID NOT complete");
    ret = suite_test_finish(&suite,0,status);
    suite_test_print1(&suite, 0);
    suite_dealloc(&suite);
  } // ENDTEST
    
  IF_TEST("suite_test_finish9") { 
    // Checks that suite_test_finish() sets properties of the test and
    // suite after the test is finished. Requires input redirection to
    // work properly and that the expected exit_code be honored which
    // is a signal in this case.  The test should PASS as the expected
    // exit_code and output should match.
    suite_t suite = {
      .testdir = strdup("test-results/testdirF9"),
      .prefix = strdup("prefF9"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[0];
    test->program = strdup("bash");
    char *input =
      "printf 'Kissy face, kissy face\\nSent to your phone\\n'\n"
      "seq 50|grep 7\n"
      "echo About to interrupt\n"
      "kill -SIGINT $$\n"
      "echo How did this happen\n";
    test->input = strdup(input);
    char *output_expect =
      "Done\n";
    test->output_expect = strdup(output_expect);
    test->exit_code_expect = -2;
    ret = suite_test_start(&suite, 0);
    if(ret != 0){ printf("Non-zero wait() return\n"); }
    pid_t child_pid = test->child_pid;
    printf("child_pid is %s\n",
           child_pid>0 ? "positive":"ZERO or NEGATIVE");
    ret = waitpid(child_pid, &status, 0);
    printf("child process %s\n",
           ret==child_pid ? "completed":"DID NOT complete");
    ret = suite_test_finish(&suite,0,status);
    suite_test_print1(&suite, 0);
    suite_dealloc(&suite);
  } // ENDTEST
    
  IF_TEST("suite_test_start_finish_err") { 
    // Checks two erorr cases for suite_test_start().  If the output
    // file canto be created during test startup, the child process
    // should print an error and exit with the TESTFAIL_OUTPUT
    // exit_code.  The error message will appear outside of any test
    // output as output has not be redirected.
    //
    // If the exec() fails, the child process should similarly print
    // an error and exit with code TESTFAILE_EXEC. This error message
    // will appear in output_actual as output redirection will have
    // occurred at that point.
    //
    // These the error exit codes will be viewable in the test
    // exit_code_actual as numeric values. These are defined in the
    // chester.h header file.
    suite_t suite = {
      .testdir = strdup("test-results/testdirF10"),
      .prefix = strdup("prefF10"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);

    printf("\nERROR CASE 1: CANNOT CREATE TEST OUTPUT FILE\n");
    test_t *test = &suite.tests[0];
    test->program = strdup("wc");
    char *input =
      "Katz: All your bass are belong to us\n";
    test->input = strdup(input);
    // ensure that the output file cannot be created by linking it to a
    // file that users do not have permission to acces
    system("rm -rf test-results/testdirF10/prefF10-output-00.txt");
    system("ln -s /no-such-file.txt test-results/testdirF10/prefF10-output-00.txt");
    test->output_expect = NULL;
    test->exit_code_expect = 0;
    // NOTE: concurrency is tricky here as error messages from the
    // child may appear early or late; wait() first in order to ensure
    // that any child error messages appear first then check return
    // values. 
    int start_ret = suite_test_start(&suite, 0);
    pid_t child_pid = test->child_pid;
    int wait_ret = waitpid(child_pid, &status, 0);
    ret = suite_test_finish(&suite,0,status);
    if(start_ret != 0){ printf("Non-zero wait() return\n"); }
    printf("child_pid is %s\n",
           child_pid>0 ? "positive":"ZERO or NEGATIVE");
    printf("child process %s\n",
           wait_ret==child_pid ? "completed":"DID NOT complete");
    suite_test_print1(&suite, 0);
    system("rm -rf test-results/testdirF10/prefF10-output-00.txt");

    printf("\nERROR CASE 2: CANNOT EXEC TEST PROGRAM\n");
    test = &suite.tests[1];
    test->program = strdup("no_such_program");
    test->input = NULL;
    test->output_expect = NULL;
    test->exit_code_expect = 0;
    ret = suite_test_start(&suite, 1);
    if(ret != 0){ printf("Non-zero wait() return\n"); }
    child_pid = test->child_pid;
    printf("child_pid is %s\n",
           child_pid>0 ? "positive":"ZERO or NEGATIVE");
    ret = waitpid(child_pid, &status, 0);
    printf("child process %s\n",
           ret==child_pid ? "completed":"DID NOT complete");
    ret = suite_test_finish(&suite,1,status);
    suite_test_print1(&suite, 1);

    suite_dealloc(&suite);
  } // ENDTEST

  ////////////////////////////////////////////////////////////////////////////////
  // PROBLEM 3 TESTS
  ////////////////////////////////////////////////////////////////////////////////

  IF_TEST("//////////////////// PROBLEM 3 ////////////////////") {
  } // ENDTEST
  IF_TEST("print_window1") {
    // Basic tests of print_window() which are shown in the examples
    // associated with the code.
    char *s = "ABCDEFGHIJKL";
    //         012345678901
    printf("\n--CALL 1--\n");
    print_window(stdout, s, 4, 3);
    // BCDEFGH
    // 1234567
    printf("\n--CALL 2--\n");
    print_window(stdout, s, 2, 5);
    // ABCDEFGH
    // 01234567
    printf("\n--CALL 3--\n");
    print_window(stdout, s, 8, 4);
    // EFGHIJKL
    // 45678901
    printf("\n");
  } // ENDTEST

  IF_TEST("print_window2") { 
    char *s =
      "Four score and seven years ago our fathers brought\n"
      "forth on this continent, a new nation, conceived\n"
      "in Liberty, and dedicated to the proposition that\n"
      "all men are created equal.\n"
      "\n"
      "Now we are engaged in a great civil war, testing\n"
      "whether that nation, or any nation so conceived\n"
      "and so dedicated, can long endure. We are met on a\n"
      "great battle-field of that war. We have come to\n"
      "dedicate a portion of that field, as a final\n"
      "resting place for those who here gave their lives\n"
      "that that nation might live. It is altogether\n"
      "fitting and proper that we should do this.\n"
      "\n"
      "But, in a larger sense, we can not dedicate -- we\n"
      "can not consecrate -- we can not hallow -- this\n"
      "ground. The brave men, living and dead, who\n"
      "struggled here, have consecrated it, far above our\n"
      "poor power to add or detract. The world will\n"
      "little note, nor long remember what we say here,\n"
      "but it can never forget what they did here. It is\n"
      "for us the living, rather, to be dedicated here to\n"
      "the unfinished work which they who fought here\n"
      "have thus far so nobly advanced. It is rather for\n"
      "us to be here dedicated to the great task\n"
      "remaining before us -- that from these honored\n"
      "dead we take increased devotion to that cause for\n"
      "which they gave the last full measure of devotion\n"
      "-- that we here highly resolve that these dead\n"
      "shall not have died in vain -- that this nation,\n"
      "under God, shall have a new birth of freedom --\n"
      "and that government of the people, by the people,\n"
      "for the people, shall not perish from the earth.\n"
      "\n"
      "Abraham Lincoln\n"
      "November 19, 1863\n";
    printf("\n--CALL 1--\n");
    print_window(stdout, s,  10, 30); // truncate start
    printf("\n--CALL 2--\n");
    print_window(stdout, s, 200, 30);
    printf("\n--CALL 3--\n");
    print_window(stdout, s, 627, 30);
    printf("\n--CALL 4--\n");
    print_window(stdout, s,1485, 30); // truncate end
    printf("\n");
  } // ENDTEST

  IF_TEST("differing_index1") {
    ret = differing_index("01234567","0123x567");
    printf("CALL 1: %d\n",ret);
    ret = differing_index("012345","01234567");
    printf("CALL 1: %d\n",ret);
    ret = differing_index("012345","01x34567");
    printf("CALL 1: %d\n",ret);
    ret = differing_index("012345","012345");
    printf("CALL 1: %d\n",ret);
  } // ENDTEST

  
  IF_TEST("suite_test_make_resultfile1") { 
    // Sets up a PASSING test with all necessary attributes to create
    // a resultfile. Checks that suite_test_make_resultfile() creates
    // the correct output for the test.
    suite_t suite = {
      .testdir = strdup("test-results/testdirMR1"),
      .prefix = strdup("prefMR1"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[0];
    test->title =
      strdup("A Test of seq");
    test->description =
      strdup("A description of using seq\n");
    test->program = strdup("seq 5");
    test->input = NULL;
    test->output_expect =
      strdup("1\n"
             "2\n"
             "3\n"
             "4\n"
             "5\n");
    test->output_actual =
      strdup(test->output_expect);
    test->exit_code_expect = 0;
    test->exit_code_actual = 0;
    test->state = TEST_PASSED;
    ret = suite_test_make_resultfile(&suite, 0);
    printf("ret: %d\n",ret);
    printf("test->resultfile_name: %s\n",test->resultfile_name);
    printf("---RESULTFILE CONTENTS---\n");
    sprintf(cmdbuf,"cat %s",test->resultfile_name);
    system(cmdbuf);
    printf("---\n");
    suite_dealloc(&suite);
  } // ENDTEST
    
  IF_TEST("suite_test_make_resultfile2") { 
    // Sets up a FAILED test where the failure is due to an output
    // mismatch. Checks that suite_test_make_resultfile() formats
    // output correctly such as printing "FAIL" along with the test
    // title and printing the character position where a difference in
    // the output appears along with a window around that position
    // using differing_index() and print_window() respectively.
    suite_t suite = {
      .testdir = strdup("test-results/testdirMR2"),
      .prefix = strdup("prefMR2"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[0];
    test->title = strdup("A Failing Test of seq");
    test->description =
      strdup("A description of using seq\n"
             "The description spans several lines\n"
             "and should all appear in the resultfile\n"
             "\n");
    test->program = strdup("seq 5");
    test->input = NULL;
    test->output_expect =
      strdup("1\n"
             "2\n"
             "3\n"
             "4\n"
             "5\n");
    test->output_actual =
      strdup("1\n"
             "20\n"
             "3\n");
    test->exit_code_expect = 0;
    test->exit_code_actual = 0;
    test->state = TEST_FAILED;
    ret = suite_test_make_resultfile(&suite, 0);
    printf("ret: %d\n",ret);
    printf("test->resultfile_name: %s\n",test->resultfile_name);
    printf("---RESULTFILE CONTENTS---\n");
    sprintf(cmdbuf,"cat %s",test->resultfile_name);
    system(cmdbuf);
    printf("---\n");
    suite_dealloc(&suite);
  } // ENDTEST
    
  IF_TEST("suite_test_make_resultfile3") { 
    // Checks suite_test_make_resultfile() on whether it correctly
    // formats the output of the EXIT CODE section.
    suite_t suite = {
      .testdir = strdup("test-results/testdirMR3"),
      .prefix = strdup("prefMR3"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[0];
    test->title = strdup("Failure Due to Exit Code");
    test->description =
      strdup("true fails to run correctly\n");
    test->program = strdup("true");
    test->input = NULL;
    test->output_expect = strdup("");
    test->output_actual = strdup("");
    test->exit_code_expect = 0;
    test->exit_code_actual = 3;
    test->state = TEST_FAILED;
    suite_test_make_resultfile(&suite, 0);
    printf("test->resultfile_name: %s\n",test->resultfile_name);
    printf("---RESULTFILE CONTENTS---\n");
    sprintf(cmdbuf,"cat %s",test->resultfile_name);
    system(cmdbuf);
    printf("---\n");
    suite_dealloc(&suite);
  } // ENDTEST
    
  IF_TEST("suite_test_make_resultfile4") { 
    // Checks whether suite_test_make_resultfile() honors the
    // output_expect field being NULL in which case the OUTPUT section
    // should print "check skipped" to indicate no output checking was
    // done.
    suite_t suite = {
      .testdir = strdup("test-results/testdirMR4"),
      .prefix = strdup("prefMR4"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[0];
    test->title = strdup("Expected output skipped");
    test->description =
      strdup("A short description of a test that skip\n"
             "checking output\n");
    test->program = strdup("gcc x.c");
    test->input = NULL;
    test->output_expect = NULL;
    test->output_actual =
      strdup("This output should not be checked\n");
    test->exit_code_expect = 0;
    test->exit_code_actual = 0;
    test->state = TEST_PASSED;
    suite_test_make_resultfile(&suite, 0);
    printf("test->resultfile_name: %s\n",test->resultfile_name);
    printf("---RESULTFILE CONTENTS---\n");
    sprintf(cmdbuf,"cat %s",test->resultfile_name);
    system(cmdbuf);
    printf("---\n");
    suite_dealloc(&suite);
  } // ENDTEST


  IF_TEST("suite_test_make_resultfile5") { 
    // Checks that when the input field of the test is not NULL, the
    // INPUT section in the result file is created properly by
    // suite_test_make_resultfile() and contains the test input.
    suite_t suite = {
      .testdir = strdup("test-results/testdirMR5"),
      .prefix = strdup("prefMR5"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[0];
    test->title = strdup("Input present");
    test->description =
      strdup("The test uses input, it should be printed in the\n"
             "resultfile\n\n");
    test->program = strdup("bash");
    test->input =
      strdup("echo Hello world\n"
             "seq 3\n"
             "exit 2\n"
             "echo Unreachable\n");
    test->output_expect =
      strdup("Hello world\n"
             "1\n"
             "2\n"
             "3\n"
             "Unreachable\n");
    test->output_actual =
      strdup("Hello world\n"
             "1\n"
             "2\n"
             "3\n");
    test->exit_code_expect = 0;
    test->exit_code_actual = 2;
    test->state = TEST_FAILED;
    suite_test_make_resultfile(&suite, 0);
    printf("test->resultfile_name: %s\n",test->resultfile_name);
    printf("---RESULTFILE CONTENTS---\n");
    sprintf(cmdbuf,"cat %s",test->resultfile_name);
    system(cmdbuf);
    printf("---\n");
    suite_dealloc(&suite);
  } // ENDTEST


  IF_TEST("suite_test_make_resultfile6") { 
    // Checks that for tests with a larger expected / actual output,
    // the OUTPUT section is created correctly by
    // suite_test_make_resultfile() and that the output generated
    // using print_window() is correct with a window of TEST_DIFFWIDTH
    // characters around the first mismatching character.
    suite_t suite = {
      .testdir = strdup("test-results/testdirMR6"),
      .prefix = strdup("prefMR6"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[0];
    test->title = strdup("Larger output diff");
    test->description =
      strdup("Larger output present with a diffreence between\n"
             "expect and actual\n\n");
    test->program = strdup("grep -n -i IT data/gettysburg.txt");
    test->input = NULL;
    test->output_expect =
      strdup("3:in Liberty, and dedicated to the proposition that\n"
             "12:that that nation might live. It is altogether\n"
             "13:fitting and proper that we should do this.\n"
             "18:struggled here, have consecrated it, far above our\n"
             "20:little note, nor long remember what we say here,\n"
             "21:but it can never forget what they did here. It is\n"
             "24:have thus far so nobly advanced. It is rather for\n");
    test->output_actual =
      strdup("3:in Liberty, and dedicated to the proposition that\n"
             "12:that that nation might live. It is altogether\n"
             "13:fitting and proper that we should do this.\n"
             "18:struggled here, have consecrated it, far above our\n"
             "20:little note, NOR LONG REMEMBER what we say here,\n"
             "21:but it can never forget what they did here. It is\n"
             "24:have thus far so nobly advanced. It is rather for\n");
    test->exit_code_expect = 0;
    test->exit_code_actual = 0;
    test->state = TEST_FAILED;
    suite_test_make_resultfile(&suite, 0);
    printf("test->resultfile_name: %s\n",test->resultfile_name);
    printf("---RESULTFILE CONTENTS---\n");
    sprintf(cmdbuf,"cat %s",test->resultfile_name);
    system(cmdbuf);
    printf("---\n");
    suite_dealloc(&suite);
  } // ENDTEST

  IF_TEST("suite_test_make_resultfile7") { 
    // Checks that if suite_test_make_resultfile() cannot create the
    // result file, it bails after printing an error message.
    suite_t suite = {
      .testdir = strdup("test-results/testdirMR7"),
      .prefix = strdup("prefMR7"),
      .tests_count = 10,
    };
    suite_create_testdir(&suite);
    test_t *test = &suite.tests[0];
    test->title = strdup("Cannot ceate");
    system("rm -f test-results/testdirMR7/prefMR7-result-00.md");
    system("ln -s /usr/bin/gcc test-results/testdirMR7/prefMR7-result-00.md");
    // set up existing file which cannot be opened for writing
    ret = suite_test_make_resultfile(&suite, 0);
    // should print an error message
    printf("ret: %d\n",ret);
    suite_dealloc(&suite);
    system("rm -f test-results/testdirMR7/prefMR7-result-00.md");
  } // ENDTEST

  ////////////////////////////////////////////////////////////////////////////////
  // PROBLEM 4 TESTS
  ////////////////////////////////////////////////////////////////////////////////

  IF_TEST("//////////////////// PROBLEM 4 ////////////////////") {
  } // ENDTEST

  IF_TEST("suite_run_tests_singleproc1") { 
    // Loads a test file with a single test in the suite and sets it
    // to be the test to run.  Checks that
    // suite_run_tests_singleproc() runs the test, produces the
    // correct output messages, and creates the necessary results
    // files.
    suite_t suite;
    suite_test_setup(&suite,"data/one_test.md","RTS1");
    suite.tests_torun[0] = 0;
    suite.tests_torun_count = 1;
    printf("CALLING FUNCTION\n");
    ret = suite_run_tests_singleproc(&suite);
    printf("FUNCTION DONE\n");
    printf("ret: %d\n",ret);
    suite_test_print_all(&suite);
    suite_dealloc(&suite);
  } // ENDTEST

  IF_TEST("suite_run_tests_singleproc2") { 
    // Loads a test suite with 4 tests and runs all 4 of them via
    // suite_run_tests_singleproc(). 2 of the 4 tests pass.. Checks
    // that the results file for each test is formatted correctly.
    suite_t suite;
    suite_test_setup(&suite,"data/four_tests.md","RTS2");
    suite.tests_torun_count = 4;
    for(int i=0; i<4; i++){
      suite.tests_torun[i] = i;
    }
    printf("CALLING FUNCTION\n");
    ret = suite_run_tests_singleproc(&suite);
    printf("FUNCTION DONE\n");
    printf("ret: %d\n",ret);
    suite_test_print_all(&suite);
    suite_dealloc(&suite);
  } // ENDTEST

  IF_TEST("suite_run_tests_singleproc3") { 
    // Loads a file with 4 tests but sets the suite so that only 2 of
    // the 4 tests run. Checks that suite_run_tests_singleproc()
    // honors the tests_torun[] and tests_torun_count fields as
    // containing the tests that should be run. All tests fail.
    suite_t suite;
    suite_test_setup(&suite,"data/four_tests.md","RTS3");
    suite.tests_torun_count = 2;
    suite.tests_torun[0] = 1;   // only run 2 tests of the 4
    suite.tests_torun[1] = 3;
    printf("CALLING FUNCTION\n");
    ret = suite_run_tests_singleproc(&suite);
    printf("FUNCTION DONE\n");
    printf("ret: %d\n",ret);
    suite_test_print_all(&suite);
    suite_dealloc(&suite);
  } // ENDTEST

  IF_TEST("suite_run_tests_singleproc4") { 
    // Loads a suite that has some special cases of tests like test
    // programs that are signaled, have empty input, fail due to
    // mismatched exit codes, etc. Runs all tests in the suite and
    // checks shows results files to ensure they are formatted
    // correctly.
    suite_t suite;
    suite_test_setup(&suite,"data/special_cases.md","RTS4");
    suite.tests_torun_count = suite.tests_count;
    for(int i=0; i<suite.tests_count; i++){
      suite.tests_torun[i] = i;
    }
    printf("CALLING FUNCTION\n");
    ret = suite_run_tests_singleproc(&suite);
    printf("FUNCTION DONE\n");
    printf("ret: %d\n",ret);
    suite_test_print_all(&suite);
    suite_dealloc(&suite);
  } // ENDTEST

  IF_TEST("suite_run_tests_singleproc5") { 
    // Loads a suite that has some special cases but only runs the
    // even numbered tests.
    suite_t suite;
    suite_test_setup(&suite,"data/special_cases.md","RTS5");
    suite.tests_torun_count = 0;
    for(int i=0; i<suite.tests_count; i+=2){
      suite.tests_torun[i/2] = i; // run even # tests only
      suite.tests_torun_count++;
    }
    printf("CALLING FUNCTION\n");
    ret = suite_run_tests_singleproc(&suite);
    printf("FUNCTION DONE\n");
    printf("ret: %d\n",ret);
    suite_test_print_all(&suite);
    suite_dealloc(&suite);
  } // ENDTEST

  IF_TEST("suite_run_tests_singleproc_err") { 
    // Loads a test suite with 4 tests and runs all 4 of them via
    // suite_run_tests_singleproc(). 2 of the 4 tests pass.. Checks
    // that the results file for each test is formatted correctly.
    suite_t suite;
    suite_test_setup(&suite,"data/four_tests.md","RTSerr");
    suite.tests_torun_count = 4;
    for(int i=0; i<4; i++){
      suite.tests_torun[i] = i;
    }
    printf("CREATING ERROR CONDITIONS FOR TESTDIR\n");
    system("ln -s /usr/bin/gcc test-results/testdirRTSerr");
    printf("CALLING FUNCTION\n");
    ret = suite_run_tests_singleproc(&suite);
    printf("FUNCTION DONE\n");
    printf("ret: %d\n",ret);
    // suite_test_print_all(&suite);
    suite_dealloc(&suite);
    system("rm -f test-results/testdirRTSerr");
  } // ENDTEST


  IF_TEST("suite_print_results_table1") { 
    // Loads a test file with a single test, runs the suite, then
    // checks the output of suite_print_results_table().
    suite_t suite;
    suite_test_setup(&suite,"data/one_test.md","PR1");
    suite.tests_torun[0] = 0;
    suite.tests_torun_count = 1;
    printf("RUNNING SUITE\n");
    ret = suite_run_tests_singleproc(&suite);
    printf("SUITE DONE\n");
    printf("\nCALLING RESULT TABLE FUNCTION\n");
    suite_print_results_table(&suite);
    suite_dealloc(&suite);
  } // ENDTEST

  IF_TEST("suite_print_results_table2") { 
    // Loads a suite file with 4 tests, runs the suite, then
    // checks the output of suite_print_results_table().
    suite_t suite;
    suite_test_setup(&suite,"data/four_tests.md","PR2");
    suite.tests_torun_count = 4;
    for(int i=0; i<4; i++){
      suite.tests_torun[i] = i;
    }
    printf("RUNNING SUITE\n");
    ret = suite_run_tests_singleproc(&suite);
    printf("SUITE DONE\n");
    printf("\nCALLING RESULT TABLE FUNCTION\n");
    suite_print_results_table(&suite);
    suite_dealloc(&suite);
  } // ENDTEST

  IF_TEST("suite_print_results_table3") { 
    // Loads a suite file with 4 tests, runs only 2 tests in the
    // suite, then checks the output of suite_print_results_table().
    suite_t suite;
    suite_test_setup(&suite,"data/four_tests.md","RTS3");
    suite.tests_torun_count = 2;
    suite.tests_torun[0] = 1;   // only run 2 tests of the 4
    suite.tests_torun[1] = 3;
    printf("RUNNING SUITE\n");
    ret = suite_run_tests_singleproc(&suite);
    printf("SUITE DONE\n");
    printf("\nCALLING RESULT TABLE FUNCTION\n");
    suite_print_results_table(&suite);
    suite_dealloc(&suite);
  } // ENDTEST

  IF_TEST("suite_print_results_table4") { 
    // Loads a suite file with 4 tests, runs only 2 tests in the
    // suite, then checks the output of suite_print_results_table().
    suite_t suite;
    suite_test_setup(&suite,"data/special_cases.md","PR4");
    suite.tests_torun_count = 0;
    for(int i=0; i<suite.tests_count; i+=2){
      suite.tests_torun[i/2] = i; // run even # tests only
      suite.tests_torun_count++;
    }
    printf("RUNNING SUITE\n");
    ret = suite_run_tests_singleproc(&suite);
    printf("SUITE DONE\n");
    printf("\nCALLING RESULT TABLE FUNCTION\n");
    suite_print_results_table(&suite);
    suite_dealloc(&suite);
  } // ENDTEST

  IF_TEST("DUMMY") {
  } // ENDTEST



  ////////////////////////////////////////////////////////////////////////////////
  // END OF ALL TESTS

  if(nrun == 0){                // check that at least one test was run
    printf("No test named '%s' found\n",test_name);
    return 1;
  }
  return 0;
}

// // ALTERNATE VERSIONS OF ABOVE TESTS
//   IF_TEST("suite_run_tests_singleproc1A") { 
//     // Loads a test file with a single test in the suite and sets it
//     // to be the test to run.  Checks that
//     // suite_run_tests_singleproc() runs the test, produces the
//     // correct output messages, and creates the necessary results
//     // files.
//     suite_t suite;
//     suite_init_from_file_peg(&suite,"data/one_test.md");
//     free(suite.testdir); free(suite.prefix);
//     suite.testdir = strdup("test-results/testdirRTS1");
//     suite.prefix = strdup("prefRTS1");
//     suite.tests_torun[0] = 0;
//     suite.tests_torun_count = 1;
//     printf("CALLING FUNCTION\n");
//     ret = suite_run_tests_singleproc(&suite);
//     printf("FUNCTION DONE\n");
//     printf("ret: %d\n",ret);
//     printf("suite.tests_passed: %d\n",suite.tests_passed);
//     printf("\n");
//     printf("CONTENTS OF TESTDIR\n");
//     sprintf(cmdbuf,"ls %s",suite.testdir);
//     system(cmdbuf);
//     for(int i=0; i<suite.tests_torun_count; i++){
//       int testnum = suite.tests_torun[i];
//       test_t *test = &suite.tests[testnum];
//       // printf("tests[%d].output_actual:\n---\n%s---",
//       //        testnum,test->output_actual);
//       printf("tests[%d].resultfile_name:\n%s\n",
//              testnum,test->resultfile_name);
//       printf("---RESULTFILE CONTENTS---\n");
//       sprintf(cmdbuf,"cat %s",test->resultfile_name);
//       system(cmdbuf);
//       printf("------\n\n");
//     }      
//     suite_dealloc(&suite);
//   } // ENDTEST

//   IF_TEST("suite_run_tests_singleproc2A") { 
//     // Loads a test suite with 4 tests and runs all 4 of them via
//     // suite_run_tests_singleproc(). 2 of the 4 tests pass.. Checks
//     // that the results file for each test is formatted correctly.
//     suite_t suite;
//     suite_init_from_file_peg(&suite,"data/four_tests.md");
//     free(suite.testdir); free(suite.prefix);
//     suite.testdir = strdup("test-results/testdirRTS2");
//     suite.prefix = strdup("prefRTS2");
//     suite.tests_torun[0] = 0;
//     suite.tests_torun[1] = 1;
//     suite.tests_torun[2] = 2;
//     suite.tests_torun[3] = 3;
//     suite.tests_torun_count = 4;
//     printf("CALLING FUNCTION\n");
//     ret = suite_run_tests_singleproc(&suite);
//     printf("FUNCTION DONE\n");
//     printf("ret: %d\n",ret);
//     printf("suite.tests_passed: %d\n",suite.tests_passed);
//     printf("\n");
//     for(int i=0; i<suite.tests_torun_count; i++){
//       int testnum = suite.tests_torun[i];
//       test_t *test = &suite.tests[testnum];
//       printf("tests[%d].resultfile_name:\n%s\n",
//              testnum,test->resultfile_name);
//       printf("---RESULTFILE CONTENTS---\n");
//       sprintf(cmdbuf,"cat %s",test->resultfile_name);
//       system(cmdbuf);
//       printf("------\n\n");
//     }      
//     suite_dealloc(&suite);
//   } // ENDTEST


//   IF_TEST("suite_run_tests_singleproc3A") { 
//     // Loads a file with 4 tests but sets the suite so that only 2 of
//     // the 4 tests run. Checks that suite_run_tests_singleproc()
//     // honors the tests_torun[] and tests_torun_count fields as
//     // containing the tests that should be run. All tests fail.
//     suite_t suite;
//     suite_init_from_file_peg(&suite,"data/four_tests.md");
//     free(suite.testdir); free(suite.prefix);
//     suite.testdir = strdup("test-results/testdirRTS3");
//     suite.prefix = strdup("prefRTS3");
//     suite.tests_torun[0] = 1;   // only run 2 tests of the 4
//     suite.tests_torun[1] = 3;
//     suite.tests_torun_count = 2;
//     printf("CALLING FUNCTION\n");
//     ret = suite_run_tests_singleproc(&suite);
//     printf("FUNCTION DONE\n");
//     printf("ret: %d\n",ret);
//     printf("suite.tests_passed: %d\n",suite.tests_passed);
//     printf("\n");
//     for(int i=0; i<suite.tests_torun_count; i++){
//       int testnum = suite.tests_torun[i];
//       test_t *test = &suite.tests[testnum];
//       printf("tests[%d].resultfile_name:\n%s\n",
//              testnum,test->resultfile_name);
//       printf("---RESULTFILE CONTENTS---\n");
//       sprintf(cmdbuf,"cat %s",test->resultfile_name);
//       system(cmdbuf);
//       printf("------\n\n");
//     }      
//     suite_dealloc(&suite);
//   } // ENDTEST

//   IF_TEST("suite_run_tests_singleproc4A") { 
//     // Loads a suite that has some special cases of tests like test
//     // programs that are signaled, have empty input, fail due to
//     // mismatched exit codes, etc. Runs all tests in the suite and
//     // checks shows results files to ensure they are formatted
//     // correctly.
//     suite_t suite;
//     suite_init_from_file_peg(&suite,"data/special_cases.md");
//     free(suite.testdir); free(suite.prefix);
//     suite.testdir = strdup("test-results/testdirRTS4");
//     suite.prefix = strdup("prefRTS4");
//     suite.tests_torun_count = 7;
//     for(int i=0; i<suite.tests_torun_count; i++){
//       suite.tests_torun[i] = i;
//     }
//     printf("CALLING FUNCTION\n");
//     ret = suite_run_tests_singleproc(&suite);
//     printf("FUNCTION DONE\n");
//     printf("ret: %d\n",ret);
//     printf("suite.tests_passed: %d\n",suite.tests_passed);
//     printf("\n");
//     for(int i=0; i<suite.tests_torun_count; i++){
//       int testnum = suite.tests_torun[i];
//       test_t *test = &suite.tests[testnum];
//       printf("tests[%d].resultfile_name:\n%s\n",
//              testnum,test->resultfile_name);
//       printf("---RESULTFILE CONTENTS---\n");
//       sprintf(cmdbuf,"cat %s",test->resultfile_name);
//       system(cmdbuf);
//       printf("------\n\n");
//     }      
//     suite_dealloc(&suite);
//   } // ENDTEST

