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
#include <inlines/eval.h>
#include "HandDistributions.h"
#include "HoldemAgnosticHand.h"
#include "Card.h"



///////////////////////////////////////////////////////////////////////////////
// Take a given agnostic hand, such as "AA" or "QJs+" or "TT-77", along with
// an optional collection of "dead" cards, and boil it down into its constituent
// specific Hold'em hands, storing these in the 'specificHands' vector passed
// in by the client.
//
// Returns the number of specific hands in the distribution.
//
// This version calls the other version of Instantiate internally.
///////////////////////////////////////////////////////////////////////////////
int HoldemAgnosticHand::Parse(const char* handText, const char* deadText)
{
  StdDeck_CardMask deadCards;
  StdDeck_CardMask_RESET(deadCards);
		
  if (deadText && strlen(deadText))
    {
      int suit, rank;
      StdDeck_CardMask hand;
      for(const char* pCard = deadText; pCard != NULL; pCard += 2)
	{
	  rank = Card::CharToRank(*pCard);
	  suit = Card::CharToSuit(*(pCard+1));
	  hand = StdDeck_MASK( StdDeck_MAKE_CARD(rank, suit) );
	  StdDeck_CardMask_OR(deadCards, deadCards, hand);
	}
    }

  return Parse(handText, deadCards);
}

///////////////////////////////////////////////////////////////////////////////
// Take a given agnostic hand, such as "AA" or "QJs+" or "TT-77", along with
// an optional collection of "dead" cards, and boil it down into its constituent
// specific Hold'em hands, storing these in the 'specificHands' vector passed
// in by the client.
//
// Returns true of syntax is correct.
///////////////////////////////////////////////////////////////////////////////
int HoldemAgnosticHand::Parse(const char* handText, StdDeck_CardMask deadCards)
{
  if (strcmp(handText, "XxXx") == 0) {
    return 1;
  }
  
  if (IsSpecificHand(handText)) {
    return 1;
  }

  char *newstr = strdup(handText);
  const char *p = newstr;
  bool suitOffsuit = false;
  int seenCards = 0;

  while (*p != '\0' && seenCards < 2) {
    if (NULL != strchr("23456789TtJjQqKkAaXx", *p)) {
      seenCards++;
      p++; while (*p == ' ') p++;
      if (NULL != strchr("23456789TtJjQqKkAaXx", *p)) {
	p++; while (*p == ' ') p++;
	if (NULL != strchr("SsOo", *p)) {
	  seenCards++;
	  suitOffsuit = true;
	  p++; while (*p == ' ') p++;
	  if (*p == '+') {
	    p++; while (*p == ' ') p++;
	  }
	}
	else if (*p == '+') {
	  seenCards++;
	  p++; while (*p == ' ') p++;
	}
	else if (*p == '-') {
	  seenCards--;
	  p++; while (*p == ' ') p++;
	  if (NULL != strchr("23456789TtJjQqKkAa", *p)) {
	    p++; while (*p == ' ') p++; seenCards++;
	    if (NULL != strchr("23456789TtJjQqKkAa", *p)) {
	      seenCards++;
	      p++; while (*p == ' ') p++;
	      if (*p != '\0') {
		if (NULL != strchr("SsOo", *p)) {
		  if (suitOffsuit) {
		    p++; while (*p == ' ') p++;
		  }
		  else {
		    printf("%d: Unexpected %c, at %s %ld\n", __LINE__, *p, p, p-handText+1);
		    goto error;
		  }
		}
	      }
	    }
	    else {
	      printf("%d: Unexpected %c, at %s %ld\n", __LINE__, *p, p, p-handText+1);
	      goto error;
	    }
	  }
	}
	else if (*p == '\0') {
	  continue;
	}
	else {
	  printf("%d: Unexpected %c, at %s %ld\n", __LINE__, *p, p, p-handText+1);
	  goto error;
	}
      }
      else {
	printf("%d: Unexpected %c, at %s %ld\n", __LINE__, *p, p, p-handText+1);
	goto error;
      }
    }
    else {
      goto error;
    }
  }
  
  if (seenCards != 2) {
    printf("Only saw %d cards in %s\n", seenCards, handText);
    goto error;
  }
  if (*p != '\0') {
    printf("Unexpected %s at end of %s\n", p, handText);
    goto error;
  }

  return 1;
 error:
  if (newstr) free(newstr);
  return 0;
}

int HoldemAgnosticHand::Instantiate(const char* handText, const char* deadText, vector<StdDeck_CardMask>& specificHands)
{
  StdDeck_CardMask deadCards;
  StdDeck_CardMask_RESET(deadCards);
		
  if (deadText && strlen(deadText))
    {
      int suit, rank;
      StdDeck_CardMask hand;
      for(const char* pCard = deadText; pCard != NULL; pCard += 2)
	{
	  rank = Card::CharToRank(*pCard);
	  suit = Card::CharToSuit(*(pCard+1));
	  hand = StdDeck_MASK( StdDeck_MAKE_CARD(rank, suit) );
	  StdDeck_CardMask_OR(deadCards, deadCards, hand);
	}
    }

  return Instantiate(handText, deadCards, specificHands);
}



