BOOSTDIR='/usr/include'
f=main.cpp

compile: $(f)
	g++ $(f) -o main -O3 -std=c++0x -fno-strict-aliasing -I $(BOOSTDIR)

valgrind:
	valgrind ./main

clean:
	rm -f main
run:
	./main
