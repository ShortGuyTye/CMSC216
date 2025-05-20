
#include "chester.h"

////////////////////////////////////////////////////////////////////////////////
// PROBLEM 1 Functions
////////////////////////////////////////////////////////////////////////////////

int suite_create_testdir(suite_t *suite){
    //check if file exists
    if (access(suite->testdir, F_OK) ==0){
        struct stat sb;
        //check if file is a directory
        int statret = stat(suite->testdir, &sb);
        if (statret == -1){
            return -1;
        }
        if S_ISDIR (sb.st_mode) {
            return 0;
        } else {
            printf("ERROR: Could not create test directory '%s'\n", suite->testdir);
            printf("        Non-directory file with that name already exists\n");
            return -1;
        }
    } else {
        //if doesnt exist make it
        int mkdirret = mkdir(suite->testdir, S_IRUSR | S_IWUSR | S_IXUSR);
        if (mkdirret == -1){
            return -1;
        }
        return 1;
    }
}

int suite_test_set_outfile_name(suite_t *suite, int testnum){
    //create string to make file name
    char outfile[MAX_FILENAME];
    sprintf(outfile, "%s/%s-output-%02d.txt", suite->testdir, suite->prefix, testnum);
    //dup string
    char *outfile_copy = strdup(outfile);
    suite->tests[testnum].outfile_name = outfile_copy;
    return 0;
}

int suite_test_create_infile(suite_t *suite, int testnum){
    //check if infile is needed
    if (suite->tests[testnum].input == NULL){
        return 0;
    }
    //create file name
    char filename[MAX_FILENAME];
    sprintf(filename, "%s/%s-input-%02d.txt", suite->testdir, suite->prefix, testnum);
    char *filename_copy = strdup(filename);
    suite->tests[testnum].infile_name = filename_copy;
    //open file just named
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1){
        perror("Could not create input file");
        return -1;
    }
    //write file base on size of input
    int size = strlen(suite->tests[testnum].input);
    int writeret = write(fd, suite->tests[testnum].input, size);
    if (writeret == -1){
        return -1;
    }
    close(fd);
    return 0;
}

int suite_test_read_output_actual(suite_t *suite, int testnum){
    //get stats of file for size
    struct stat sb;
    int stat_ret = stat(suite->tests[testnum].outfile_name, &sb);
    if (stat_ret == -1){
        perror("Couldn't open file");
        return -1;
    }
    //sets appropriate size string
    int size = sb.st_size;
    char *output = malloc(size + 1);
    //open file
    int fd = open(suite->tests[testnum].outfile_name, O_RDONLY);
    if (fd == -1){
        perror("Couldn't open file");
        return -1;
    }
    //read from file with size
    int number_read = read(fd, output, size);
    if (number_read == -1){
        perror("Couldn't open file");
        return -1;
    }
    //null terminate and set
    output[size] = '\0';
    suite->tests[testnum].output_actual = output;
    close(fd);
    return number_read;
}

////////////////////////////////////////////////////////////////////////////////
// PROBLEM 2 Functions
////////////////////////////////////////////////////////////////////////////////

int suite_test_start(suite_t *suite, int testnum){
    //sets suite
    int outfile = suite_test_set_outfile_name(suite, testnum);
    int infile = suite_test_create_infile(suite, testnum);
    if (outfile == -1 || infile == -1){
        return -1;
    }
    //create child
    int child = fork();
    if (child == 0){
        //open file
        int fd = open(suite->tests[testnum].outfile_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fd == -1){
            exit(TESTFAIL_OUTPUT);
        }
        //output redirect
        int out = dup2(fd, STDOUT_FILENO);
        int err = dup2(fd, STDERR_FILENO);
        if (out == -1 || err == -1){
            return -1;
        }
        //check input
        if (suite->tests[testnum].infile_name != NULL){
            int fdInfile = open(suite->tests[testnum].infile_name, O_RDONLY, S_IRUSR | S_IWUSR);
            if (fdInfile == -1){
                exit(TESTFAIL_INPUT);
            }
            //input redirect
            int in = dup2(fdInfile, STDIN_FILENO);
            if (in == -1){
                return -1;
            }
        }
        //prepare and excecute
        char *new_argv[32];
        int new_argc;
        int split = split_into_argv(suite->tests[testnum].program, new_argv, &new_argc);
        if (split == -1){
            return -1;
        }
        int ret = execvp(new_argv[0], new_argv);
        if (ret == -1){
            perror("ERROR: test program failed to exec");
            exit(TESTFAIL_EXEC);
        }
    } else {
        //parent remembers child sets state and finishes
        suite->tests[testnum].child_pid = child;
        suite->tests[testnum].state = TEST_RUNNING;
        return 0;
    }
    return -1;
}

