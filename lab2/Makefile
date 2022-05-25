BOOSTDIR='/usr/include'
Leda = '/usr/local/LEDA/incl'
Ledalib = '/usr/local/LEDA'
LIBFLAGS= -lleda -lm
f=src/main.cpp

compile: $(f)
	g++ $(f) -o main -O3 -std=c++0x -fno-strict-aliasing -I $(BOOSTDIR) -I $(Leda) -L $(Ledalib) $(LIBFLAGS)

valgrind:
	valgrind --leak-check=full --track-origins=yes ./main

clean:
	rm -f main
run:
	./main
