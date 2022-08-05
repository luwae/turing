CXX=g++
CPPFLAGS=-Wall

TM_SRCS=tape.cpp machine.cpp
TM_OBJS_P=$(patsubst %.cpp,src/concrete/%.o,$(TM_SRCS))

TMLANG_SRCS=lex.cpp parse.cpp
TMLANG_OBJS_P=$(patsubst %.cpp,src/concrete/tmlang/%.o,$(TMLANG_SRCS))

tmrun: $(TM_OBJS_P) $(TMLANG_OBJS_P) src/concrete/tmrun.o
	$(CXX) $(CPPFLAGS) -o tmrun $^

clean:
	rm -f src/concrete/*.o
	rm -f src/concrete/tmlang/*.o
	rm -f tmrun

.PHONY: clean