int suite_test_finish(suite_t *suite, int testnum, int status){
    //check status and set accordingly
    if (WIFEXITED(status)){
        suite->tests[testnum].exit_code_actual = WEXITSTATUS(status);
    } else {
        suite->tests[testnum].exit_code_actual = WTERMSIG(status) * -1;
    }
    //read output
    int output = suite_test_read_output_actual(suite, testnum);
    if (output == -1){
        suite->tests[testnum].state = TEST_FAILED;
        return -1;
    }
    //setting state for no expect
    if (suite->tests[testnum].output_expect == NULL){
        if (suite->tests[testnum].exit_code_expect == suite->tests[testnum].exit_code_actual){
            suite->tests[testnum].state = TEST_PASSED;
            suite->tests_passed++;
            return 0;
        } else {
            suite->tests[testnum].state = TEST_FAILED;
            return 0;
        }
    }
    //setting state for expected exists
    int output_test = strcmp(suite->tests[testnum].output_expect, suite->tests[testnum].output_actual);
    if (suite->tests[testnum].exit_code_expect == suite->tests[testnum].exit_code_actual && output_test == 0){
        suite->tests[testnum].state = TEST_PASSED;
        suite->tests_passed++;
    } else {
        suite->tests[testnum].state = TEST_FAILED;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// PROBLEM 3 Functions
////////////////////////////////////////////////////////////////////////////////

void print_window(FILE *out, char *str, int center, int lrwidth){
    //set start and stop point
    int stop = center + lrwidth;
    int start = center - lrwidth;
    //keep points inbounds
    if (strlen(str) - 1 < stop){
        stop = strlen(str) - 1;
    }
    if (start < 0){
        start = 0;
    }
    //make new string
    int newLen = stop - start + 3;
    char newString[newLen];
    //fill new string
    for (int i = 0; i < newLen - 2; i++){
        newString[i] = str[start + i];
    } 
    //newline and null terminate 
    newString[newLen - 2] = '\n';
    newString[newLen - 1] = '\0';
    fprintf(out, "%s", newString);
}

int differing_index(char *strA, char *strB){
    //check if same length
    if (strlen(strA) != strlen(strB)){
        if (strlen(strA) < strlen(strB)){
            //check for a is smaller
            for (int i = 0; i < strlen(strA); i++){
                if (strA[i] != strB[i]){
                    return i;
                }
            }
            return strlen(strA);
        } else {
            //check for b is smaller
            for (int i = 0; i < strlen(strB); i++){
                if (strA[i] != strB[i]){
                    return i;
                }
            }
            return strlen(strB);
        }
    //if same length
    } else {
        for (int i = 0; i < strlen(strA); i++){
                if (strA[i] != strB[i]){
                    return i;
                }
            }
            return -1;
    }
    return -2;
}

int suite_test_make_resultfile(suite_t *suite, int testnum){

    //create the actual file
    char filename[MAX_FILENAME];
    sprintf(filename, "%s/%s-result-%02d.md", suite->testdir, suite->prefix, testnum);
    char *filenamedup = strdup(filename);
    suite->tests[testnum].resultfile_name = filenamedup;
    FILE *fd = fopen(filename, "w");
    if (fd == NULL){
        printf("ERROR: Could not create result file '%s'\n", filename);
        return -1;
    }

    //print first line based on passing
    char line[MAX_FILENAME];
    if (suite->tests[testnum].state == 2){
        sprintf(line, "# TEST %d: %s (ok)\n", testnum, suite->tests[testnum].title);
        fprintf(fd, "%s", line);
    } else if (suite->tests[testnum].state == 3){
        sprintf(line, "# TEST %d: %s (FAIL)\n", testnum, suite->tests[testnum].title);
        fprintf(fd, "%s", line);
    }

    //print description line followed by actuall description
    fprintf(fd, "## DESCRIPTION\n");
    sprintf(line, "%s", suite->tests[testnum].description);
    fprintf(fd, "%s", line);

    //print program line
    sprintf(line, "## PROGRAM: %s\n", suite->tests[testnum].program);
    fprintf(fd, "%s", line);

    //print conditional input line
    if (suite->tests[testnum].input == NULL){
        fprintf(fd, "## INPUT: None\n");
    } else {
        sprintf(line, "## INPUT:\n%s", suite->tests[testnum].input);
        fprintf(fd, "%s", line);
    }

    //print conditional output line
    if (suite->tests[testnum].output_expect == NULL){
        fprintf(fd, "## OUTPUT: skipped check\n");
    } else if (differing_index(suite->tests[testnum].output_expect, suite->tests[testnum].output_actual) == -1){
        fprintf(fd, "## OUTPUT: ok\n");
    } else {
        sprintf(line, "## OUTPUT: MISMATCH at char position %d\n", 
            differing_index(suite->tests[testnum].output_expect, suite->tests[testnum].output_actual));
        fprintf(fd, "%s", line);
        fprintf(fd, "### Expect\n");
        print_window(fd, suite->tests[testnum].output_expect, 
            differing_index(suite->tests[testnum].output_expect, suite->tests[testnum].output_actual), TEST_DIFFWIDTH);
        fprintf(fd, "### Actual\n");
        print_window(fd, suite->tests[testnum].output_actual, 
            differing_index(suite->tests[testnum].output_expect, suite->tests[testnum].output_actual), TEST_DIFFWIDTH);
    }

    //print exit code line for mismatch and ok
    if (suite->tests[testnum].exit_code_actual == suite->tests[testnum].exit_code_expect){
        fprintf(fd, "## EXIT CODE: ok\n");
    } else {
        fprintf(fd, "## EXIT CODE: MISMATCH\n");
        sprintf(line, "- Expect: %d\n", suite->tests[testnum].exit_code_expect);
        fprintf(fd, "%s", line);
        sprintf(line, "- Actual: %d\n", suite->tests[testnum].exit_code_actual);
        fprintf(fd, "%s", line);

    }

    //result
    if (suite->tests[testnum].state == 2){
        fprintf(fd, "## RESULT: ok\n");
    } else if (suite->tests[testnum].state == 3){
        fprintf(fd, "## RESULT: FAIL\n");
    }

    //close and return
    fclose(fd);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// PROBLEM 4 Functions
////////////////////////////////////////////////////////////////////////////////

int suite_run_tests_singleproc(suite_t *suite){
    //create testdir
    int testdir = suite_create_testdir(suite);
    if (testdir == -1){
        printf("ERROR: Failed to create test directory\n");
        return -1;
    }
    printf("Running with single process: ");
    //loop over tests to run
    for (int i = 0; i < suite->tests_torun_count; i++){
        //start
        int start = suite_test_start(suite, suite->tests_torun[i]);
        if (start == -1){
            return -1;
        }
        int status;
        //wait
        int wait = waitpid(suite->tests[suite->tests_torun[i]].child_pid, &status, 0);
        if (wait == -1){
            return -1;
        }
        //finish
        int finish = suite_test_finish(suite, suite->tests_torun[i], status);
        int result = suite_test_make_resultfile(suite, suite->tests_torun[i]);
        if (finish == -1 || result == -1){
            return -1;
        }
        printf(".");
    }
    printf(" Done\n");

    return 0;
}

void suite_print_results_table(suite_t *suite){
    //create new string for states
    char fail[MAX_FILENAME + 13];
    for (int i = 0; i < suite->tests_torun_count; i++){
        if (suite->tests[suite->tests_torun[i]].state == TEST_PASSED){
            sprintf(fail, "ok");
        } else if (suite->tests[suite->tests_torun[i]].state == TEST_FAILED){
            sprintf(fail, "FAIL -> see %s", suite->tests[suite->tests_torun[i]].resultfile_name);
        }
        //print results
        printf("%2d) %-20s : %s\n", suite->tests_torun[i], suite->tests[suite->tests_torun[i]].title, fail);
    }
}
