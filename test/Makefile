# Compile commands
ifeq ($(shell clang > /dev/null 2> /dev/null; echo $$?), $(shell echo 1))
export CC = clang
else
export CC = gcc
endif
ifeq ($(shell clang++ > /dev/null 2> /dev/null; echo $$?), $(shell echo 1))
export CXX = clang++
export LINK = clang++
else
export CXX = g++
export LINK = g++
endif

export CFLAGS = -Wall -Wextra -Wabi -Weffc++ -Wshadow -Wfloat-equal -Wno-unused-result -O3 -c
export CXXFLAGS = -Wall -Wextra -Wabi -Weffc++ -Wshadow -Wfloat-equal -Wno-unused-result -O3 -c
export LFLAGS = -Wall -Wextra -Wabi -Weffc++ -Wshadow -Wfloat-equal -Wno-unused-result -s -O3

# Shell commands
export MV = mv -f
export CP = cp -f -r
export UPDATE = $(CP) -u
export RM = rm -f
export MKDIR = mkdir -p

all: kmp spr gen aho

kmp: kmp.cpp
	$(LINK) $< $(LFLAGS) -o $@

spr: spr.cpp
	$(LINK) $< $(LFLAGS) -o $@

gen: gen.cpp
	$(LINK) $< $(LFLAGS) -o $@

aho: aho.cpp
	$(LINK) $< $(LFLAGS) -o $@
