# 
#
# 
# 

# Variables.
CXX ?= g++
CXXFLAGS = -MMD -MP -I. -std=c++14 -Wextra -Wpedantic -Wall -O3 -DNDEBUG
LDLIBS = -lboost_system -lpthread -lssl -lcrypto

# Using all cpp files in current directory.
MAIN = ../bin/EfOVRP
LIB = ../lib/libEfOVRP.a
SRC = $(wildcard *.cpp)\
			$(wildcard ./algorithms/*.cpp)\
			$(wildcard ./algorithms/*/*.cpp)\
			$(wildcard ./routing/*.cpp)\
			$(wildcard ./problems/*.cpp)\
			$(wildcard ./problems/*/*.cpp)\
			$(wildcard ./problems/*/*/*.cpp)\
			$(wildcard ./structures/generic/*.cpp)\
			$(wildcard ./structures/EfOVRP/*.cpp)\
			$(wildcard ./structures/EfOVRP/input/*.cpp)\
			$(wildcard ./structures/EfOVRP/solution/*.cpp)\
			$(wildcard ./structures/*.cpp)\
			$(wildcard ./utils/*.cpp)

# Checking for libosrm
ifeq ($(shell pkg-config --exists libosrm && echo 1),1)
LDLIBS += $(shell pkg-config --libs libosrm) -lboost_filesystem -lboost_iostreams -lboost_thread -lrt -ltbb
CXXFLAGS += $(shell pkg-config --cflags libosrm) -D USE_LIBOSRM=true
else
#	SRC := $(filter-out ./routing/libosrm_wrapper.cpp, $(SRC)) homsick
endif

OBJ = $(SRC:.cpp=.o)
DEPS = $(SRC:.cpp=.d)

# Main target.
all : $(MAIN) $(LIB)

$(MAIN) : $(OBJ) main.o
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

$(LIB) : $(OBJ)
	mkdir -p $(@D)
	$(AR) cr $@ $^

# Building .o files.
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include ${DEPS}

clean :
	$(RM) $(OBJ) $(DEPS)
	$(RM) $(MAIN)
	$(RM) $(LIB)
