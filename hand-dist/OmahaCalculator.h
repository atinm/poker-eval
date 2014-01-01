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

#pragma once

#include <inlines/eval_omaha.h>

class OmahaHandDistribution;

///////////////////////////////////////////////////////////////////////////////
//
// OmahaCalculator
// (c) 2009 by James Devlin
//
// Performs multiway isometric equity computation for Texas Hold'em games. 
// Handles any number of players with known hands, any number of players with 
// unknown (random) hands, at any street (preflop, flop, turn, or river).
//
// Internally, this calculator will use either Monte Carlo simulation or 
// exhaustive exploration as appropriate to analyze hands.
//
// This evaluator uses the native C language version of the Pokersource
// poker-eval evaluator (native C version) for speed.
//
// Usage:
//
//		In order to calculate equity in (for example) the following scenario...
//
//			Player 1:	AhKhXX
//			Player 2:	Td9sXX
//			Player 3:	TTxx,[AQ]xx,AQJT
//			Player 4:	[J-8J-8]xx
//			Player 5:	random
//			Player 6:	random
//			Player 7:	random
//			Board:		Ks7d4d (flop)
//
//		Write code similar to the following...
//
//			double results[10];
//			OmahaEquityCalculator calc;
//			calc.Calculate("AAxx|KKxx|TTxx|[J-8J-8]xx|XxXxXxXx|XxXxXxXx|XxXxXxXx", "Ks7d4d", "", 100000, results);
//
//		Use the | symbol to separate players. Use the comman (,) to separate parts of a single player's
//		range.
//
//		You can express each player's hand using either specific or ranged hand hand notation. All of 
//		the following are ways to specify valid player hand values.
//
//      Single hands
//      To specify a single hand for example type AsQc7h3d
//
//      All combos of a hand
//      To get all 16 combos of a hand, for example an A, K, Q and a J, enter 
//      AKQJ without suits.
//
//      Suits
//      To specify suited cards, enclosing them in square brackets, e.g. [AK]xx will
//      give all hands containing suited AK and two other cards.
//      [AK][AK] will give A's and K's double suited. [JT][98] will give double
//      suited JT98 hands.
//      [XXX]X will give all hands with three of a suit.
//      [X][X][X][X] will give all hands with four different suits (rainbox).
//      To specify off-suit cards, enclose one of the cards in square brackets, e.g.
//      [A]KQJ will give all hands with an A of a different suit from all other cards.
//      [Ah]KQJ will give all hands with an Ah and all other cards of a different suit.
//
//      Pairs
//      QQxx will give all hands containing two queens
//      [QT+][QT+] double suited queens with two other T+ cards.
//
//      Card types
//      x = random, all ranks, 2..A
//      2 .. T J Q K A
//      [] = group suited cards
//
//      Ranges from high to low ranks, e.g. for each of the four cards, you can
//      enter a range, e.g. K-Txxx would give Txxx, Jxxx Qxxx, Kxxx, Axxx.
//
//      Ranges can be combined for all four cards, e.g. 3+9-7Q+K-T where first card is 3 or
//      better, second card is between 7-9, third card is greater than or equal to Q, and
//      the fourth cards is between T and K.
//
//      Percentile (todo)
//      15% would give the top 15% of hands (ProPokerTools ranking).
//      10-25% would give the top 10% to 25% of hands (ProPokerTools ranking).
//
//		To indicate a random player hand, use "XxXxXxXx".
//
//		'results' will now contain the equity, expressed as a value between 0.0 and 1.0, for each player in the hand.
//		The player equities appear in the same order as player hands were passed into the function.
//
//			dResults[0] = 37.1	  (player 1)
//			dResults[1] =  5.5	  (player 2)
//			dResults[2] = 19.2	  (player 3)
//			dResults[3] =  5.9	  (player 4)
//			dResults[4] = 10.7 	  (player 5)
//			dResults[5] = 10.7 	  (player 6)
//			dResults[6] = 10.7	  (player 7)
//			dResults[7] =  0.0	  (unused)
//			dResults[8] =  0.0	  (unused)
//			dResults[9] =  0.0	  (unused)
//
//		When Monte Carlo is used, the equity values may deviate (slightly).
//
///////////////////////////////////////////////////////////////////////////////

class OmahaCalculator
{
public:
	OmahaCalculator(bool omahaHilo = false);
	virtual ~OmahaCalculator();

	// Calculate using either Monte Carlo or exhaustive enumeration.
	int Calculate(const char* hands, const char* board, const char* dead, int64_t numberOfTrials, int* combos, double* results);

	// Calculate using Monte Carlo only.
	int CalculateMC(const char* hands, const char* board, const char* dead, int64_t numberOfTrials, int* combos, double* results);

	// Calculate using exhaustive enumeration only.
	int CalculateEE(const char* hands, const char* board, const char* dead, int* combos, double* results);

	// Get/set the "Monte Carlo" threshhold, the number of outcomes above which
	void SetMCThreshhold(uint64_t t);
	uint64_t GetMCThreshhold() const;

private:

	void	PreCalculate(const char* hands, const char* board, const char* dead, int numberOfTrials, int* combos, double* results);
	int64_t	PostCalculate();

	int		CalculateExhaustive();
	int		CalculateMonteCarlo();
	int		CalculateExhaustiveBoards();
	int		CalculateExhaustiveRecurse(int playerIndex, StdDeck_CardMask deadCur);
	void	EvalOneTrial( StdDeck_CardMask boardFragment, int playerCount);

	void	Reset();
	void	Store(const char* hands, const char* board, const char* dead, int trialCount, int* outCombos, double* outResults);
	int		CreateHandDistributions(const char* hands);
	bool	IsDeterministic();
	void	LinkHandDistributions();
	uint64_t EstimatePossibleOutcomes();
	uint64_t CalculateCombinations(int N, int R);

	StdDeck_CardMask m_boardMask;
	StdDeck_CardMask m_deadMask;
	StdDeck_CardMask m_deadMaskDyn;
	int m_numberOfBoardCards;
	int m_numberOfRangedHands;
	int m_numberOfSpecificHands;
	int m_totalHands;
	int m_collisions;
	double* m_pResults;
	int* m_pCombos;
	uint64_t m_indicatedTrials;
	uint64_t m_actualTrials;
	uint64_t m_possibleOutcomes;
	uint64_t m_MonteCarloThreshhold;
	vector<OmahaHandDistribution*> m_dists;
	double m_wins[23];
	int m_handVals[23];
	int m_lowHandVals[23];
	int m_tiedPlayerIndexes[23];
	int m_tiedLowPlayerIndexes[23];
	bool m_wasMonteCarlo;
	bool m_omahaHiLo;
};
