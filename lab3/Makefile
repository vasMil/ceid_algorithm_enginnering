BOOSTDIR='/usr/include'
f=src/main.cpp

compile: $(f)
	g++ $(f) -o main -O3 -std=c++0x -fno-strict-aliasing -I $(BOOSTDIR)

valgrind:
	valgrind --leak-check=full --track-origins=yes ./main

clean:
	rm -f main
run:
	./main
