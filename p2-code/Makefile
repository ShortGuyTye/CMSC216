# CMSC216 Project 2 Makefile
AN = p2
CLASS = 216

# -Wno-comment: disable warnings for multi-line comments, present in some tests
# CFLAGS = -Wall -Wno-comment -Werror -g  -Wno-format-security 
CFLAGS = -Wall -Werror -g
CC     = gcc $(CFLAGS)
SHELL  = /bin/bash
CWD    = $(shell pwd | sed 's/.*\///g')

PROGRAMS = \
	clock_main \
	test_clock_update \
	puzzlebox   \
	treemap_main \
	test_treemap_verify \


	# scale_main \
	# test_scale_update \

export PARALLEL?=True		#enable parallel testing if not overridden

all : $(PROGRAMS) 

# cleaning target to remove compiled programs/objects
clean :
	rm -f $(PROGRAMS) *.o

help :
	@echo 'Typical usage is:'
	@echo '  > make                          # build all programs'
	@echo '  > make clean                    # remove all compiled items'
	@echo '  > make zip                      # create a zip file for submission'
	@echo '  > make prob1                    # built targets associated with problem 1'
	@echo '  > make test                     # run all tests'
	@echo '  > make test-prob2               # run test for problem 2'
	@echo '  > make test-prob2 testnum=5     # run problem 2 test #5 only'
	@echo '  > make update                   # download and install any updates to project files'


############################################################
# 'make zip' to create complete.zip for submission
ZIPNAME = $(AN)-complete.zip
zip : clean clean-tests
	rm -f $(ZIPNAME)
	cd .. && zip "$(CWD)/$(ZIPNAME)" -r "$(CWD)"
	@echo Zip created in $(ZIPNAME)
	@if (( $$(stat -c '%s' $(ZIPNAME)) > 10*(2**20) )); then echo "WARNING: $(ZIPNAME) seems REALLY big, check there are no abnormally large test files"; du -h $(ZIPNAME); fi
	@if (( $$(unzip -t $(ZIPNAME) | wc -l) > 256 )); then echo "WARNING: $(ZIPNAME) has 256 or more files in it which may cause submission problems"; fi

############################################################
# `make update` to get project updates
update :
	curl -s https://www.cs.umd.edu/~profk/216/$(AN)-update.sh | /bin/bash 

################################################################################
# clock problem
prob1 : clock_main test_clock_update

clock_main : clock_main.o clock_sim.o clock_update.o
	$(CC) -o $@ $^

clock_main.o : clock_main.c clock.h
	$(CC) -c $<

clock_sim.o : clock_sim.c clock.h
	$(CC) -c $<

clock_update.o : clock_update.c clock.h
	$(CC) -c $<

test_clock_update : test_clock_update.c clock_sim.o clock_update.o
	$(CC) -o $@ $^

test-prob1 : prob1 test_clock_update test-setup
	./testy -o md test_clock_update.org $(testnum)

################################################################################
# debugging problem
prob2 : puzzlebox

puzzlebox.o : puzzlebox.c
	$(CC) -c $<

puzzlebox : puzzlebox.o
	$(CC) -o $@ $^


test-prob2 : prob2 test-setup
	./puzzlebox input.txt

################################################################################
# treemap problem
prob3 : treemap_main

treemap_main : treemap_main.o treemap_funcs.o
	$(CC) -o $@ $^

treemap_main.o : treemap_main.c treemap.h
	$(CC) -c $<

treemap_funcs.o : treemap_funcs.c treemap.h
	$(CC) -c $<

test_treemap_verify : test_treemap_verify.c treemap.h
	$(CC) -o $@ $<

test-prob3 : prob3 test-setup test_treemap_verify
	./testy -o md test_treemap.org $(testnum) 

################################################################################
# Testing Targets
test : test-prob1 test-prob2 test-prob3

test-setup:
	@chmod u+x testy

clean-tests :
	rm -rf test-results