///////////////////////////////////////////////////////////////////////////////
// Take a given agnostic hand, such as "AA" or "QJs+" or "TT-77", along with
// an optional collection of "dead" cards, and boil it down into its constituent
// specific Hold'em hands, storing these in the 'specificHands' vector passed
// in by the client.
//
// Returns the number of specific hands the agnostic hand contains.
///////////////////////////////////////////////////////////////////////////////
int HoldemAgnosticHand::Instantiate(const char* handText, StdDeck_CardMask deadCards, vector<StdDeck_CardMask>& specificHands)
{
  if (strcmp(handText, "XxXx") == 0)
    {
      return InstantiateRandom(deadCards, specificHands);
    }

  bool isPlus = (NULL != strchr(handText, '+'));
  bool isSlice = (NULL != strchr(handText, '-'));
  int handRanks[2] = {0,0};
  int rankCeils[2] = {0,0};

  if (isSlice)
    {
      const char* index = strchr(handText, '-');

      char handCeil[4];
      char handFloor[4];
      strncpy(handCeil, handText, index - handText);
      strcpy(handFloor, index + 1);

      handRanks[0] = Card::CharToRank(handFloor[0]);
      handRanks[1] = Card::CharToRank(handFloor[1]);
      rankCeils[0] = Card::CharToRank(handCeil[0]);
      rankCeils[1] = Card::CharToRank(handCeil[1]);
    }
  else
    {
      handRanks[0] = Card::CharToRank(handText[0]);
      handRanks[1] = Card::CharToRank(handText[1]);
      rankCeils[0] = isPlus ? Card::Ace : handRanks[0];
      rankCeils[1] = (NULL != strchr("Xx", handText[1])) ? Card::Ace : Card::King;
    }

  StdDeck_CardMask hand;

  int combos = 0;

  if (IsPair(handText))
    {
      StdDeck_CardMask card1, card2;

      for (int rank = handRanks[0]; rank <= rankCeils[0]; rank++)
	{
	  for(int suit1 = StdDeck_Suit_FIRST; suit1 <= StdDeck_Suit_LAST; suit1++)
	    {
	      for (int suit2 = suit1 + 1; suit2 <= StdDeck_Suit_LAST; suit2++)
		{
		  card1 = StdDeck_MASK( StdDeck_MAKE_CARD(rank, suit1) );
		  card2 = StdDeck_MASK( StdDeck_MAKE_CARD(rank, suit2) );
		  StdDeck_CardMask_OR(hand, card1, card2);
		  if (!StdDeck_CardMask_ANY_SET(deadCards, hand))
		    {
		      specificHands.push_back(hand);
		      combos++;
		    }
		}
	    }


	}
    }
  else if (IsSuited(handText))
    {
      StdDeck_CardMask card1, card2, hand;
      // If a range like "A4s+" was specified, increment only the bottom card
      // ie, "A4s, A5s, A6s, ..., AQs, AKs
      int rank0Increment = 1;
      if (handRanks[0] == Card::Ace)
	rank0Increment = 0;
      for (int rank0 = handRanks[0], rank1 = handRanks[1]; 
	   rank0 <= rankCeils[0] && rank1 <= rankCeils[1];
	   rank0 += rank0Increment, rank1++)
	{
	  for(int suit = StdDeck_Suit_FIRST; suit <= StdDeck_Suit_LAST; suit++)
	    {
	      if (rank0 == rank1)
		continue;
	      card1 = StdDeck_MASK( StdDeck_MAKE_CARD(rank0, suit) );
	      card2 = StdDeck_MASK( StdDeck_MAKE_CARD(rank1, suit) );
	      StdDeck_CardMask_OR(hand, card1, card2);
	      if (!StdDeck_CardMask_ANY_SET(deadCards, hand))
		{
		  specificHands.push_back(hand);
		  combos++;
		}
	    }
	}
    }
  else if (IsOffSuit(handText))
    {
      StdDeck_CardMask card1, card2, hand;

      int rank0Increment = 1;
      if (handRanks[0] == Card::Ace)
	rank0Increment = 0;

      for (int rank0 = handRanks[0], rank1 = handRanks[1]; 
	   rank0 <= rankCeils[0] && rank1 <= rankCeils[1];
	   rank0 += rank0Increment, rank1++)
	{
	  for(int suit1 = StdDeck_Suit_FIRST; suit1 <= StdDeck_Suit_LAST; suit1++)
	    {
	      for (int suit2 = StdDeck_Suit_FIRST; suit2 <= StdDeck_Suit_LAST; suit2++)
		{
		  if (suit1 == suit2)
		    continue;

		  card1 = StdDeck_MASK( StdDeck_MAKE_CARD(rank0, suit1) );
		  card2 = StdDeck_MASK( StdDeck_MAKE_CARD(rank1, suit2) );
		  StdDeck_CardMask_OR(hand, card1, card2);
		  if (!StdDeck_CardMask_ANY_SET(deadCards, hand))
		    {
		      specificHands.push_back(hand);
		      combos++;
		    }
		}
	    }
	}
    }
  else
    {
      StdDeck_CardMask card1, card2, hand;

      int rank0Increment = 1;
      if (handRanks[0] == Card::Ace)
	rank0Increment = 0;

      for (int rank0 = handRanks[0], rank1 = handRanks[1]; 
	   rank0 <= rankCeils[0] && rank1 <= rankCeils[1];
	   rank0 += rank0Increment, rank1++)
	{
	  for(int suit1 = StdDeck_Suit_FIRST; suit1 <= StdDeck_Suit_LAST; suit1++)
	    {
	      for (int suit2 = StdDeck_Suit_FIRST; suit2 <= StdDeck_Suit_LAST; suit2++)
		{
		  if (rank0 == rank1 && suit1 == suit2)
		    continue;
		  card1 = StdDeck_MASK( StdDeck_MAKE_CARD(rank0, suit1) );
		  card2 = StdDeck_MASK( StdDeck_MAKE_CARD(rank1, suit2) );
		  StdDeck_CardMask_OR(hand, card1, card2);
		  if (!StdDeck_CardMask_ANY_SET(deadCards, hand))
		    {
		      specificHands.push_back(hand);
		      combos++;
		    }
		}
	    }
	}
    }

  return combos;
}



