../lib/.libs/libpoker-dist.so: AgnosticHand.o Card.o CardConverter.o HoldemCalculator.o HoldemHandDistribution.o mtrand.o
	g++ -shared -Wl,-soname,libpoker-dist.so.1 -o ../lib/.libs/libpoker-dist.so.1.0.0 AgnosticHand.o Card.o CardConverter.o HoldemCalculator.o HoldemHandDistribution.o mtrand.o
	 (cd "../lib/.libs" && rm -f "libpoker-dist.so.1" && ln -s "libpoker-dist.so.1.0.0" "libpoker-dist.so.1")
	 (cd "../lib/.libs" && rm -f "libpoker-dist.so" && ln -s "libpoker-dist.so.1.0.0" "libpoker-dist.so")

AgnosticHand.o: AgnosticHand.cpp AgnosticHand.h Card.h HandDistributions.h
	g++ -g -O0 -std=c++11 -Wall -fPIC -I ../include -c AgnosticHand.cpp

Card.o: Card.cpp Card.h HandDistributions.h
	g++ -g -O0 -std=c++11 -Wall -fPIC -I ../include -c Card.cpp

CardConverter.o: CardConverter.cpp CardConverter.h HandDistributions.h
	g++ -g -O0 -std=c++11 -Wall -fPIC -I ../include -c CardConverter.cpp

HoldemCalculator.o: HoldemCalculator.cpp HoldemCalculator.h HoldemHandDistribution.h CardConverter.h HandDistributions.h
	g++ -g -O0 -std=c++11 -Wall -fPIC -I ../include -c HoldemCalculator.cpp

HoldemHandDistribution.o: HoldemHandDistribution.cpp HoldemHandDistribution.h AgnosticHand.h CardConverter.h mtrand.h HandDistributions.h
	g++ -g -O0 -std=c++11 -Wall -fPIC -I ../include -c HoldemHandDistribution.cpp

mtrand.o: mtrand.cpp mtrand.h
	g++ -g -O0 -std=c++11 -Wall -fPIC -I ../include -c mtrand.cpp

clean:
	rm *.o ../lib/.libs/libpoker-dist*.*
