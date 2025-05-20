# CMSC216 Project 4 Makefile
AN = p4
CLASS = 216

CFLAGS = -Wall -Wno-comment -Werror -g 
CC     = gcc $(CFLAGS)
SHELL  = /bin/bash
CWD    = $(shell pwd | sed 's/.*\///g')

PROGRAMS = chester
TEST_PROGRAMS = test_chester

export PARALLEL?=True		#enable parallel testing if not overridden
export LANG=C			#sets locale to avoid unicode quotes

############################################################
# Default target and cleaning target to remove compiled programs/objects
all : $(PROGRAMS) $(TEST_PROGRAMS)


clean :
	rm -f $(PROGRAMS) $(TEST_PROGRAMS) *.o vgcore*

############################################################
# help message to show build targets
help :
	@echo 'Typical usage is:'
	@echo '  > make                          # build all programs'
	@echo '  > make clean                    # remove all compiled items'
	@echo '  > make zip                      # create a zip file for submission'
	@echo '  > make prob1                    # built targets associated with problem 1'
	@echo '  > make test                     # run all tests'
	@echo '  > make test-prob2               # run test for problem 2'
	@echo '  > make test-prob2 testnum=5     # run problem 2 test #5 only'

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
	curl https://www.cs.umd.edu/~profk/216/$(AN)-update.sh | /bin/bash 


############################################################
# chester targets
%.o : %.c chester.h chester_parse.h
	$(CC) -c $<

chester : chester_main.o chester_funcs.o chester_util.o chester_parse.o 
	$(CC) -o $@ $^

test_chester : test_chester.o chester_funcs.o chester_util.o chester_parse.o
	$(CC) -o $@ $^

# test_bake_funcs34 : test_bake_funcs34.o bake_funcs.o bake_util.o
# 	$(CC) -o $@ $^

############################################################
# Testing Targets
test : test-prob1 test-prob2 test-prob3 test-prob4 test-prob5

test-setup:
	@chmod u+x testy

clean-tests :
	rm -rf test-results chester-test*

test-prob1 : test_chester test-setup
	./testy -o md test_prob1.org $(testnum)

test-prob2 : test_chester test-setup
	./testy -o md test_prob2.org $(testnum)

test-prob3 : test_chester test-setup
	./testy -o md test_prob3.org $(testnum)

test-prob4 : test_chester test-setup
	./testy -o md test_prob4.org $(testnum)

test-prob5 : chester test-setup
	./testy -o md test_prob5.org $(testnum)

