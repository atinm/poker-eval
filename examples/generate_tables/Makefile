all: generate_table test

generate_table: generate_table.o mtrand.o pokerlib.o
	g++ -o generate_table -g -O2 generate_table.o mtrand.o pokerlib.o

test: test.cpp
	g++ -o test test.cpp

mtrand.o: mtrand.cpp mtrand.h
	g++ -c -g -O2 -c mtrand.cpp

pokerlib.o: pokerlib.cpp pokerlib.h poker.h arrays.h
	g++ -c -g -O2 pokerlib.cpp

generate_table.o: generate_table.cpp poker.h pokerlib.h
	g++ -c -g -O2 generate_table.cpp

clean:
	rm *.o generate_table HandRanks.dat
