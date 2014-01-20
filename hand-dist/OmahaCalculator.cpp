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

#include "HandDistributions.h"
#include <inlines/eval_omaha.h>

#include "OmahaCalculator.h"
#include "OmahaHandDistribution.h"
#include "OmahaAgnosticHand.h"
#include "CardConverter.h"



///////////////////////////////////////////////////////////////////////////////
// Default constructor for OmahaCalculator objects.
///////////////////////////////////////////////////////////////////////////////
OmahaCalculator::OmahaCalculator(bool omahaHiLo)
{
  m_MonteCarloThreshhold = 20000000;
  m_omahaHiLo = omahaHiLo;
}



///////////////////////////////////////////////////////////////////////////////
// Default destructor for OmahaCalculator objects.
///////////////////////////////////////////////////////////////////////////////
OmahaCalculator::~OmahaCalculator(void)
{
}



///////////////////////////////////////////////////////////////////////////////
// Calculate equities for the specified matchup using either Monte Carlo
// or Exhaustive Enumeration.
///////////////////////////////////////////////////////////////////////////////
int OmahaCalculator::Calculate(const char* hands, const char* board, const char* dead, int64_t trialCount, int *outCombos, double* outResults)
{
  PreCalculate(hands, board, dead, trialCount, outCombos, outResults);

  if (EstimatePossibleOutcomes() > m_MonteCarloThreshhold)
    CalculateMonteCarlo();
  else
    CalculateExhaustive();

  return PostCalculate();
}



///////////////////////////////////////////////////////////////////////////////
// Calculate equities for the specified matchup using Monte Carlo. ALWAYS
// uses Monte Carlo, even if the number of possible combinations is low.
///////////////////////////////////////////////////////////////////////////////
int OmahaCalculator::CalculateMC(const char* hands, const char* board, const char* dead, int64_t numberOfTrials, int* combos, double* results)
{
  PreCalculate(hands, board, dead, numberOfTrials, combos, results);
  CalculateMonteCarlo();
  return PostCalculate();
}



///////////////////////////////////////////////////////////////////////////////
// Calculate equities for the specified matchup using Exhaustive Enumeration.
// ALWAYS uses exhaustive enumeration, even if the number of possible combos
// is very large.
///////////////////////////////////////////////////////////////////////////////
int OmahaCalculator::CalculateEE(const char* hands, const char* board, const char* dead, int* combos, double* results)
{
  PreCalculate(hands, board, dead, 0, combos, results);
  CalculateExhaustive();
  return PostCalculate();
}



///////////////////////////////////////////////////////////////////////////////
// Internal helper method. Perform setup prior to running the calculation.
///////////////////////////////////////////////////////////////////////////////
void OmahaCalculator::PreCalculate(const char* hands, const char* board, const char* dead, int numberOfTrials, int* combos, double* results)
{
  TRACE("\n\n************************************************************\n"
	"* CALCULATING MATCHUP: Board = [%s]\n"
	"************************************************************\n",
	(board && strlen(board) > 0) ? board : "PREFLOP" );

  Reset();
  Store(hands, board, dead, numberOfTrials, combos, results);
  CreateHandDistributions(hands);
  if (numberOfTrials == 0)
    EstimatePossibleOutcomes();
}


///////////////////////////////////////////////////////////////////////////////
// Reset the state of the calculator, setting various internal fields to their
// default values.
///////////////////////////////////////////////////////////////////////////////
void OmahaCalculator::Reset()
{
  StdDeck_CardMask_RESET(m_boardMask);
  StdDeck_CardMask_RESET(m_deadMask);
  StdDeck_CardMask_RESET(m_deadMaskDyn);
  m_numberOfBoardCards = 0;
  m_numberOfRangedHands = 0;
  m_numberOfSpecificHands = 0;
  m_collisions = 0;
  m_totalHands = 0;
  m_possibleOutcomes = 1L;
  m_pResults = NULL;
  m_pCombos = NULL;
  m_indicatedTrials = 0L;
  m_actualTrials = 0L;
  memset(m_tiedPlayerIndexes, 0, sizeof(m_tiedPlayerIndexes));
  memset(m_tiedLowPlayerIndexes, 0, sizeof(m_tiedLowPlayerIndexes));
  memset(m_wins, 0, sizeof(m_wins));
}



