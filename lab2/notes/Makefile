BOOSTDIR='/usr/include'
fname=nodeInfo_init_nullEdge

compile: $(fname).cpp
	g++ $(fname).cpp -o $(fname) -O3 -std=c++0x -fno-strict-aliasing -I $(BOOSTDIR)

valgrind:
	valgrind ./$(fname)

clean:
	rm -f $(fname)
run:
	./$(fname)