################################################################################
#
# Copyright (c) 2013 Atin Malaviya
#
# DISCLAIMER OF WARRANTY
#
# This source code is provided "as is" and without warranties as to performance
# or merchantability. The author and/or distributors of this source code may 
# have made statements about this source code. Any such statements do not 
# constitute warranties and shall not be relied on by the user in deciding 
# whether to use this source code.
#
# This source code is provided without any express or implied warranties 
# whatsoever. Because of the diversity of conditions and hardware under which
# this source code may be used, no warranty of fitness for a particular purpose
# is offered. The user is advised to test the source code thoroughly before 
# relying on it. The user must assume the entire risk of using the source code.
#
#################################################################################
UNAME := $(shell uname)
../lib/.libs/libpoker-dist.so: HoldemAgnosticHand.o Card.o CardConverter.o HoldemCalculator.o HoldemHandDistribution.o OmahaAgnosticHand.o OmahaCalculator.o OmahaHandDistribution.o mtrand.o
ifeq ($(UNAME),Darwin)
	g++ -dynamiclib -compatibility_version 2 -current_version 2.0 -Wl,-single_module -install_name libpoker-dist.1.dylib -o ../lib/.libs/libpoker-dist.1.dylib HoldemAgnosticHand.o Card.o CardConverter.o HoldemCalculator.o HoldemHandDistribution.o OmahaAgnosticHand.o OmahaCalculator.o OmahaHandDistribution.o mtrand.o ../lib/.libs/libpoker-eval.dylib
	 (cd "../lib/.libs" && rm -f "libpoker-dist.dylib" && ln -s "libpoker-dist.1.dylib" "libpoker-dist.dylib")
else
	g++ -shared -Wl,-soname,libpoker-dist.so.1 -o ../lib/.libs/libpoker-dist.so.1.0.0 HoldemAgnosticHand.o Card.o CardConverter.o HoldemCalculator.o HoldemHandDistribution.o OmahaAgnosticHand.o OmahaCalculator.o OmahaHandDistribution.o mtrand.o
	 (cd "../lib/.libs" && rm -f "libpoker-dist.so.1" && ln -s "libpoker-dist.so.1.0.0" "libpoker-dist.so.1")
	 (cd "../lib/.libs" && rm -f "libpoker-dist.so" && ln -s "libpoker-dist.so.1.0.0" "libpoker-dist.so")
endif

Card.o: Card.cpp Card.h HandDistributions.h
	g++ -g -O0 -std=c++11 -Wall -fPIC -fpermissive -I ../include -c Card.cpp

CardConverter.o: CardConverter.cpp CardConverter.h HandDistributions.h
	g++ -g -O0 -std=c++11 -Wall -fPIC -fpermissive -I ../include -c CardConverter.cpp

HoldemAgnosticHand.o: HoldemAgnosticHand.cpp HoldemAgnosticHand.h Card.h HandDistributions.h
	g++ -g -O0 -std=c++11 -Wall -fPIC -fpermissive -I ../include -c HoldemAgnosticHand.cpp

HoldemCalculator.o: HoldemCalculator.cpp HoldemCalculator.h HoldemHandDistribution.h CardConverter.h HandDistributions.h
	g++ -g -O0 -std=c++11 -Wall -fPIC -fpermissive -I ../include -c HoldemCalculator.cpp

HoldemHandDistribution.o: HoldemHandDistribution.cpp HoldemHandDistribution.h HoldemAgnosticHand.h CardConverter.h mtrand.h HandDistributions.h
	g++ -g -O0 -std=c++11 -Wall -fPIC -fpermissive -I ../include -c HoldemHandDistribution.cpp

OmahaAgnosticHand.o: OmahaAgnosticHand.cpp OmahaAgnosticHand.h Card.h HandDistributions.h
	g++ -g -O0 -std=c++11 -Wall -fPIC -fpermissive -I ../include -c OmahaAgnosticHand.cpp

OmahaCalculator.o: OmahaCalculator.cpp OmahaCalculator.h OmahaHandDistribution.h CardConverter.h HandDistributions.h
	g++ -g -O0 -std=c++11 -Wall -fPIC -fpermissive -I ../include -c OmahaCalculator.cpp

OmahaHandDistribution.o: OmahaHandDistribution.cpp OmahaHandDistribution.h HoldemAgnosticHand.h CardConverter.h mtrand.h HandDistributions.h
	g++ -g -O0 -std=c++11 -Wall -fPIC -fpermissive -I ../include -c OmahaHandDistribution.cpp

mtrand.o: mtrand.cpp mtrand.h
	g++ -g -O0 -std=c++11 -Wall -fPIC -fpermissive -I ../include -c mtrand.cpp

clean:
	rm *.o ../lib/.libs/libpoker-dist*.*