///////////////////////////////////////////////////////////////////////////////
// Store the various pieces of information passed in by the user. We store these
// as member variables so we don't have to pass a lot of junk between internal
// functions.
///////////////////////////////////////////////////////////////////////////////
void OmahaCalculator::Store(const char* hands, const char* board, const char* dead, int trialCount, int* outCombos, double* outResults)
{
  // Convert board cards and dead cards to Pokersource mask format
  m_boardMask = CardConverter::TextToPokerEval(board);
  m_deadMask = CardConverter::TextToPokerEval(dead);
  StdDeck_CardMask_OR(m_deadMask, m_deadMask, m_boardMask);

  // Tuck this away for later
  m_pResults = outResults;
  m_pCombos = outCombos;
  m_indicatedTrials = trialCount;

  // Get the number of board cards that were supplied. For Hold'em, should be 0, 3, 4 or 5.
  m_numberOfBoardCards = StdDeck_numCards(m_boardMask);
}



///////////////////////////////////////////////////////////////////////////////
// Take the specified group of player hands, which may be either specific hands
// or hand distributions, and create a OmahaHandDistribution object for each.
// Doing this is a little tricky/tedious, since the hand text can be formatted
// in so many different ways.
///////////////////////////////////////////////////////////////////////////////
int OmahaCalculator::CreateHandDistributions(const char* hands)
{
  // Make a copy of the input string of hands. We need to do this in order to tokenize.
  char* handsCopy = strdup(hands);

  // Split the input string containing player hands into individual strings
  // and store them temporarily in a list, prior to creating OmahaHandDistribution
  // objects below. We have to do it this way because OmahaHandDistribution
  // calls strtok internally, and nested strtok calls are problematic.
  list<char*> playerHandsColl;
  char* pElem = strtok(handsCopy, "|");
  while (pElem)
    {
      playerHandsColl.push_back( strdup(pElem) );
      pElem = strtok(NULL, "|");
      m_totalHands++;
    }

  // Now we can free the copied string...
  free(handsCopy);

  // Create a mask which we'll use to store all KNOWN/SPECIFIC player hands.
  StdDeck_CardMask staticPlayerCards;
  StdDeck_CardMask_RESET(staticPlayerCards);


  // Iterate over the supplied player hands, ignoring any ranged/random hands
  // for now, focusing only on the SPECIFIC/KNOWN hands that were specified.
  m_dists.resize(m_totalHands);
  list<char*>::const_iterator end = playerHandsColl.end();
  list<char*>::const_iterator iter = playerHandsColl.begin();
  for (int index = 0; iter != end; iter++, index++)
    {
      if (OmahaAgnosticHand::IsSpecificHand(*iter))
	{
	  OmahaHandDistribution* pCur = new OmahaHandDistribution(*iter, m_deadMask);
	  m_dists[index] = pCur;
	  bool bUnused;
	  StdDeck_CardMask theHand = pCur->Choose(m_deadMask, bUnused);
	  StdDeck_CardMask_OR(staticPlayerCards, staticPlayerCards, theHand);
	  //m_hands[index] = theHand;
	  m_numberOfSpecificHands++;
	}
    }

  // Add the static/known player cards to the dead mask
  StdDeck_CardMask_OR(m_deadMask, m_deadMask, staticPlayerCards);

  // Iterate over the supplied player hands, focusing on ranged/random hands.
  // We already handled specific/known hands above.
  iter = playerHandsColl.begin();
  for (int index = 0; iter != end; iter++, index++)
    {
      if (!OmahaAgnosticHand::IsSpecificHand(*iter))
	{
	  OmahaHandDistribution* pCur = new OmahaHandDistribution(*iter, m_deadMask);
	  m_dists[index] = pCur;
	  m_numberOfRangedHands++;
	}
    }

  // Lastly, we strdup'd a bunch of string above. Free them.
	
  for (iter = playerHandsColl.begin(); iter != end; iter++)
    free(*iter);

  m_deadMaskDyn = m_deadMask;

  ASSERT(m_numberOfRangedHands + m_numberOfSpecificHands == m_totalHands);

  return m_totalHands;
}



///////////////////////////////////////////////////////////////////////////////
// Enumerate all possible boards for the given scenario, passing each board
// to the evaluator. This function is called in 2 cases:
//
//		- If every player involved in the matchup has a specific, known hand.
//
//		- If one or more players have hand ranges/distributions, and exhaustive
//		  enumeration is used, this function is called after each player has
//		  been dealt a specific hand.
//
///////////////////////////////////////////////////////////////////////////////
int OmahaCalculator::CalculateExhaustiveBoards()
{
  StdDeck_CardMask missingBoardCards;
  StdDeck_CardMask_RESET(missingBoardCards);

  if (m_numberOfBoardCards == 0)
    ENUMERATE_5_CARDS_D(missingBoardCards, m_deadMaskDyn, EvalOneTrial(missingBoardCards, m_totalHands); );
  else if (m_numberOfBoardCards == 3)
    ENUMERATE_2_CARDS_D(missingBoardCards, m_deadMaskDyn, EvalOneTrial(missingBoardCards, m_totalHands); );
  else if (m_numberOfBoardCards == 4)
    ENUMERATE_1_CARDS_D(missingBoardCards, m_deadMaskDyn, EvalOneTrial(missingBoardCards, m_totalHands); );
  else if (m_numberOfBoardCards == 5)
    EvalOneTrial(missingBoardCards, m_totalHands);

  return m_totalHands;
}



