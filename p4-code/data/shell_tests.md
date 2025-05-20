!prefix=shell
!testdir=chester-test-shell

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
