#!/bin/bash
# Count how many times a word appears in standard input and report
# that, then end via a SIGTERM signal

word=$1
count=$(grep -c -i "$word" <&0)
mypid=$$
printf "The word '%s' appeared on %d lines\n" "$word" "$count"
printf "Terminating now\n"
kill -SIGTERM $mypid
printf "This output shoudl not appear\n"
