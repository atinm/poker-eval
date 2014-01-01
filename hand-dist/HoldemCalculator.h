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

#pragma once

#include <inlines/eval.h>

class HoldemHandDistribution;

///////////////////////////////////////////////////////////////////////////////
//
// HoldemCalculator
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
//			Player 1:	[AhKh]
//			Player 2:	[Td9s]
//			Player 3:	[QQ+,AQs+,AQo+]
//			Player 4:	[JJ-88]
//			Player 5:	random
//			Player 6:	random
//			Player 7:	random
//			Board:		Ks7d4d (flop)
//
//		Write code similar to the following...
//
//			double results[10];
//			HoldemEquityCalculator calc;
//			calc.Calculate("AhKh|Td9s|QQ+,AQs+,AQo+|JJ-88|XxXx|XxXx|XxXx", "Ks7d4d", "", 100000, results);
//
//		Use the | symbol to separate players. Use the comman (,) to separate parts of a single player's
//		range.
//
//		You can express each player's hand using either specific or ranged hand hand notation. All of 
//		the following are valid player hand values.
//
//			- "2d2c"				A specific hand: the 2d and the 2c.
//			- "QQ+"					Any pair of Queens, Kings, or Aces
//			- "A3s"					Any A3 of the same suit.
//			- "ATo+"				Any ATo, AJo, AQo or AKo
//			- "TT-88"				Any pair of 88s, 99s, or TTs.
//			- "54"					Any 54 suited OR offsuit
//			- "ATs+,AQo+,KQs,22+"	Any suited AT through AK OR any offsuit AQ through AK OR
//									any KQ suited OR any pair (22s or better).
//
//		To indicate a random player hand, use "XxXx".
//
//		'results' will now contain the equity, expressed as a value between 0.0 and 1.0, for each player in the hand.
//		The player equities appear in the same order as player hands were passed into the function.
//
//			dResults[0] = 37.1	  (player 1 - AhAs)
//			dResults[1] =  5.5	  (player 2 - Td9s)
//			dResults[2] = 19.2	  (player 3 - QQ+,AQs+,AQo+)
//			dResults[3] =  5.9	  (player 4 - JJ-88)
//			dResults[4] = 10.7 	  (player 5 - random)
//			dResults[5] = 10.7 	  (player 6 - random)
//			dResults[6] = 10.7	  (player 7 - random)
//			dResults[7] =  0.0	  (unused)
//			dResults[8] =  0.0	  (unused)
//			dResults[9] =  0.0	  (unused)
//
//		When Monte Carlo is used, the equity values may deviate (slightly).
//
///////////////////////////////////////////////////////////////////////////////

class HoldemCalculator
{
public:
	HoldemCalculator();
	virtual ~HoldemCalculator();

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
	void	Store(const char* hands, const char* board, const char* dead, int trialCount, int* combos, double* outResults);
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
	vector<HoldemHandDistribution*> m_dists;
	double m_wins[23];
	int m_handVals[23];
	int m_tiedPlayerIndexes[23];
	bool m_wasMonteCarlo;
};
