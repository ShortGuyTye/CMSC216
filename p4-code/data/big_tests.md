!prefix=big
!testdir=chester-test-big

# seq check
Checks whether sequence program executes

!program=seq 10

```output
1
2
3
4
5
6
7
8
9
10
```

# wc 1 to 10
Checks that wc works with input; should fail as the input is slightly
mangled.

!program=wc
```input
1
2
3
4

6
7
8
9
10
```

```output
10 10 21
```

# bash with output
Checks whether bash produces output that is correct. The test should
pass.

!program=bash -c 'echo Hello world; echo More output; echo All done now;'

```output
Hello world
More output
All done now
```

# tail with input
Checks that the tail command works for given input. The expected
output and the exit code are both wrong so the test should fail.

!program=tail -n 5
!exit_code=3

```input
Now we are engaged in a great civil war, testing
whether that nation, or any nation so conceived
and so dedicated, can long endure. We are met on a
great battle-field of that war. We have come to
dedicate a portion of that field, as a final
resting place for those who here gave their lives
that that nation might live. It is altogether
fitting and proper that we should do this.
```

```output
great battle-field of that war. We have come to
dedicate a portion of that field, as a final
resting PLACE FOR those who here gave their lives
that that nation might live. It is altogether
fitting and proper that we should do this.
```

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

# seq 10
Checks whether sequence program executes

!program=seq 10

```output
1
2
3
4
5
6
7
8
9
10
```

# seq 5 5 50
Checks whether sequence program executes

!program=seq 5 5 50

```output
5
10
15
20
25
30
35
40
45
50
```

# seq 8 -1 0
Checks whether sequence program executes
Should fail.

!program=seq 5 5 50

```output
8
7
6
5
4
3
2
1

```

# seq 5000 1000 9000
Checks whether sequence program executes
Should fail

!program=seq 5000 1000 9000

```output
5000
6000
7000
8000
9000
10000
```

# seq 5000 1000 10000
Checks whether sequence program executes
Should fail

!program=seq 5000 1000 9000
!exit_code=2

```output
5000
6000
7000
8000
9000
10000
```

# seq -s ' ' 10 2 20
Checks whether sequence program executes

!program=seq -s ' ' 10 2 20

```output
10 12 14 16 18 20
```

# seq -s ' ' 10 -1 1
Checks whether sequence program executes
Should fail

!program=seq -s ' ' 10 -1 1
!exit_code=-3

```output
10 9 8 7 6 5 4 3 2 1
```

# bash echos
Uses bash echoing

!program=bash -c 'echo Hi there!; echo This is swell;'

```output
Hi there!
This is swell
```

# bash interactive A
Checks that input is properly fed to bash

!program=bash
!exit_code=3

```input
echo First line
echo Second line
seq 200|wc
echo Done
exit 3;
```

```output
First line
Second line
    200     200     692
Done
```

# bash interactive B
Checks that input is properly fed to bash

!program=bash
!exit_code=-2

```input
printf 'Kissy face, kissy face\nSent to your phone\n'
seq 50|grep 7
echo About to interrupt
kill -SIGINT $$
echo How did this happen;
```

```output
Kissy face, kissy face
Sent to your phone
7
17
27
37
47
About to interrupt
```

# bash stderr
Runs data/print_stdout_stderr.sh which produces output 
on both stdout and stderr

!program=bash data/print_stdout_stderr.sh

```output
1 First Message to stdout
2 Second Message to stderr
3 Third Message to stdout
```

# bash count
Runs data/count_terminate.sh with given input.

!program=bash data/count_terminate.sh apateu
!exit_code=-15

```input
Apateu, apateu
Apateu, apateu
Apateu, apateu
Uh, uh-huh, uh-huh
Apateu, apateu
Apateu, apateu
Apateu, apateu
Uh, uh-huh, uh-huh
```

```output
The word 'apateu' appeared on 6 lines
Terminating now
```
