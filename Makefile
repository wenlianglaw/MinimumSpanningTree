FILE="mst"

all:	make run 

make:
	@g++ $(FILE).cpp -o $(FILE) -std=c++14 -O2 

run:
	@./$(FILE)

clean:
	@rm ./$(FILE)
