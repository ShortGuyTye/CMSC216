!prefix=four-tests

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
