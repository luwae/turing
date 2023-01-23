CC=gcc
CFLAGS=-c -g -Wall -Werror -Wextra

SDIR=src
ODIR=build

CONCRETE_SRCS = branch_vec prim_vec state_vec uchar_vec
CONCRETE_SRCS += branch execution machine primitive state tape concrete_test
CONCRETE_POBJS = $(patsubst %,$(ODIR)/concrete/%.o,$(CONCRETE_SRCS))
TMLANG_SRCS = lex lex_test
TMLANG_POBJS = $(patsubst %,$(ODIR)/tmlang/%.o,$(TMLANG_SRCS))


all: create_dir concrete_test lex_test

create_dir:
	mkdir -p $(ODIR)/concrete $(ODIR)/tmlang

concrete_test: $(CONCRETE_POBJS)
	$(CC) -o $@ $^

$(ODIR)/concrete/%.o: $(SDIR)/concrete/%.c
	$(CC) $(CFLAGS) -c -o $@ $< -I $(SDIR)/util

lex_test: $(TMLANG_POBJS)
	$(CC) -o $@ $^

$(ODIR)/tmlang/%.o: $(SDIR)/tmlang/%.c
	$(CC) $(CFLAGS) -c -o $@ $< -I $(SDIR)/util

clean:
	rm -rf $(ODIR) concrete_test lex_test

.PHONY: all create_dir concrete_test lex_test
