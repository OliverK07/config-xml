VERBOSE =
CC = gcc
AR = ar

UARCH = $(shell uname -m)
ifeq "$(UARCH)" "x86_64"
UARCH =
CFLAGS = -m64
LFLAGS = -m64
endif

CFLAGS += -std=c++11 -O

LIBDIR = -L.
LIBS = -lstdc++ -lpthread -lxerces-c

INCLUDES = -I.

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %cpp,%o, $(SRCS))
BINNAME = demo



.PHONY: all clean
.SUFFIXES: .cpp .o

all: $(BINNAME)

%.o: %.cpp
	$(VERBOSE)$(CC) $(CFLAGS) $(INCLUDES) -c $<

$(BINNAME): $(OBJS)
	$(VERBOSE)$(CC) $(LFLAGS) $(OBJS) $(LIBDIR) $(LIBS) -o $(BINNAME)

clean:
	$(VERBOSE)rm -rf $(OBJS) $(BINNAME)
