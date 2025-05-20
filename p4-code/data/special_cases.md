!prefix=special-cases
!testdir=chester-test-special

# Segfault Test A
Checks that the data/raise_sigsegv.sh program runs and the return code
is properly handled. The test should fail.

!program=bash data/raise_sigsegv.sh

```output
This script raises a SIGSEGV
About the raise the signal which
should end the program
```

# Segfault Test B
Like previous test but sets exit code expectation to the segfault signal.

!program=bash data/raise_sigsegv.sh
!exit_code=-11

```output
This script raises a SIGSEGV
About the raise the signal which
should end the program
```

# Error Redirect
Uses data/print_stdout_stderr.sh to produce output on both stdout and
stderr to make sure tests redirect both to the results file.

!program=bash data/print_stdout_stderr.sh

```output
1 First Message to stdout
2 Second Message to stderr
3 Third Message to stdout
```

# Empty Input
Sends empty input to a program.

!program=wc

```input
```

# Term Signal A
Checks that term signal is handled properly. Test fails as output and
exit_cod do not match expected.

!program=bash data/count_terminate.sh play
```input
All work and no play makes Jack a dull boy.
All work and no Play makes Jack a dull boy.
All work and no pLay makes Jack a dull boy.
All work and no pLaY makes Jack a dull boy.
All work and no PLAY makes Jack a dull boy.
All work and no PlAy makes Jack a dull boy.
```

```output
Terminating now
```

# Term Signal B
Checks that term signal is handled properly. Test passes due to exit
code not matching expectation.

!program=bash data/count_terminate.sh play

```input
All work and no play makes Jack a dull boy.
All work and no Play makes Jack a dull boy.
All work and no pLay makes Jack a dull boy.
All work and no pLaY makes Jack a dull boy.
All work and no PLAY makes Jack a dull boy.
All work and no PlAy makes Jack a dull boy.
```

```output
The word 'play' appeared on 6 lines
Terminating now
```

# Term Signal C
Checks that term signal is handled properly. Test passes due to
expected values being met.

!program=bash data/count_terminate.sh play
!exit_code=-15

```input
All work and no play makes Jack a dull boy.
All work and no Play makes Jack a dull boy.
All work and no pLay makes Jack a dull boy.
All work and no pLaY makes Jack a dull boy.
All work and no PLAY makes Jack a dull boy.
All work and no PlAy makes Jack a dull boy.
```

```output
The word 'play' appeared on 6 lines
Terminating now
```

# Ignore Output
Runs a command with unpredictable output but that output should be
ignored as no expected output is provided leading the test to pass.

!program=bash -c 'echo $RANDOM'

# Empty Description

!program=seq 4 2 12

```output
4
6
8
10
12
```
