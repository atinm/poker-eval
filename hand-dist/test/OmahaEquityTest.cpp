///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009 James Devlin
// Copyright (c) 2013 Atin Malaviya
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
#include <inlines/eval.h>
#include "HandDistributions.h"
#include "OmahaCalculator.h"

int errorCount = 0;

void performMatchup(const char* hands, const char* board, const char* dead, bool hilo, int numberOfTrials, bool withExhaustive, double *expected);

int main(int argc, char* argv[])
{
  // First, let's demonstrate how you'd perform a simple calculation

  //double results[7];
  //OmahaCalculator calc;
  //calc.Calculate("AhKhQhJh|XxXxXxXx|XxXxXxXx|XxXxXxXx", "Ks7d4d", "", 100000, results);

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
    // 9dTsJsQs vs 4dAh2s3c on 8d7h6s9h6c board, Omaha Hi/Lo split: Monte-Carlo, Enumerated should be split 50%
    double expected[] = { 50.0, 50.0 };
    performMatchup("9dTsJsQs|4dAh2s3c", "8d7h6s9h6c", NULL, true, numberOfTrials, true, expected);
  }

  {
    // 4s5d2h3c vs 4dAh2c3s on 3d2dh6s5h6c board, Omaha Hi/Lo split: Monte-Carlo, Enumerated should be split 50%
    double expected[] = { 50.0, 50.0 };
    performMatchup("4s5d2h3c|4dAh2c3s", "3d2d5h6h5c", NULL, true, numberOfTrials, true, expected);
  }

  {
    // AAxx vs. KKxx of any suits: Monte Carlo
    double expected[] = { 69.87, 30.13 };
    performMatchup("AAXX|KKXX", NULL, NULL, false, numberOfTrials, false, expected);
  }

  {
    // [AX]xx vs. [KX]xx top suited, any others: Monte Carlo
    double expected[] = { 52.92, 47.08 };
    performMatchup("[AX]XX|[KX]XX", NULL, NULL, false, numberOfTrials, false, expected);
  }

  {
    // [QB][QB] vs. [JB][JB]: pair of Qs and suited big cards vs pair of Js and suited bi. Monte Carlo
    double expected[] = { 57.17, 42.83 };
    performMatchup("[QB][QB]|[JB][JB]", NULL, NULL, false, numberOfTrials, false, expected);
  }

  {
    // Q+T+XX vs. 6+2+XX: Monte Carlo
    double expected[] = { 54.05, 45.96 };
    performMatchup("Q+T+XX|6+2+XX", NULL, NULL, false, numberOfTrials, false, expected);
  }

  {
    // Q:0:0:2 vs. Q:2:0:0: Monte Carlo
    double expected[] = { 59.19, 40.81 };
    performMatchup("Q:0:0:2|Q:2:0:0", NULL, NULL, false, numberOfTrials, false, expected);
  }

  {
    // BBXX vs. XXXX: Monte Carlo
    double expected[] = { 55.08, 44.92 };
    performMatchup("BBXX|XXXX", NULL, NULL, false, numberOfTrials, false, expected);
  }

  {
    // XXXX/sna vs. [K-2K-2]XX: Monte Carlo
    double expected[] = { 50.0, 50.0 };
    performMatchup("[K-2k-2]XX|XXXX/sna", NULL, NULL, false, numberOfTrials, false, expected);
  }

  printf("\n\nAll tests concluded with %d errors.", errorCount);
  printf("\nPress any character to exit.\n");
  getchar();

  return 0;
}



void performMatchup(const char* hands, const char* board, const char* dead, bool hilo, int numberOfTrials, bool withExhaustive, double *expected)
{
  int combos[23];
  double results[23];
  memset(combos, 0, sizeof(combos));
  memset(results, 0, sizeof(results));

  int numberOfHands = std::count(hands, hands + strlen(hands), '|') + 1;

  // First, let's run the matchup using Monte Carlo...

  OmahaCalculator calc(hilo);

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
	  //		d) An obscure bug in Omaha Equilab (probably not)
	  //
	  
	  if (expected[h] > results[h] + 1.0 || expected[h] < results[h] - 1.0)
	    {
	      printf("Expected[%d]: %2.2f, Result[%d]: %2.2f\n", h, expected[h], h, results[h]);
	      errorCount++;
	      ASSERT(0);
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
	  //		d) An obscure bug in Omaha Equilab (probably not)
	  //
			
	  if (expected[h] > results[h] + 1.0 || expected[h] < results[h] - 1.0)
	    {
	      printf("Expected[%d]: %2.2f, Result[%d]: %2.2f\n", h, expected[h], h, results[h]);
	      errorCount++;
	      ASSERT(0);
	    }
	}
    }
}

