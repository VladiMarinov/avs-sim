#Finds OS Name
uname_S := $(shell uname -s)

exec = avs-sim
sources = $(wildcard src/*.cpp)
objects = $(sources:.cpp=.o)

# Set CC to clang++ for MACOS, g++ for Windows/Linux
ifeq ($(uname_S), Darwin)
    CC = clang++
else
	 	CC = g++
endif

flags = -g -Wall -Wpedantic -Wextra -std=c++17

$(exec): $(objects)
	$(CC) $(objects) $(flags) -o $(exec)

%.o: %.cpp %.h
	$(CC) -c $(flags) $< -o $@

clean:
	-rm avs-sim
	-rm *.o
	-rm src/*.o
	make

remove:
	-rm avs-sim 
	-rm *.o
	-rm src/*.o

run:
	@make -s 
	@./avs-sim examples/example1.avs

rv:
	make clean 
	@./avs-sim examples/example1.avs

compiler_info:
	@echo "Makefile autodetected environment as " $(uname_S)"... Using " $(CC) 

.PHONY: docs
docs:
	@doxygen Doxyfile
	@echo "Docs generated successfully under /doc.."

.PHONY: cloc
cloc:
	@cloc . --exclude-dir='Eigen','doc'
