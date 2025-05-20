!testdir=chester-test-seq
!prefix=seq

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

