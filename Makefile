
CC = llvm-g++
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS  :=  #-lOpenCL
LD_STATIC_LIBS := ../accelerator-toolkit/lib/accelerator-toolkit.a ../evolution-toolkit/lib/evolution-toolkit.a
CC_FLAGS  := -Wall -std=c++11 -pedantic -I../accelerator-toolkit/src/ -I../evolution-toolkit/src/ -O3 #(optimierung) # -g (for debugging)

app.exe: obj/main.o $(OBJ_FILES) 
	$(CC) $(LD_FLAGS) $(LD_STATIC_LIBS) -o $@ $^

all: app.exe 
	@echo "done"

obj/%.o: src/%.cpp src/%.hpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

.PHONY: objects 

objects: $(OBJ_FILES)
	@echo object files created	

clean:
	@rm -f obj/*.o *.exe *.o *.stackdump
	@echo project cleaned