///////////////////////////////////////////////////////////////////////////////
// Run a single trial, evaluating all hands and determining the winner. Called
// whether we're calculating equity via Monte Carlo or exhaustive enumeration.
///////////////////////////////////////////////////////////////////////////////
void OmahaCalculator::EvalOneTrial
(
 // This parameter contains the randomly-sampled or -enumerated remainder of the
 // board. For example, if the flop is AsKsQs as above, this parameter will contain
 // the current randomly-sampled-or-enumerated turn and river cards.
 StdDeck_CardMask boardFragment,

 // Number of players (total)
 int playerCount
 )
{
  m_actualTrials++;

  HandVal besthi = HandVal_NOTHING;
  LowHandVal bestlo = LowHandVal_NOTHING;
  int bestHiIndex = -1;
  int bestLoIndex = -1;
  HandVal curhi = HandVal_NOTHING;
  LowHandVal curlo = LowHandVal_NOTHING;
  bool isTie = false;
  bool isLowTie = false;
  int numTies = 0, numLowTies = 0;

  // Combine the base board (eg, flop of "AsKsQs") with the sampled fragment
  // (eg, turn and river of "2s5d"). After this call, 'boardFragment' will
  // contain the entire board (all 5 cards).
	
  StdDeck_CardMask_OR(boardFragment, boardFragment, m_boardMask);

  // Evaluate each players full 7-card hand in turn...

  for (int i = 0; i < playerCount; i++) {
    // Evaluate the resulting hand...curhi and curlo are HandVals we can compare to
    // the value of other hands in order to determine a winner. Use HiLow8_EVAL
    // as it can do both depending on whether the loval parameter is passed.
    int ret = StdDeck_OmahaHiLow8_EVAL_LUT(m_dists[i]->Current(), boardFragment, &curhi, m_omahaHiLo ? &curlo : NULL);
    if (ret) {
        printf("Error evaluating OmahaHi, %d\n", ret);
	printf("Hand: ");
	CardMask c = m_dists[i]->Current();
	StdDeck_printMask(c);
	printf(", Board: ");
	StdDeck_printMask(boardFragment);
	printf("\n");
	exit(-1);
      }

      // If this hand is the best we've seen so far, adjust state...
      if (curhi > besthi) {
	besthi = curhi;
	isTie = false;
	numTies = 0;
	bestHiIndex = i;
      }

      // Otherwise, if this hand ties another, adjust some state...
      else if (curhi == besthi)	{
	if (numTies == 0) {
	  m_tiedPlayerIndexes[0] = bestHiIndex;
	  m_tiedPlayerIndexes[1] = i;
	}
	else {
	  m_tiedPlayerIndexes[numTies + 1] = i;
	}

	isTie = true;
	numTies++;
      }

      // Store the player's hand value...
      m_handVals[i] = curhi;

      // If this hand is the best low hand we've seen so far, adjust some state...
      if (curlo != LowHandVal_NOTHING && curlo < bestlo) {
	bestlo = curlo;
	isLowTie = false;
	numLowTies = 0;;
	bestLoIndex = i;
      }

      // Otherwise, if this hand ties another low, add ties
      else if (curlo != LowHandVal_NOTHING && curlo == bestlo) {
	if (numLowTies == 0) {
	  m_tiedLowPlayerIndexes[0] = bestLoIndex;
	  m_tiedLowPlayerIndexes[1] = i;
	}
	else {
	  m_tiedPlayerIndexes[numLowTies + 1] = i;
	}

	isLowTie = true;
	numLowTies++;
      }

      // Store the player's low hand value...
      m_lowHandVals[i] = curlo;
  }

  // max part of pot any player can win
  float maxPotSize = 1.0f;
  if (bestlo != LowHandVal_NOTHING)
    maxPotSize = 0.5f; // split pot

  // divide up the high half
  if (!isTie) {
    // besthi gets the maxPotSize for hi side, could be all
    m_wins[bestHiIndex] += maxPotSize;
  }
  else {
    // split the high half between ties
    double partialHiWin = maxPotSize / ((double)numTies + 1.0f);
    for (int i = 0; i <= numTies; i++)
      m_wins[ m_tiedPlayerIndexes[i] ] += partialHiWin;
  }

  // if there is a low side of the pot, divide the low half
  if (bestlo != LowHandVal_NOTHING) {
    if (!isLowTie) {
      // bestlo gets half the pot
      m_wins[bestLoIndex] += maxPotSize;
    }
    else {
      // split the low half between ties
      double partialLoWin = maxPotSize / ((double)numLowTies + 1.0f);
      for (int i = 0; i <= numLowTies; i++)
	m_wins[ m_tiedLowPlayerIndexes[i] ] += partialLoWin;
    }
  }
}



