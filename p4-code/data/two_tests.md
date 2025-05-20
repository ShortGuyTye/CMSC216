!prefix=two-tests
!testdir=chester-test-two-tests

# Basic bash Test
Checks that bash properly produces output.

!program=bash -c 'echo Chester is; echo a Tester'

```output
Chester is
a Tester
```

# Count chars with wc
Checks that wc (word count) works with provided
input.

!program=wc

```input
This is a test.
This is only a test.
Keep calm and carry on.
```

```output
 3 14 61
```
