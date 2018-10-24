#
# Makefile for a CS 101 lab or programming assignment.
# Do not modify this file!!!
#

SUBMIT_URL = https://cs.ycp.edu/marmoset/bluej/SubmitProjectViaBlueJSubmitter
COURSE_NAME = CS 101
SEMESTER = Spring 2018
PROJECT_NUMBER = assign05

# Some unknown software on the KEC Windows load
# sets the GCC_EXEC_PREFIX environment variable, which
# prevents Cygwin's gcc/g++ from working correctly.
# Make sure it is unset when we invoke g++.
CXX = unset GCC_EXEC_PREFIX && g++

NCURSES_DIR := $(shell if [ -r /usr/include/ncurses/ncurses.h ]; then echo "yes"; fi)

ifeq ($(NCURSES_DIR),yes)
CXXFLAGS_EXTRA := -DCONS_NCURSES_IS_IN_NCURSES_INCLUDE_DIR
endif

OSNAME = $(shell uname -o)
ifeq ($(OSNAME),Cygwin)
LDFLAGS = -Wl,--enable-auto-import
endif

CXXFLAGS = -g -Wall -I./include $(CXXFLAGS_EXTRA)

SRC = Snake.cpp lib/Console.cpp
OBJ = $(SRC:.cpp=.o)
EXE = $(SRC:.cpp=.exe)

$(EXE) : $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS) -lncurses

submit : submit.properties solution.zip
	perl submitToMarmoset.pl solution.zip submit.properties

solution.zip : collected-files.txt
	@echo "Creating a solution zip file"
	@zip -9 $@ `cat collected-files.txt`
	@rm -f collected-files.txt

# Create the submit.properties file that describes how
# the project should be uploaded to the Marmoset server.
submit.properties : nonexistent
	@echo "Creating submit.properties file"
	@rm -f $@
	@echo "submitURL = $(SUBMIT_URL)" >> $@
	@echo "courseName = $(COURSE_NAME)" >> $@
	@echo "semester = $(SEMESTER)" >> $@
	@echo "projectNumber = $(PROJECT_NUMBER)" >> $@

# Collect the names of all files that don't appear
# to be generated files.
collected-files.txt :
	@echo "Collecting the names of files to be submitted"
	@rm -f $@
	@find . -not \( \
				-name '*\.o' \
				-or -name '*\.exe' \
				-or -name '*~' \
				-or -name 'collected-files.txt' \
				-or -name 'submit.properties' \
				-or -type d \
			\) -print \
		| perl -ne 's,\./,,; print' \
		> $@

# This is just a dummy target to force other targets
# to always be out of date.
nonexistent :

# Remove generated files.
clean : 
	rm -f *.o lib/*.o *.exe collected-files.txt submit.properties solution.zip