///////////////////////////////////////////////////////////////////////////////
// Once the simulation has been run, this little helper function takes each
// player's win counts and converts these into equity/win percentage.
///////////////////////////////////////////////////////////////////////////////
int64_t OmahaCalculator::PostCalculate()
{
  int totalPlayers = m_dists.size();
  memset(m_pResults, 0, sizeof(double) * totalPlayers);
  memset(m_pCombos, 0, sizeof(int) * totalPlayers);
  for (int r = 0; r < totalPlayers; r++)
    {
      m_pResults[r] = (m_wins[r] / m_actualTrials) * 100.0;
      m_pCombos[r] = m_dists[r]->GetCount();

      TRACE("Player %2d:   %7d   %5.2f%%    \"%s\"\n", r+1, m_pCombos[r], m_pResults[r], m_dists[r]->GetText());
    }

  TRACE("\nRan %llu trials via %s.\n", m_actualTrials, m_wasMonteCarlo ? "Monte Carlo" : "exhaustive enumeration");

  return m_actualTrials;
}



///////////////////////////////////////////////////////////////////////////////
// Return true if this is a "deterministic" situation, meaning a matchup in
// which every player has a specific 2-card holding.
///////////////////////////////////////////////////////////////////////////////
bool OmahaCalculator::IsDeterministic(void)
{
  return (m_numberOfRangedHands == 0);
}



///////////////////////////////////////////////////////////////////////////////
// Package our HandDistribution objects into a linked list so that we can
// choose hands in a round-robin fashion, reducing the likelihood of bias
// from collisions between various hand ranges/distributions.
///////////////////////////////////////////////////////////////////////////////
void OmahaCalculator::LinkHandDistributions(void)
{
  // Set up the circular linked list
  int handCount = m_dists.size();
  for (int i = 0; i < handCount; i++)
    {
      if (i > 0)
	m_dists[i-1]->m_pNext = m_dists[i];
    }
  m_dists[handCount-1]->m_pNext = m_dists[0];
}



///////////////////////////////////////////////////////////////////////////////
// Exhaustively enumerate all possible outcomes.
///////////////////////////////////////////////////////////////////////////////
int OmahaCalculator::CalculateExhaustiveRecurse(int playerIndex, StdDeck_CardMask deadCur)
{
  StdDeck_CardMask curHand;
  StdDeck_CardMask deadCurThisLevel;

  OmahaHandDistribution* pDist = m_dists[playerIndex];
  int handCount = pDist->GetCount();
  for (int h = 0; h < handCount; h++)
    {
      deadCurThisLevel = deadCur;

      curHand = pDist->Get(h);
      if (!StdDeck_CardMask_ANY_SET(deadCurThisLevel, curHand) || pDist->IsUnary())
	{
	  pDist->SetCurrent(curHand);
	  StdDeck_CardMask_OR(deadCurThisLevel, deadCurThisLevel, curHand);
	}
      else
	continue;

      if (playerIndex < (m_totalHands - 1))
	{
	  CalculateExhaustiveRecurse(playerIndex + 1, deadCurThisLevel);
	}
      else
	{
	  m_deadMaskDyn = deadCurThisLevel;
	  CalculateExhaustiveBoards();
	}
    }

  return m_actualTrials;
}



///////////////////////////////////////////////////////////////////////////////
// Exhaustively enumerate all possible outcomes.
///////////////////////////////////////////////////////////////////////////////
int OmahaCalculator::CalculateExhaustive(void)
{
  m_wasMonteCarlo = false;
  return CalculateExhaustiveRecurse(0, m_deadMask);
}



