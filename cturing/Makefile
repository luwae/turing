CC=gcc
CFLAGS=-g

SDIR=src
ODIR=build

CONCRETE_SRCS = branch_vec prim_vec state_vec uchar_vec
CONCRETE_SRCS += branch execution machine main primitive state tape
CONCRETE_POBJS = $(patsubst %,$(ODIR)/concrete/%.o,$(CONCRETE_SRCS))

.PHONY: all
all: create_dir concrete

.PHONY: create_dir
create_dir:
	mkdir -p $(ODIR)/concrete

concrete: $(CONCRETE_POBJS)
	$(CC) $(CFLAGS) -o $@ $(CONCRETE_POBJS)

$(ODIR)/concrete/%.o: $(SDIR)/concrete/%.c
	$(CC) $(CFLAGS) -c -o $@ $< -I src/util

.PHONY: clean
clean:
	rm -rf $(ODIR) concrete
