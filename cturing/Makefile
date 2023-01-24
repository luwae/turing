CC=gcc
CFLAGS=-c -g -Wall -Wextra

SDIR=src
ODIR=build

CONCRETE_SRCS = branch_vec prim_vec state_vec uchar_vec
CONCRETE_SRCS += branch execution machine primitive state tape
CONCRETE_POBJS = $(patsubst %,$(ODIR)/concrete/%.o,$(CONCRETE_SRCS))
LEX_UTIL_SRCS = lex lex_helper
LEX_UTIL_POBJS = $(patsubst %,$(ODIR)/util/%.o,$(LEX_UTIL_SRCS))
TMLANG_SRCS = lex
TMLANG_POBJS = $(patsubst %,$(ODIR)/tmlang/%.o,$(TMLANG_SRCS))

TEST = tmlang_lex_test

all:

test: $(TEST)

$(ODIR):
	mkdir -p $(ODIR)/concrete $(ODIR)/tmlang $(ODIR)/util

tmlang_lex_test: $(ODIR) $(ODIR)/tmlang/lex_test.o $(TMLANG_POBJS) $(LEX_UTIL_POBJS)
	$(CC) -o $@ $(ODIR)/tmlang/lex_test.o $(TMLANG_POBJS) $(LEX_UTIL_POBJS)

$(ODIR)/concrete/%.o: $(SDIR)/concrete/%.c
	$(CC) $(CFLAGS) -o $@ $< -I $(SDIR)

$(ODIR)/tmlang/%.o: $(SDIR)/tmlang/%.c
	$(CC) $(CFLAGS) -o $@ $< -I $(SDIR)

$(ODIR)/util/%.o: $(SDIR)/util/%.c
	$(CC) $(CFLAGS) -o $@ $< -I $(SDIR)

clean:
	rm -rf $(ODIR) $(TEST)

.PHONY: all $(TEST)
