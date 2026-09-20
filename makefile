PROG = bin/exec
TESTPROG = bin/tests
CC = g++
CPPFLAGS = -O3 -gdwarf-3 -W -Wall -pedantic -std=c++17
MAIN = main.o

OBJS = graph.o graph_generator.o  benchmark.o graphio.o random.o  rcl.o
TESTOBJECTS = test_base.o test_graph.o test_graph_generator.o test_benchmark.o test_graphio.o 

main: $(MAIN) $(OBJS)
	$(CC) $(MAIN) $(OBJS) -o $(PROG)
	mv *.o build/

test: $(TESTOBJECTS) $(OBJS)
	$(CC) $(CPPFLAGS) $^ -o $(TESTPROG)
	mv *.o build/

test_base.o:
	$(CC) $(CPPFLAGS) -c tests/test_base.cpp

test_graph.o: include/data_structures/graph.h
	$(CC) $(CPPFLAGS) -c tests/test_graph.cpp

test_graph_generator.o: include/benchmark/graph_generator.h
	$(CC) $(CPPFLAGS) -c tests/test_graph_generator.cpp

test_benchmark.o: include/benchmark/benchmark.h
	$(CC) $(CPPFLAGS) -c tests/test_benchmark.cpp

test_graphio.o: include/helpers/graphio.h
	$(CC) $(CPPFLAGS) -c tests/test_graphio.cpp

main.o:
	$(CC) $(CPPFLAGS) -c src/main.cpp

graph.o: include/data_structures/graph.h
	$(CC) $(CPPFLAGS) -c src/data_structures/graph.cpp

graph_generator.o: include/benchmark/graph_generator.h
	$(CC) $(CPPFLAGS) -c src/benchmark/graph_generator.cpp

random.o: include/helpers/random.h
	$(CC) $(CPPFLAGS) -c src/helpers/random.cpp

benchmark.o: include/benchmark/benchmark.h
	$(CC) $(CPPFLAGS) -c src/benchmark/benchmark.cpp

graphio.o: include/helpers/graphio.h
	$(CC) $(CPPFLAGS) -c src/helpers/graphio.cpp

rcl.o: include/algorithms/rcl.h
	$(CC) $(CPPFLAGS) -c src/algorithms/rcl.cpp

clean:
	rm -f ./bin/exec
	rm -f ./bin/tests
	rm -f ./*.o
	rm -f "!(parameters|instances).txt"
	rm -f ./*.csv
	rm -f ./*.stdout
	rm -f ./*.stderr
