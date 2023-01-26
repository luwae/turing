CC=gcc
CFLAGS=-c -g -Wall -Wextra

SDIR=src
ODIR=build

CONCRETE_SRCS = branch_vec prim_vec state_vec uchar_vec
CONCRETE_SRCS += branch execution machine primitive state tape
CONCRETE_POBJS = $(patsubst %,$(ODIR)/concrete/%.o,$(CONCRETE_SRCS))
UTIL_LEX_SRCS = lex helper turing_helper
UTIL_LEX_POBJS = $(patsubst %,$(ODIR)/util/lex/%.o,$(UTIL_LEX_SRCS))
TMLANG_SRCS = lex
TMLANG_POBJS = $(patsubst %,$(ODIR)/tmlang/%.o,$(TMLANG_SRCS))

TEST = tmlang_lex_test

all:

test: $(TEST)

$(ODIR):
	mkdir -p $(ODIR)/concrete $(ODIR)/tmlang $(ODIR)/util/lex

tmlang_lex_test: $(ODIR)/tmlang/lex_test.o $(TMLANG_POBJS) $(UTIL_LEX_POBJS)
	$(CC) -o $@ $(ODIR)/tmlang/lex_test.o $(TMLANG_POBJS) $(UTIL_LEX_POBJS)

$(ODIR)/concrete/%.o: $(SDIR)/concrete/%.c $(ODIR)
	$(CC) $(CFLAGS) -o $@ $< -I $(SDIR)

$(ODIR)/tmlang/%.o: $(SDIR)/tmlang/%.c $(ODIR)
	$(CC) $(CFLAGS) -o $@ $< -I $(SDIR)

$(ODIR)/util/lex/%.o: $(SDIR)/util/lex/%.c $(ODIR)
	$(CC) $(CFLAGS) -o $@ $< -I $(SDIR)

clean:
	rm -rf $(ODIR) $(TEST)

.PHONY: all test clean
