///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009 James Devlin
//
// DISCLAIMER OF WARRANTY
//
// This source code is provided "as is" and without warranties as to performance
// or merchantability. The author and/or distributors of this source code may 
// have made statements about this source code. Any such statements do not 
// constitute warranties and shall not be relied on by the user in deciding 
// whether to use this source code.
//
// This source code is provided without any express or implied warranties 
// whatsoever. Because of the diversity of conditions and hardware under which
// this source code may be used, no warranty of fitness for a particular purpose
// is offered. The user is advised to test the source code thoroughly before 
// relying on it. The user must assume the entire risk of using the source code.
//
///////////////////////////////////////////////////////////////////////////////
#include <unistd.h>
#include "HandDistributions.h"
#include "HoldemCalculator.h"

int errorCount = 0;

void performMatchup(const char* hands, const char* board, const char* dead, int numberOfTrials, bool withExhaustive, double *expected);

int main(int argc, char* argv[])
{
	// First, let's demonstrate how you'd perform a simple calculation

	//double results[7];
	//HoldemCalculator calc;
	//calc.Calculate("AhKh|Td9s|QQ+,AQs+,AQo+|JJ-88|XxXx|XxXx|XxXx", "Ks7d4d", "", 100000, results);

	// Now let's run some sample matchups. The 'expected' array contains the correct equities
	// for this matchup as returned by PokerStove/PokerCalculator. The performMatchup() function
	// (see below) will run the matchup and check the equities returned against the expected values,
	// throwing an assertion and incrementing the global 'errorCount' variable if the results
	// deviate by more than 1%.

	int numberOfTrials = 1000000;

	FILE* f = fopen("HandRanks.dat", "rb");
	if (f) {
	  int fd = fileno(f);
	  int myfd = dup(fd);
	  StdDeck_Initialize_LUT(myfd, 0);
	  fclose(f);
	}

	{
		// Typical 3-way matchup - pocket pair vs. loose-ish vs. tight-ish
	  double expected[] = { 39.07, 19.5, 41.42 };
	  performMatchup("TsTc|AXs|QQ+,AQ+", NULL, NULL, numberOfTrials, false, expected);
	}

	{
		// 7-way matchup involving 2 specific hands, 2 ranged hands, and 3 random hands, on the flop
		double expected[] = { 37.1, 5.5, 19.2, 5.9, 10.75, 10.75, 10.75  };
		performMatchup("AhKh|Td9s|QQ+,AQs+,AQo+|JJ-88|XxXx|XxXx|XxXx", "Ks7d4d", "", 1000000, false, expected);
	}

	{
		// AA vs. KK of any suits: Monte Carlo + Exhaustive
		double expected[] = { 81.9, 18.1 };
		performMatchup("AA|KK", NULL, NULL, numberOfTrials, true, expected);
	}


	{
		// AA vs. KK of any suits with flop [Th Jc Qs]: Monte Carlo + Exhaustive
		double expected[] = { 75.9, 24.1 };
		performMatchup("AA|KK", "ThJcQs", NULL, numberOfTrials, true, expected);
	}


	{
		// 10-way preflop all-in with AA through 55: Monte Carlo
		double expected[] = { 24.3, 18.8, 14.1, 10.6, 8.1, 6.0, 4.7, 4.2, 4.5, 4.8};
		performMatchup("AA|KK|QQ|JJ|TT|99|88|77|66|55", NULL, NULL, numberOfTrials, false, expected);
	}


	{
		// 10-way preflop all-in with AA through 55 with flop of [2c3s4d]: Monte Carlo
		double expected[] = { 13.5, 10.6, 9.6, 8.6, 7.6, 6.7, 5.7, 5.7, 12.6, 19.5};
		performMatchup("AA|KK|QQ|JJ|TT|99|88|77|66|55", "2c3s4d", NULL, numberOfTrials, false, expected);
	}

	{
		// Aces vs. Kings with shared suits
		double expected[] = { 82.6, 17.4 };
		performMatchup("AsAc|KsKc", NULL, NULL, numberOfTrials, true, expected);
	}

	{
		// Specific hands and hand ranges in the same trial
		double expected[] = { 30.8, 15.5, 38.1, 15.6 };
		performMatchup("AhKh|2d2c|77|A2s+", NULL, NULL, numberOfTrials, false, expected);
	}

	{
		// Aces versus 5 random/uknown hands
		double expected[] = { 82.6, 17.4 };
		performMatchup("AcAs,KcKs|QcQs", NULL, NULL, numberOfTrials, true, expected);
	}


	{
		// Aces versus 5 random/uknown hands
		double expected[] = { 49.2, 10.15, 10.15, 10.15, 10.15, 10.15 };
		performMatchup("AA|XxXx|XxXx|XxXx|XxXx|XxXx", NULL, NULL, numberOfTrials, false, expected);
	}


	{
		// Typical 3-way matchup - pocket pair vs. loose-ish vs. tight-ish
		double expected[] = { 37.1, 19.5, 43.4 };
		performMatchup("TsTc|A2+,22+|QQ+,AQ+", NULL, NULL, numberOfTrials, false, expected);
	}

	printf("\n\nAll tests concluded with %d errors.", errorCount);
	printf("\nPress any character to exit.\n");
	getchar();

	return 0;
}



void performMatchup(const char* hands, const char* board, const char* dead, int numberOfTrials, bool withExhaustive, double *expected)
{
  int combos[23];
	double results[23];
  memset(combos, 0, sizeof(combos));
	memset(results, 0, sizeof(results));

	int numberOfHands = std::count(hands, hands + strlen(hands), '|') + 1;

	// First, let's run the matchup using Monte Carlo...

	HoldemCalculator calc;
	calc.CalculateMC(hands, board, dead, numberOfTrials, combos, results);

	if (expected)
	{
		for (int h = 0; h < numberOfHands; h++)
		{
			// If this assert is triggered, then the results returned by the above
			// equity calculation didn't match the results returned by PokerStove.
			// This might happen for four reasons:
			//
			//		a) The calculation wasn't set up properly
			//		b) The calculation was Monte Carlo'd with a low number of trials
			//		c) There's a bug in the equity calculation logic.
			//		d) An obscure bug in PokerStove (probably not)
			//
			
			if (expected[h] > results[h] + 1.0 || expected[h] < results[h] - 1.0)
			{
			  printf("Expected[%d]: %2.2f, Result[%d]: %2.2f\n", h, expected[h], h, results[h]);
				errorCount++;
				//ASSERT(0);
			}
		}
	}

	if (!withExhaustive)
		return;

	// Then let's run it with Exhaustive Enumeration...

	calc.CalculateEE(hands, board, dead, combos, results);

	if (expected)
	{
		for (int h = 0; h < numberOfHands; h++)
		{
			// If this assert is triggered, then the results returned by the above
			// equity calculation didn't match the results returned by PokerStove.
			// This might happen for four reasons:
			//
			//		a) The calculation wasn't set up properly
			//		b) The calculation was run with a low number of trials
			//		c) There's a bug in the equity calculation logic.
			//		d) An obscure bug in PokerStove (probably not)
			//
			
			if (expected[h] > results[h] + 1.0 || expected[h] < results[h] - 1.0)
			{
			  printf("Expected[%d]: %2.2f, Result[%d]: %2.2f\n", h, expected[h], h, results[h]);
				errorCount++;
				//ASSERT(0);
			}
		}
	}
}

