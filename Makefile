# Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
# GNU General Public License v3.0, see LICENSE file.
# CLI/Makefile
#

SHELL := /bin/sh

ifndef CXX
	export CXX:=g++
endif

CFlags  := -std=c++11
archive  := libCLI.a
inc      := CLI/
lib      := lib/
IFlags   := -I.
LFlags   := -I. -L$(lib) -lCLI

object_names := SingleMode.o MultiMode.o
objects      := $(addprefix $(lib), $(object_names))

deps := Argument Required Default Switch Flag List \
	IO Exceptions Utilities ArgManager \
	SingleMode MultiMode

headers := $(addprefix $(inc), $(addsuffix .hpp, $(deps)))

example_names := hello calc
examples      := $(addprefix examples/, $(example_names))


all: $(archive) $(examples)



$(archive): $(objects)
	ar -rv $(lib)$@ $(objects)
	ranlib $(lib)$@


$(lib)%.o: $(lib)%.cpp $(inc)%.hpp
	$(CXX) -c $< -o $@ $(IFlags) $(CFlags)


examples/%: examples/%.cpp $(headers)
	$(CXX) -o $@.o -c $< $(IFlags) $(CFlags)
	$(CXX) -o $@.exe $@.o $(LFlags) $(CFlags)


.PHONY: clean
clean:
	rm -rf $(objects)
	rm -rf $(lib)$(archive)

.PHONY: veryclean
veryclean: clean
	rm -rf $(addsuffix .o, $(examples))
	rm -rf $(addsuffix .exe, $(examples))


# test run for TravisCI
.PHONY: test
test:
	@./examples/hello.exe "" || exit 1