///////////////////////////////////////////////////////////////////////////////
// Here's where we loop a million or so times, evaluating one battle with each
// iteration.
///////////////////////////////////////////////////////////////////////////////
int OmahaCalculator::CalculateMonteCarlo(void)
{
  m_wasMonteCarlo = true;

  LinkHandDistributions();

  StdDeck_CardMask cardCombo;
  StdDeck_CardMask usedCardsThisTrial;
  OmahaHandDistribution* pFirstToDraw = m_dists[0];
  OmahaHandDistribution* pDist;

  // Now run however many trials
  while ( m_actualTrials < m_indicatedTrials )
    {
      // Reset "used cards"
      usedCardsThisTrial = m_deadMask;

      bool bCollisionError = false;

      // Figure out which player we'll deal to first
      pDist = pFirstToDraw;

      // For each player in the hand, choose his hand. If the player has a specific hand,
      // great, use that one. Otherwise pick one hand from his distribution randomly.
      do
	{
	  pDist->Choose(usedCardsThisTrial, bCollisionError);
	  if (bCollisionError)
	    {
	      m_collisions++;
	      break;
	    }

	  if (!pDist->IsUnary())
	    {
	      // Add the chosen player hand to the dead/used cards...
	      StdDeck_CardMask_OR(usedCardsThisTrial, usedCardsThisTrial, pDist->Current());
	    }

	  pDist = pDist->Next();

	} while (pDist != pFirstToDraw);

      if (bCollisionError)
	continue;

      // Now generate a single random board
      DECK_MONTECARLO_N_CARDS_D(StdDeck, cardCombo, usedCardsThisTrial, 
				5 - m_numberOfBoardCards, 1, this->EvalOneTrial(cardCombo, m_totalHands); );

      pFirstToDraw = pDist->Next();
    }

  return m_actualTrials;
}



///////////////////////////////////////////////////////////////////////////////
// Perform a quick and dirty estimate of the number of possible outcomes,
// given all combinations of player hands and board cards. This figure is
// not exact, because it doesn't take into account that when you choose
// a hand from one player's distribution, that can reduce the number of
// possible hands in another player's distribution. In fact, this function
// needs a lot of work...HOWEVER...when the opponent's distributions are
// wide, it shouldn't matter so much and when they're not, it's likely
// that we can exhaustively enumerate anyway.
///////////////////////////////////////////////////////////////////////////////
uint64_t OmahaCalculator::EstimatePossibleOutcomes()
{
  uint64_t last = 1;
  uint64_t total = 1;

  for (size_t hand = 0; hand < m_dists.size(); hand++)
    {
      total *= m_dists[hand]->GetCount();
      if (last > total) // overflow
	return UINT64_MAX;
      last = total;
    }

  total *= CalculateCombinations( (52 - (m_totalHands * 2)) - m_numberOfBoardCards, 5 - m_numberOfBoardCards);

  //TRACE("Estimated combinations: %lld\n", total);

  return (total < last) ? UINT64_MAX :	total;
}



///////////////////////////////////////////////////////////////////////////////
// Simple algorithm to calculate combinations of N things taken R at a time.
// The only place this is used is to estimate how many possible outcomes
// we're dealing with in situations involving exhaustive enumeration.
///////////////////////////////////////////////////////////////////////////////
uint64_t OmahaCalculator::CalculateCombinations(int N, int R)
{
  uint64_t answer = 1;
  int multiplier = N;
  int divisor = 1;
  int k = min(N, N - R);

  if (R == 0) // choose none
    return answer;

  while (divisor <= k) {
    answer = (answer * multiplier) / divisor;
    multiplier--;
    divisor++;
  }

  return answer;
}



///////////////////////////////////////////////////////////////////////////////
// SET the "Monte Carlo threshhold", the number of possible outcomes (taking
// into account all possible player cards and board cards) above which we
// use Monte Carlo instead of exhaustive enumeration. Note: only 
// OmahaCalculator::Calculate observes this value. OmahaCalculator::CalculateMC
// and OmahaCalculator::CalculateEE ignore it.
///////////////////////////////////////////////////////////////////////////////
void OmahaCalculator::SetMCThreshhold(uint64_t t)
{
  m_MonteCarloThreshhold = t;
}


///////////////////////////////////////////////////////////////////////////////
// GET the "Monte Carlo threshhold", the number of possible outcomes (taking
// into account all possible player cards and board cards) above which we
// use Monte Carlo instead of exhaustive enumeration. Note: only 
// OmahaCalculator::Calculate observes this value. OmahaCalculator::CalculateMC
// and OmahaCalculator::CalculateEE ignore it.
///////////////////////////////////////////////////////////////////////////////
uint64_t OmahaCalculator::GetMCThreshhold() const
{ 
  return m_MonteCarloThreshhold;
}
