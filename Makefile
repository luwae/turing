CXX=g++
CPPFLAGS=-Wall

CCR_SRCS=tape.cpp machine.cpp lex.cpp parse.cpp turing.cpp
CCR_OBJS_DIR=$(patsubst %.cpp,src/concrete/%.o,$(CCR_SRCS))

concrete: $(CCR_OBJS_DIR)
	$(CXX) $(CPPFLAGS) -o turing $^

clean:
	rm src/concrete/*.o
	rm turing

.PHONY: clean
