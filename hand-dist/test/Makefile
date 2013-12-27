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

all: HoldemEquityTest OmahaEquityTest

HoldemEquityTest: HoldemEquityTest.cpp
#ifeq ($(UNAME),Darwin)
#	g++ -o HoldemEquityTest HoldemEquityTest.cpp -g -O0 -std=c++11 -Wall -fPIC -fpermissive -I.. -I../../include ../../lib/.libs/libpoker-dist.dylib ../../lib/.libs/libpoker-eval.dylib -lm -lc
#else
	g++ -o HoldemEquityTest HoldemEquityTest.cpp -g -O0 -std=c++11 -Wall -fPIC -fpermissive -I.. -I../../include -L../../lib/.libs -lpoker-dist -lpoker-eval -lm -lc
#endif
OmahaEquityTest: OmahaEquityTest.cpp
	g++ -o OmahaEquityTest OmahaEquityTest.cpp -g -O0 -std=c++11 -Wall -fPIC -fpermissive -I.. -I../../include -L../../lib/.libs -lpoker-dist -lpoker-eval -lm -lc

clean:
	rm HoldemEquityTest OmahaEquityTest