///////////////////////////////////////////////////////////////////////////////
// Take the "XxXx" (random/unknown) agnostic hand and convert it to it's
// specific constituent hands. Now, if no dead cards are specified, a random
// hand always contains 1,326 possibilities. If one or more dead cards are
// specified, that number will be less.
//
// Returns the number of specific hands contained in the agnostic hand.
///////////////////////////////////////////////////////////////////////////////
int HoldemAgnosticHand::InstantiateRandom(StdDeck_CardMask deadCards, vector<StdDeck_CardMask>& specificHands)
{
  StdDeck_CardMask curHand;
  DECK_ENUMERATE_2_CARDS_D(StdDeck, curHand, deadCards, specificHands.push_back(curHand); );
  return specificHands.size();
}




///////////////////////////////////////////////////////////////////////////////
// Helper method used by HoldemAgnosticHand::Instantiate. Determine if this is a
// "pair" sort of hand such as "AA" or "QQ+" or "JJ-88".
///////////////////////////////////////////////////////////////////////////////
bool HoldemAgnosticHand::IsPair(const char* handText)
{ 
  return (handText[0] == handText[1]);
}



///////////////////////////////////////////////////////////////////////////////
// Helper method used by HoldemAgnosticHand::Instantiate. Determine if this is an
// "suited" sort of hand such as "A2s" or "T9s+" or "QJs-65s".
///////////////////////////////////////////////////////////////////////////////
bool HoldemAgnosticHand::IsSuited(const char* handText)
{ 
  return (strlen(handText) >= 3 && handText[2] == 's');
}



///////////////////////////////////////////////////////////////////////////////
// Helper method used by HoldemAgnosticHand::Instantiate. Determine if this is an
// "offsuit" sort of hand such as "A2o" or "T9o+" or "QJo-65o".
///////////////////////////////////////////////////////////////////////////////
bool HoldemAgnosticHand::IsOffSuit(const char* handText)
{ 
  return (strlen(handText) >= 3 && handText[2] == 'o');
}



///////////////////////////////////////////////////////////////////////////////
// Helper method used by HoldemAgnosticHand::Instantiate. Determine if this is an
// "inclusive" sort of hand, meaning either suited or unsuited but not a pair.
// For example, "AJ", "K2+", and "T9-87" are all "inclusive".
///////////////////////////////////////////////////////////////////////////////
bool HoldemAgnosticHand::IsInclusive(const char* handText)
{ 
  int textlen = strlen(handText);
  return !IsPair(handText) && ((textlen == 2) || (textlen == 3 && handText[2] == '+'));
}

///////////////////////////////////////////////////////////////////////////////
// This static function is just a quick way to look at a given textual hand
// and determine if it's a specific/known hand such as "AhKh" or "2d2c".
// This implementation is ugly - sorry.
///////////////////////////////////////////////////////////////////////////////
bool HoldemAgnosticHand::IsSpecificHand(const char* handText)
{
  if (strlen(handText) == 4)
    {
      return (NULL != strchr("SsHhDdCc", handText[1]) && 
	      NULL != strchr("SsHhDdCc", handText[3]) &&
	      NULL != strchr("23456789TtJjQqKkAa", handText[0]) &&
	      NULL != strchr("23456789TtJjQqKkAa", handText[2]));
    }

  return false;
}
