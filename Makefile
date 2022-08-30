CXX=g++
CPPFLAGS=-Wall

TM_SRCS=tape.cpp machine.cpp
TM_OBJS_P=$(patsubst %.cpp,src/concrete/%.o,$(TM_SRCS))

TMLANG_SRCS=lex.cpp parse.cpp
TMLANG_OBJS_P=$(patsubst %.cpp,src/concrete/tmlang/%.o,$(TMLANG_SRCS))

ATM_SRCS=amachine.cpp
ATM_OBJS_P=$(patsubst %.cpp,src/abstract/%.o,$(ATM_SRCS))

ATMLANG_SRCS=lex.cpp parse.cpp 
ATMLANG_OBJS_P=$(patsubst %.cpp,src/abstract/atmlang/%.o,$(ATMLANG_SRCS))

all: tmrun atm2tm

tmrun: $(TM_OBJS_P) $(TMLANG_OBJS_P) src/concrete/tmrun.o
	$(CXX) $(CPPFLAGS) -o tmrun $^

atm2tm: $(ATM_OBJS_P) $(ATMLANG_OBJS_P) src/abstract/atm2tm.o
	$(CXX) $(CPPFLAGS) -o atm2tm $^

clean:
	rm -f src/concrete/*.o
	rm -f src/concrete/tmlang/*.o
	rm -f src/abstract/*.o
	rm -f src/abstract/atmlang/*.o
	rm -f tmrun
	rm -f atm2tm

.PHONY: clean
