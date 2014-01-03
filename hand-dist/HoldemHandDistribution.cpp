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

#include "HandDistributions.h"
#include <inlines/eval.h>

#include "HoldemHandDistribution.h"
#include "HoldemAgnosticHand.h"
#include "CardConverter.h"
#include "mtrand.h"

///////////////////////////////////////////////////////////////////////////////
// Default constructor for HoldemHandDistribution objects. No-op.
///////////////////////////////////////////////////////////////////////////////
HoldemHandDistribution::HoldemHandDistribution(void)
{

}



///////////////////////////////////////////////////////////////////////////////
// Single-paramter constructor. Initialize the HoldemHandDistribution with a
// Hold'em hand ("AhKh") or a hand range/distribution ("A2s+,22+").
///////////////////////////////////////////////////////////////////////////////
HoldemHandDistribution::HoldemHandDistribution(const char* hand)
{
	Init(hand);
}



///////////////////////////////////////////////////////////////////////////////
// As above, but allow the client to pass in dead cards. These cards will be
// excluded from whatever distribution we create.
///////////////////////////////////////////////////////////////////////////////
HoldemHandDistribution::HoldemHandDistribution(const char* hand, StdDeck_CardMask deadCards)
{
	Init(hand, deadCards);
}



///////////////////////////////////////////////////////////////////////////////
// Destructor for HoldemHandDistribution objects. No-op.
///////////////////////////////////////////////////////////////////////////////
HoldemHandDistribution::~HoldemHandDistribution(void)
{

}



///////////////////////////////////////////////////////////////////////////////
// Initialize the HoldemHandDistribution with a specific hand ("AhKh") or
// a hand range/distribution ("A2s+,QQ+,JTs-87s").
///////////////////////////////////////////////////////////////////////////////
int HoldemHandDistribution::Init(const char* hand)
{
	StdDeck_CardMask dead;
	StdDeck_CardMask_RESET(dead);

	return Init(hand, dead);
}



///////////////////////////////////////////////////////////////////////////////
// Same as above, but allow the client to pass in a mask of dead cards to 
// exclude from the distribution.
///////////////////////////////////////////////////////////////////////////////
int HoldemHandDistribution::Init(const char* hand, StdDeck_CardMask deadCards)
{
	m_handText = hand;

	char* handCopy = strdup(hand);

	char* pElem = strtok(handCopy, ",");
	while (pElem != NULL)
	{
	  if (HoldemAgnosticHand::Parse(pElem, deadCards)) {
	    if (HoldemAgnosticHand::IsSpecificHand(pElem))
	      {
		m_current = CardConverter::TextToPokerEval(pElem);
		m_hands.push_back(m_current);
		
	      }
	    else
	      {
		HoldemAgnosticHand::Instantiate(pElem, deadCards, m_hands);
	      }
	  }
	  else {
	    printf("Could not parse: %s\n", pElem);
	  }
	  pElem = strtok(NULL, ",");
	}

	free(handCopy);

	// Now we need to remove duplicate elements from the array

	std::sort( m_hands.begin(), m_hands.end(), CardMaskGreaterThan );
	std::vector<StdDeck_CardMask>::iterator new_end_pos;
	new_end_pos = std::unique( m_hands.begin(), m_hands.end(), CardMaskEqual );
	m_hands.erase( new_end_pos, m_hands.end() );

	return m_hands.size();
}




///////////////////////////////////////////////////////////////////////////////
// A distribution is a collection of 1 or more specific hands. This function
// randomly selects and returns one specific hand from the distribution,
// making sure the chosen hand doesn't collide with any of the dead cards.
// In other words, if some other hand "chose" the AsKs, this distribution
// shouldn't be allowed to "choose" any hand containing the As or the Ks.
///////////////////////////////////////////////////////////////////////////////
StdDeck_CardMask HoldemHandDistribution::Choose(StdDeck_CardMask deadCards, bool& bCollisionError)
{
	if (IsUnary())
		return m_current;

	MTRand53 rand;
	int handCount = m_hands.size();
	bCollisionError = false;

	// This is a little bit of a hack. So this HoldemHandDistribution has N potential
	// hands and we'll choose one of these randomly for each trial. Fine. But it's
	// possible that some of these N hands are now impossible, because of the cards
	// chosen for other distributions. This won't usually happen, but it can happen.
	// So we loop. If the first/second/etc. hand we pick is "collided", we just try
	// again. This is messier than, but quicker than, sculpting each hand's distribution
	// to take into account the cards used by prior distributions. It's the "throw
	// a dart at the dartboard" approach and 99% of the time it will work fine...

	for (int attempt = 0; attempt < 10; attempt++)
	{
		int randVal = rand.under(handCount);
		StdDeck_CardMask randHand = m_hands[randVal];

		if (!StdDeck_CardMask_ANY_SET(randHand, deadCards))
		{
			m_current = randHand;
			return m_current;
		}
	}

	// ...until the 1% of the time when this HoldemHandDistributions specific
	// hands are all impossible/blocked by other distributions. You can see how
	// this might happen if you gave player 1, 2, 3, and 4 the "AQs+" range and
	// then gave player 5 the "KK+" range. If players 1 through 4 are each dealt
	// (by chance) an AK, then player 5's distribution is blocked. All the cards
	// are used elsewhere. In this case, since it happens so rarely, we want to
	// throw the entire trial out.

	bCollisionError = true;
	StdDeck_CardMask nullHand;
	StdDeck_CardMask_RESET(nullHand);
	return nullHand;
}



///////////////////////////////////////////////////////////////////////////////
// Used by std::sort when we sort the distribution prior to removing duplicate
// hands from the distribution.
///////////////////////////////////////////////////////////////////////////////
bool HoldemHandDistribution::CardMaskGreaterThan( StdDeck_CardMask a, StdDeck_CardMask b )
{
	return a.cards_n < b.cards_n;
}



///////////////////////////////////////////////////////////////////////////////
// Used by std::unique when we remove duplicate hands (if any) from the 
// distribution.
///////////////////////////////////////////////////////////////////////////////
bool HoldemHandDistribution::CardMaskEqual( StdDeck_CardMask a, StdDeck_CardMask b )
{
	return StdDeck_CardMask_EQUAL(a, b);
}
