make: 
	@g++ src/main.cpp -o avs-sim

run:
	@make -s
	@./avs-sim

