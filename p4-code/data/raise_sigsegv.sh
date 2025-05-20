#!/bin/bash

mypid=$$
printf "This script raises a SIGSEGV\n"
printf "About the raise the signal which\n"
printf "should end the program\n"

kill -SIGSEGV $mypid # send signal to self

printf "This output is after the signal\n"
printf "and should not appear\n"
