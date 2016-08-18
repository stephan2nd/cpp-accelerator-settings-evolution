# Makefile for accelerator-settings-evolution

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
 CC := g++
endif
ifeq ($(UNAME_S),Darwin)
 CC := llvm-g++
endif

CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS  := -L../cpp-accelerator-toolkit/lib -L../cpp-evolution-toolkit/lib
LD_LIBS   := -lacctool -levotools
CC_FLAGS  := -Wall -std=c++11 -pedantic -pthread -I../cpp-accelerator-toolkit/src -I../cpp-evolution-toolkit/src -O3 # -g (for debugging)

app.exe: $(OBJ_FILES) 
	$(CC) $(CC_FLAGS) $^ -o $@ $(LD_FLAGS) $(LD_LIBS)  

all: app.exe 
	@echo "done"

obj/%.o: src/%.cpp src/%.hpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

clean:
	@rm -f obj/*.o *.exe *.o *.stackdump
	@echo project cleaned
