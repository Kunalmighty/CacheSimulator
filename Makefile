CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra 
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

cache: cache.o
	g++ -o cache cache.o

cache.o: cache.cpp
	g++ -c cache.cpp $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o cache
