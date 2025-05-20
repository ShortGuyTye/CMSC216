#!/bin/bash

printf "1 First Message to stdout\n"  >&1
printf "2 Second Message to stderr\n" >&2
printf "3 Third Message to stdout\n" >&1
