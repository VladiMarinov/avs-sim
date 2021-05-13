exec = avs-sim
sources = $(wildcard src/*.cpp)
objects = $(sources:.cpp=.o)
CC = g++
flags = -g -Wall -Wpedantic -Wextra

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

