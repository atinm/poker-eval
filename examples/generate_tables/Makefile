all: generate_table generate_pokersource_table generate_pokersource_omahalo_table test

generate_table: generate_table.o mtrand.o pokerlib.o
	g++ -o generate_table -g -O2 generate_table.o mtrand.o pokerlib.o

generate_pokersource_table: generate_pokersource_table.o mtrand.o pokerlib.o
	g++ -o generate_pokersource_table -g -O2 generate_pokersource_table.o mtrand.o pokerlib.o

generate_pokersource_omahalo_table: generate_pokersource_omahalo_table.o mtrand.o pokerlib.o
	g++ -o generate_pokersource_omahalo_table -g -O2 generate_pokersource_omahalo_table.o mtrand.o pokerlib.o

mtrand.o: mtrand.cpp mtrand.h
	g++ -c -g -O2 -c mtrand.cpp

pokerlib.o: pokerlib.cpp pokerlib.h poker.h arrays.h
	g++ -c -g -O2 pokerlib.cpp

generate_table.o: generate_table.cpp poker.h pokerlib.h
	g++ -c -g -O2 generate_table.cpp

generate_pokersource_table.o: generate_pokersource_table.cpp poker.h pokerlib.h
	g++ -c -g -O2 generate_pokersource_table.cpp

generate_pokersource_omahalo_table.o: generate_pokersource_omahalo_table.cpp poker.h pokerlib.h
	g++ -c -g -O2 generate_pokersource_omahalo_table.cpp

clean:
	rm *.o generate_table generate_pokersource_table generate_pokersource_omahalo_table HandRanks.dat
