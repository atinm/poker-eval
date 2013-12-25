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


//////////////////////////////////////////////////////////////////////////////
// Helper class, used to convert cards between various formats. For example,
// to convert a textual representation of a card into its Pokersource mask
// value.
//////////////////////////////////////////////////////////////////////////////
class CardConverter
{
public:


	//////////////////////////////////////////////////////////////////////////////
	// Given a string such as "AcKcQcJcTc" representing a poker hand, return a 
	// poker-eval StdDeck_CardMask representing that hand.
	//////////////////////////////////////////////////////////////////////////////
	static StdDeck_CardMask TextToPokerEval(const char* strHand)
	{
		StdDeck_CardMask theHand, theCard;
		StdDeck_CardMask_RESET(theHand);

		if (strHand && strlen(strHand))
		{
			int cardIndex = -1;
			char* curCard = const_cast<char*>(strHand);

			while (*curCard)
			{
				// Take the card text and convert it to an index (0..51)
				StdDeck_stringToCard(curCard, &cardIndex);
				// Convert the card index to a mask
				theCard = StdDeck_MASK(cardIndex);
				// Add the card (mask) to the hand
				StdDeck_CardMask_OR(theHand, theHand, theCard);
				// Advance to the next card (if any)
				curCard += 2;
			}
		}

		return theHand;
	}


	//////////////////////////////////////////////////////////////////////////////
	// Given a string such as "AcKcQcJcTc" representing a poker hand, fill an
	// array with the PokerEval mask value of each card.
	//////////////////////////////////////////////////////////////////////////////
	static int TextToPokerEvalArray(const char* strHand, StdDeck_CardMask* pArray)
	{
		int numCards = 0;
		StdDeck_CardMask mask;
		StdDeck_CardMask_RESET(mask);
		if (strHand != NULL && pArray != NULL)
		{
			int cardIndex = -1;
			int arrayIndex = 0;

			for (const char* pChar = strHand; *pChar != '\0'; pChar += 2, numCards++)
			{
				
				// First, take the string and convert it to a card index.
				StdDeck_stringToCard(const_cast<char*>(pChar), &cardIndex);
				
				// Then take the card index and convert it to a mask
				StdDeck_CardMask_OR(mask, mask, StdDeck_MASK(cardIndex));

				// Lastly store the mask in the array
				if (numCards % 2 != 0)
				{
					pArray[arrayIndex] = mask;
					arrayIndex++;
				}
			}
		}

		return numCards;
	}



	//////////////////////////////////////////////////////////////////////////////
	// Convert a card from PokerTracker to poker-eval format.
	//////////////////////////////////////////////////////////////////////////////
	static StdDeck_CardMask PokerTrackerToPokerEval(int nPokerTrackerCardId)
	{
		return PokerEvalCards[nPokerTrackerCardId];
	}

private:
	CardConverter(void) { }

	static StdDeck_CardMask PokerEvalCards[53];
};
