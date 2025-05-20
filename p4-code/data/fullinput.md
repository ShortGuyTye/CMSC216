!prefix=prob1

# echo check
Checks whether the echo command works

!program=date

```output
hello
```

# sleep 2s
Checks that sleep finishes

!program=sleep 0.5
```output
```


# pwd check
Checks whether the sequence seq command works

!program=pwd
```output
hello
```

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

# ls check
Checks whether the ls program can be run

!program=ls
```output
chester_funcs.c
chester_funcs.c~
chester_funcs.o
chester.h
chester.h~
chester_main
chester_main.c
chester_main.c~
```

# ls not there
Checks whether the ls program returns an error code on a file note existing

!program=ls no-such-file
!exit_code=2
```output
ls: cannot access 'no-such-file': No such file or directory
```

# gcc fails on file
Checks output and exit code for a gcc call.

!program=gcc no-such-file.c
!exit_code=0
```output
cc1: fatal error: no-such-file.c: No such file or directory
compilation terminated.
```

# wc 1 to 10
Checks that wc works with input

!program=wc
```input

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

```output
10 10 21
```

# date runs
Prints the date, output checks are skipped

!program=date


