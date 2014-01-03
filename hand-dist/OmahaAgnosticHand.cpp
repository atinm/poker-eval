///////////////////////////////////////////////////////////////////////////////
//
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

#include <inlines/eval_omaha.h>
#include "HandDistributions.h"
#include "OmahaAgnosticHand.h"
#include "CardConverter.h"
#include "Card.h"

#ifdef MY_DEBUG
#define dbg_printf(...) printf(__VA_ARGS__);
#define dbg_printMask(...) StdDeck_printMask(__VA_ARGS__)
#else
#define dbg_printf(...)
#define dbg_printMask(...)
#endif
///////////////////////////////////////////////////////////////////////////////
// Take a given agnostic hand, such as "AKQJ" or "T+T+T+T+" or "A-TA-TTT-77", along with
// an optional collection of "dead" cards, and boil it down into its constituent
// specific Omaha hands, storing these in the 'specificHands' vector passed
// in by the client.
//
// Returns the number of specific hands in the distribution.
//
// This version calls the other version of Instantiate internally.
///////////////////////////////////////////////////////////////////////////////

static bool funcIsNoPair(int rank0, int rank1, int rank2, int rank3)
{
  return !(rank0 == rank1 || rank0 == rank2 || rank0 == rank3 || rank1 == rank2 || rank1 == rank3 || rank2 == rank3);
}

static bool funcIsOnePair(int rank0, int rank1, int rank2, int rank3)
{
  return ((rank0 == rank1 && rank0 != rank2 && rank0 != rank3) ||
	  (rank0 == rank2 && rank0 != rank1 && rank0 != rank3) ||
	  (rank0 == rank3 && rank0 != rank1 && rank0 != rank2) ||
	  (rank1 == rank2 && rank1 != rank3 && rank1 != rank0) ||
	  (rank1 == rank3 && rank1 != rank2 && rank1 != rank0) ||
	  (rank2 == rank3 && rank2 != rank0 && rank2 != rank1));
}

static bool funcIsTwoPair(int rank0, int rank1, int rank2, int rank3)
{
  return ((rank0 == rank1 && rank2 == rank3 && rank0 != rank2) ||
	  (rank0 == rank3 && rank1 == rank2 && rank0 != rank1));
}

static bool funcIsTrips(int rank0, int rank1, int rank2, int rank3)
{
  // return false when isTrips is true
  return ((rank0 == rank1 && rank1 == rank2) ||
	  (rank0 == rank1 && rank1 == rank3) ||
	  (rank0 == rank2 && rank0 == rank3) ||
	  (rank1 == rank2 && rank2 == rank3));
}

static bool funcIsQuads(int rank0, int rank1, int rank2, int rank3)
{
  return (rank0 == rank1 && rank1 == rank2 && rank2 == rank3);
}

static bool funcIsAtLeastOnePair(int rank0, int rank1, int rank2, int rank3)
{
  return (funcIsQuads(rank0, rank1, rank2, rank3) ||
	  funcIsTrips(rank0, rank1, rank2, rank3) ||
	  funcIsTwoPair(rank0, rank1, rank2, rank3) ||
	  funcIsOnePair(rank0, rank1, rank2, rank3));
}

static bool funcIsAtLeastTrips(int rank0, int rank1, int rank2, int rank3)
{
  return (funcIsQuads(rank0, rank1, rank2, rank3) ||
	  funcIsTrips(rank0, rank1, rank2, rank3));
}

static bool funcIsThreeOfSuit(int suit0, int suit1, int suit2, int suit3)
{
  return funcIsTrips(suit0, suit1, suit2, suit3); // works the same as ranks!
}

static bool funcIsRainbow(int suit0, int suit1, int suit2, int suit3)
{
  return funcIsNoPair(suit0, suit1, suit2, suit3); // works the same as ranks!
}

static bool funcIsOneSuited(int suit0, int suit1, int suit2, int suit3)
{
  // works the same as ranks!
  return (funcIsQuads(suit0, suit1, suit2, suit3) ||
	  funcIsTrips(suit0, suit1, suit2, suit3) ||
	  funcIsOnePair(suit0, suit1, suit2, suit3));
}

static bool funcIsSingleSuited(int suit0, int suit1, int suit2, int suit3)
{
  return funcIsOnePair(suit0, suit1, suit2, suit3); // works the same as ranks!
}

static bool funcIsDoubleSuited(int suit0, int suit1, int suit2, int suit3)
{
  return funcIsTwoPair(suit0, suit1, suit2, suit3); // works the same as ranks!
}

static bool funcIsMonotone(int suit0, int suit1, int suit2, int suit3)
{
  return funcIsQuads(suit0, suit1, suit2, suit3); // works the same as ranks!
}

static bool funcIsAtLeastThreeSuit(int suit0, int suit1, int suit2, int suit3)
{
  return funcIsAtLeastTrips(suit0, suit1, suit2, suit3); // works the same as ranks!
}

static bool funcIsAtLeastSingleSuit(int suit0, int suit1, int suit2, int suit3)
{
  return funcIsAtLeastOnePair(suit0, suit1, suit2, suit3);
}

static bool funcIsSuitedAce(int rank0, int rank1, int rank2, int rank3,
			    int suit0, int suit1, int suit2, int suit3)
{
  return ((rank0 == Card::Ace && (suit0 == suit1 || suit0 == suit2 || suit0 == suit3)) ||
	  (rank1 == Card::Ace && (suit1 == suit0 || suit1 == suit2 || suit1 == suit3)) ||
	  (rank2 == Card::Ace && (suit2 == suit0 || suit2 == suit1 || suit2 == suit3)) ||
	  (rank3 == Card::Ace && (suit3 == suit0 || suit3 == suit1 || suit3 == suit2)));
}

static bool funcIsSuitedNonAce(int rank0, int rank1, int rank2, int rank3,
			    int suit0, int suit1, int suit2, int suit3)
{
  if (funcIsAtLeastOnePair(suit0, suit1, suit2, suit3)) {
    if ((suit0 == suit1 && rank0 != Card::Ace && rank1 != Card::Ace) ||
	(suit0 == suit2 && rank0 != Card::Ace && rank2 != Card::Ace) ||
	(suit0 == suit3 && rank0 != Card::Ace && rank3 != Card::Ace) ||
	(suit1 == suit2 && rank1 != Card::Ace && rank2 != Card::Ace) ||
	(suit1 == suit3 && rank1 != Card::Ace && rank3 != Card::Ace) ||
	(suit2 == suit3 && rank2 != Card::Ace && rank3 != Card::Ace))
      return true;
  }

  return false;
}

OmahaAgnosticHand::OmahaAgnosticHand(void)
{
  Reset();
}

OmahaAgnosticHand::~OmahaAgnosticHand(void)
{
}

void OmahaAgnosticHand::Reset(void)
{
  m_isNoPair = false;
  m_isNoTrips = false;
  m_isNoQuads = false;
  m_isOnePair = false;
  m_isTwoPair = false;
  m_isTrips = false;
  m_isQuads = false;
  m_isAtLeastOnePair = false;
  m_isAtLeastTrips = false;
  m_isThreeOfSuit = false;
  m_isRainbow = false;
  m_isOneSuited = false;
  m_isSingleSuited = false;
  m_isDoubleSuited = false;
  m_isMonotone = false;
  m_isAtLeastSingleSuit = false;
  m_isAtLeastThreeSuit = false;
  m_isSuitedAce = false;
  m_isSuitedNonAce = false;
  m_seenCards = 0;
  for (int i=0; i < OMAHA_MAXHOLE; i++) {
    m_gap[i] = 0;
    m_suitType[i] = Any;
  }
}

int OmahaAgnosticHand::Parse(const char* handText, const char* deadText)
{
  StdDeck_CardMask deadCards;
  StdDeck_CardMask_RESET(deadCards);
                
  if (deadText && strlen(deadText)) {
      int suit, rank;
      StdDeck_CardMask hand;
      for(const char* pCard = deadText; pCard != NULL; pCard += 2) {
	rank = Card::CharToRank(*pCard);
	suit = Card::CharToSuit(*(pCard+1));
	hand = StdDeck_MASK( StdDeck_MAKE_CARD(rank, suit) );
	StdDeck_CardMask_OR(deadCards, deadCards, hand);
      }
  }

  return Parse(handText, deadCards);
}

int OmahaAgnosticHand::Parse(const char* handText, StdDeck_CardMask deadCards)
{
  Reset(); // start fresh every time

  if (strcmp(handText, "XXXX") == 0) {
    return 1; // valid
  }

  if (IsSpecificHand(handText)) {
    return 1; // valid
  }

  const char *p = handText;

  // Shared between parsing and generation
  //int rankFloor[4], rankCeil[4];
  //int m_suitFloor[4], suitCeil[4];
  //int gap[4] = {0, 0, 0, 0};
  //SuitType m_suitType[4] = {Any, Any, Any, Any};

  bool firstSuit = true;
  bool isSuited = false;

//   // rank filters
//   bool isNoPair = false, isNoTrips = false, isNoQuads = false,
//     isOnePair = false, isTwoPair = false, isTrips = false, isQuads = false,
//     isAtLeastOnePair = false, isAtLeastTrips = false;
//   // suit filters
//   bool isThreeOfSuit = false, isRainbow = false, isOneSuited = false,
//     isSingleSuited = false, isDoubleSuited = false, isMonotone = false,
//     isAtLeastSingleSuit = false, isAtLeastThreeSuit = false;
//   bool isSuitedAce = false, isSuitedNonAce = false;

  int cur = 0;
  while (*p != '\0' && *p != '/') {
    while (*p == ' ') p++;
    
    if (m_seenCards == OMAHA_MAXHOLE) {
      // we can only close any suited sections after
      if (*p == ']') {
	if (isSuited) {
	  isSuited = false;
	}
	else {
	  printf("Closing ']' in non-bracketed section\n");
	  goto error;
	}
	p++; while (*p == ' ') p++;
      }
      else {
	if (isSuited) {
	  printf("No closing ']' found, %s\n", p);
	  goto error;
	}
	else {
	  printf("Unrecognized chars %s\n", p);
	  goto error;
	}
      }
      break; // we're done
    }

    if (*p == '[') {
      if (firstSuit) {
	m_suitType[cur] = Any;
	dbg_printf("m_suitType[%d]=Any, %s\n", cur, p);
      }
      else {
	m_suitType[cur] = New;
	dbg_printf("m_suitType[%d]=New, %s\n", cur, p);
      }
      isSuited = true;
      p++; while (*p == ' ') p++;
    }
    else if (*p == ']') {
      if (isSuited) {
	isSuited = false;
	m_suitType[cur] = Any;
	dbg_printf("m_suitType[%d]=Any, %s\n", cur, p);
      }
      else {
	printf("Closing ']' in non-bracketed section\n");
	goto error;
      }
      p++; while (*p == ' ') p++;
    }
    else if (*p == ':' && cur > 0) {
      p++; while (*p == ' ') p++;
      if (isdigit(*p)) {
	m_gap[cur] = atoi(p);
	if (m_gap[cur] > 12 || m_gap[cur] < 0) {
	  printf("gap error: 0 < \"%d\" < 12\n", m_gap[cur]);
	  goto error;
	}
                
	if (m_gap[cur] < 10) {
	  p++; while (*p == ' ') p++;
	}
	else {
	  p++; p++; while (*p == ' ') p++; // two chars
	}
	m_gap[cur]++; // need to be 1  more than specified as Q-J is 0 gap, but 1 rank difference
                
	// can be any rank and suit as long as rank gap is ok
	m_rankFloor[cur] = Card::Two;
	m_rankCeil[cur] = Card::Ace;
	m_suitFloor[cur] = 0;
	m_suitCeil[cur] = 3;
	cur++;
	m_seenCards++;
      }
      else {
	printf("non-digit gap: %c\n", *p);
	goto error;
      }
    }
    else if (NULL != strchr("AaKkQqJjTt98765432BbRrFfMmZzLlNnYyXx", *p)) {
      if (NULL != strchr("AaKkQqJjTt98765432", *p)) {
	if (p[1] == '-') {
	  m_rankFloor[cur] = Card::CharToRank(*p);
	  dbg_printf("%d: m_rankFloor[%d]: %d, %s\n", __LINE__, cur, m_rankFloor[cur], p);
	  p++; while (*p == ' ') p++;
	  p++; while (*p == ' ') p++;
	  if (NULL != strchr("AaKkQqJjTt98765432", *p)) {
	    m_rankCeil[cur] = Card::CharToRank(*p);
	    p++; while (*p == ' ') p++;
	  }
	  else {
	    printf("%c not in \"AaKkQqJjTt98765432\"\n", *p);
	    goto error;
	  }
	  if (m_rankCeil[cur] < m_rankFloor[cur]) {
	    int tmp = m_rankCeil[cur];
	    m_rankCeil[cur] = m_rankFloor[cur];
	    m_rankFloor[cur] = tmp;
	    dbg_printf("%d: m_rankFloor[%d]: %d\n", __LINE__, cur, m_rankFloor[cur]);
	  }
	}
	else if (p[1] == '+') {
	  m_rankFloor[cur] = Card::CharToRank(*p);
	  dbg_printf("%d: m_rankFloor[%d]: %d\n", __LINE__, cur, m_rankFloor[cur]);
	  p++; while (*p == ' ') p++;
	  p++; while (*p == ' ') p++;
	  m_rankCeil[cur] = Card::Ace;
	}
	else {
	  m_rankFloor[cur] = Card::CharToRank(*p);
	  dbg_printf("%d: m_rankFloor[%d]: %d\n", __LINE__, cur, m_rankFloor[cur]);
	  m_rankCeil[cur] = m_rankFloor[cur];
	  m_gap[cur] = -1; // any
	  p++; while (*p == ' ') p++;
	}
      }
      else {
	switch (*p) {
	case 'B':
	case 'b':
	  m_rankFloor[cur] = Card::Jack;
	  m_rankCeil[cur] = Card::Ace;
	  break;
	case 'R':
	case  'r':
	  m_rankFloor[cur] = Card::Ten;
	  m_rankCeil[cur] = Card::Ace;
	  break;
	case 'F':
	case 'f':
	  m_rankFloor[cur] = Card::Jack;
	  m_rankCeil[cur] = Card::King;
	  break;
	case 'M':
	case 'm':
	  m_rankFloor[cur] = Card::Seven;
	  m_rankCeil[cur] = Card::Ten;
	  break;
	case 'Z':
	case 'z':
	  m_rankFloor[cur] = Card::Two;
	  m_rankCeil[cur] = Card::Six;
	  break;
	case 'L':
	case 'l':
	  m_rankFloor[cur] = Card::Two;
	  m_rankCeil[cur] = Card::Eight;
	  break;
	case 'N':
	case 'n':
	  m_rankFloor[cur] = Card::Nine;
	  m_rankCeil[cur] = Card::King;
	  break;
	case 'Y':
	case 'y':
	  m_rankFloor[cur] = Card::Two;
	  m_rankCeil[cur] = Card::Five;
	  break;
	case 'X':
	case 'x':
	  m_rankFloor[cur] = Card::Two;
	  m_rankCeil[cur] = Card::Ace;
	  break;
	default:
	  printf("%c not in \"BbRrFfMmZzLlNnYyXx\"\n", *p);
	  goto error;
	}
        
	p++; while (*p == ' ') p++;
      }
      
      if (*p != '\0' && NULL != strchr("CcDdHhSs", *p)) {
	if (isSuited) {
	  printf("Suit %c specified in [] section\n", *p);
	  goto error;
	}
	
	m_suitType[cur] = Specific;
	dbg_printf("m_suitType[%d] = Specific, %s\n", cur, p);
	m_suitCeil[cur] = Card::CharToSuit(*p);
	m_suitFloor[cur] = Card::CharToSuit(*p);
	
	p++; while (*p == ' ') p++;
      }
      else {
	m_suitCeil[cur] = 3; // max suit
	m_suitFloor[cur] = 0; // min suit
	
	if (isSuited) {
	  if (!firstSuit && m_suitType[cur] != New) {
	    m_suitType[cur] = Current;
	    dbg_printf("m_suitType[%d] = Current, %s\n", cur, p);
	  }
	  firstSuit = false;
	}
	else {
	  m_suitType[cur] = Any;
	  dbg_printf("m_suitType[%d] = Any, %s\n", cur, p);
	}
      }
      dbg_printf("m_rankFloor[%d]: %d, m_rankCeil[%d]: %d, m_suitFloor[%d]: %d, m_suitCeil[%d]: %d\n",
	     cur, m_rankFloor[cur], cur, m_rankCeil[cur], cur, m_suitFloor[cur], cur, m_suitCeil[cur]);
      m_seenCards++;
      cur++;
    }
    else {
      printf("Unrecognized char %c\n", *p);
      goto error;
    }
  }

  if (isSuited) {
    printf("No closing ']' found, %s\n", p);
    goto error;
  }

  if (m_seenCards != OMAHA_MAXHOLE) {
    printf("Need to specify 4 cards per hand, specified: %d\n", m_seenCards);
    goto error;
  }
  
  // check if filters are specified
  while (*p == ' ') p++;
  if (*p == '/') {
    while (*p == '/') {
      p++;
      if (NULL != strchr("Nn", *p)) {
	  p++;
	  if (NULL != strchr("Pp", *p)) {
	    m_isNoPair = true;
	  }
	  else if (NULL != strchr("Tt", *p)) {
	    m_isNoTrips = true;
	  }
	  else if (NULL != strchr("Qq", *p)) {
	    m_isNoQuads = true;
	  }
	  else {
	    printf("Unrecognized filter /n%c\n", *p);
	    goto error;
	  }
	  p++;
	}
      
      else if (NULL != strchr("Oo", *p)) {
	p++;
	if (NULL != strchr("Pp", *p)) {
	  m_isOnePair = true;
	}
	else if (NULL != strchr("Ss", *p)) {
	  m_isOneSuited = true;
	}
      }
      else if (NULL != strchr("Tt", *p)) {
	p++;
	if (NULL != strchr("Pp", *p)) {
	  m_isTwoPair = true;
	}
	else if (NULL != strchr("Rr", *p)) {
	  m_isTrips = true;
	}
	else if (NULL != strchr("Ss", *p)) {
	  m_isThreeOfSuit = true;
	}
	else {
	  printf("Unrecognized filter /t%c\n", *p);
	  goto error;
	}
	p++;
      }      
      else if (NULL != strchr("Qq", *p)) {
	p++;
	if (NULL != strchr("Uu", *p)) {
	  m_isQuads = true;
	}
	else {
	  printf("Unrecognized filter /q%c\n", *p);	  
	  goto error;
	}
	p++;
      }
      else if (NULL != strchr("Rr", *p)) {
	p++;
	if (NULL != strchr("Bb", *p)) {
	  m_isRainbow = true;
	}
	else {
	  printf("Unrecognized filter /r%c\n", *p);
	  goto error;
	}
	p++;
      }
      else if (NULL != strchr("Oo", *p)) {
	p++;
	if (NULL != strchr("Ss", *p)) {
	  m_isOneSuited = true;
	}
	else {
	  printf("Unrecognized filter /o%c\n", *p);
	  goto error;
	}
	p++;
      }
      else if (NULL != strchr("Dd", *p)) {
	p++;
	if (NULL != strchr("Ss", *p)) {
	  m_isDoubleSuited = true;
	}
	else {
	  printf("Unrecognized filter /d%c\n", *p);
	  goto error;
	}
	p++;
      }
      else if (NULL != strchr("Mm", *p)) {
	p++;
	if (NULL != strchr("Tt", *p)) {
	  m_isMonotone = true;
	}
	else {
	  printf("Unrecognized filter /m%c\n", *p);
	  goto error;
	}
	p++;
      }
      else if (NULL != strchr("Ss", *p)) {
	p++;
	if (NULL != strchr("Ss", *p)) {
	  m_isSingleSuited = true;
	}
	else if (NULL != strchr("Aa", *p)) {
	  m_isSuitedAce = true;
	}
	else if (NULL != strchr("Nn", *p)) {
	  p++;
	  if (NULL != strchr("Aa", *p)) {
	    m_isSuitedNonAce = true;
	  }
	  else {
	    printf("Unrecognized filter /sn%c\n", *p);
	    goto error;
	  }
	}
	else {
	  printf("Unrecognized filter /s%c\n", *p);
	  goto error;
	}
	p++;
      }
      else if (NULL != strchr("Aa", *p)) {
	p++;
	if (NULL != strchr("Ll", *p)) {
	  p++;
	  if (NULL != strchr("Tt", *p)) {
	    p++;
	    if (NULL != strchr("Ss", *p)) {
	      m_isAtLeastThreeSuit = true;
	    }
	    else if (NULL != strchr("Rr", *p)) {
	      m_isAtLeastTrips = true;
	    }
	    else {
	      printf("Unrecognized filter /alt%c\n", *p);
	      goto error;
	    }
	  }
	  else {
	    printf("Unrecognized filter /al%c\n", *p);
	    goto error;
	  }
	}
	else if (NULL != strchr("Ss", *p)) {
	  p++;
	  if (NULL != strchr("Ss", *p)) {
	    m_isAtLeastSingleSuit = true;
	  }
	  else {
	    printf("Unrecognized filter /as%c\n", *p);
	    goto error;
	  }
	}
	else if (NULL != strchr("Oo", *p)) {
	  p++;
	  if (NULL != strchr("Pp", *p)) {
	    m_isAtLeastOnePair = true;
	  }
	  else {
	    printf("Unrecognized filter /ao%c\n", *p);
	    goto error;
	  }
	}
	else {
	  printf("Unrecognized filter /a%c\n", *p);
	  goto error;
	}
	p++;
      }
      
      while (*p == ' ') p++;
    }	
  }
  else if (*p != '\0') {
    // we should have ended input
    printf("Extra characters at end of input: %s\n", p);
    goto error;
  }
  dbg_printf("1:%d-%d/%d-%d, 2:%d-%d/%d-%d, 3:%d-%d/%d-%d, 4:%d-%d/%d-%d\n",
	 m_rankFloor[0], m_rankCeil[0], m_suitFloor[0], m_suitCeil[0],
	 m_rankFloor[1], m_rankCeil[1], m_suitFloor[1], m_suitCeil[1],
	 m_rankFloor[2], m_rankCeil[2], m_suitFloor[2], m_suitCeil[2],
	 m_rankFloor[3], m_rankCeil[3], m_suitFloor[3], m_suitCeil[3]);

  return 1; // success

 error:
  printf("Error parsing %s, parsing at %s: %ld\n", handText, p, p-handText+1);
  return 0; // failure
}

int OmahaAgnosticHand::Instantiate(const char* handText, const char* deadText, vector<StdDeck_CardMask>& specificHands)
{
  StdDeck_CardMask deadCards;
  StdDeck_CardMask_RESET(deadCards);
                
  if (deadText && strlen(deadText)) {
      int suit, rank;
      StdDeck_CardMask hand;
      for(const char* pCard = deadText; pCard != NULL; pCard += 2) {
	rank = Card::CharToRank(*pCard);
	suit = Card::CharToSuit(*(pCard+1));
	hand = StdDeck_MASK( StdDeck_MAKE_CARD(rank, suit) );
	StdDeck_CardMask_OR(deadCards, deadCards, hand);
      }
  }

  return Instantiate(handText, deadCards, specificHands);
}

///////////////////////////////////////////////////////////////////////////////
// Take a given agnostic hand, such as "AKQJ" or "T+T+T+T+" or "A-TA-TTT-77", along with
// an optional collection of "dead" cards, and boil it down into its constituent
// specific Omaha hands, storing these in the 'specificHands' vector passed
// in by the client.
//
// Returns the number of specific hands the agnostic hand contains.
///////////////////////////////////////////////////////////////////////////////
int OmahaAgnosticHand::Instantiate(const char* handText, StdDeck_CardMask deadCards, vector<StdDeck_CardMask>& specificHands)
{
  if (strcmp(handText, "XXXX") == 0) {
    return InstantiateRandom(deadCards, specificHands);
  }

  if (IsSpecificHand(handText)) {
    specificHands.push_back(CardConverter::TextToPokerEval(handText));
    return specificHands.size();
  }

  // we should have parsed the hand already using Parse().
  if (m_seenCards != 4)
    return 0;

  StdDeck_CardMask card1, card2, card3, card4;
  StdDeck_CardMask hand;
  int combos = 0;

  StdDeck_CardMask_RESET(hand);

  // first card
  for (int rank0 = m_rankFloor[0]; rank0 <= m_rankCeil[0]; rank0++)
    {
      for(int suit0 = m_suitFloor[0]; suit0 <= m_suitCeil[0]; suit0++)
        {
	  StdDeck_CardMask used1;
	  StdDeck_CardMask_RESET(used1);

	  card1 = StdDeck_MASK( StdDeck_MAKE_CARD(rank0, suit0) );
	  StdDeck_CardMask_OR(used1, used1, card1);

	  // second card
	  for (int rank1 = m_rankFloor[1]; rank1 <= m_rankCeil[1]; rank1++)
            {
	      if (m_gap[1] > 0 && m_gap[1] != (rank0-rank1)) continue;

	      for(int suit1 = m_suitFloor[1]; suit1 <= m_suitCeil[1]; suit1++)
                {
		  switch (m_suitType[1])
                    {
		    case New:
		      if (suit1 == suit0) {
			dbg_printf("skipping suit1: %d, suit0: %d\n", suit1, suit0);
			continue; // we want only mismatched suits
		      }
		      break;
		    case Current:
		      if (suit1 != suit0) {
			dbg_printf("skipping suit1: %d, suit0: %d\n", suit1, suit0);
			continue; // we want only matching suits
		      }
		      break;
		    case Specific:
		      break;
		    case Any:
		      break;
                    }
                    
		  StdDeck_CardMask used2;
		  StdDeck_CardMask_RESET(used2);
		  card2 = StdDeck_MASK( StdDeck_MAKE_CARD(rank1, suit1) );
		  dbg_printf("rank1: %d, suit1: %d\n", rank1, suit1);
		  dbg_printf("set card2: "); dbg_printMask(card2); dbg_printf("\n");
		  if (!StdDeck_CardMask_ANY_SET(used1, card2))
                    {
		      StdDeck_CardMask_OR(used2, used1, card2);
                    }
		  else
		    continue; // in use card
                    
		  // third card
		  for (int rank2 = m_rankFloor[2]; rank2 <= m_rankCeil[2]; rank2++)
                    {
		      if (m_gap[2] > 0 && m_gap[2] != (rank1-rank2)) continue;

		      for(int suit2 = m_suitFloor[2]; suit2 <= m_suitCeil[2]; suit2++)
                        {
			  switch (m_suitType[2])
                            {
			    case New:
			      if (suit2 == suit0 || suit2 == suit1) {
				dbg_printf("skipping suit2: %d, suit0: %d, suit1: %d\n", suit2, suit0, suit1);
				continue; // we want only mismatched suits
			      }
			      break;
			    case Current:
			      if (suit2 != suit1) {
				dbg_printf("skipping suit2: %d, suit1: %d\n", suit2, suit1);
				continue; // we want only matching suits
			      }
			      break;
			    case Specific:
			      break;
			    case Any:
			      break;
                            }

			  StdDeck_CardMask used3;
			  StdDeck_CardMask_RESET(used3);

			  card3 = StdDeck_MASK( StdDeck_MAKE_CARD(rank2, suit2) );
			  if (!StdDeck_CardMask_ANY_SET(used2, card3))
                            {
			      StdDeck_CardMask_OR(used3, used2, card3);
                            }
			  else
			    continue; // in use card

			  // fourth card
			  for (int rank3 = m_rankFloor[3]; rank3 <= m_rankCeil[3]; rank3++)
                            {
			      if (m_gap[3] > 0 && m_gap[3] != (rank2-rank3)) continue;

			      for(int suit3 = m_suitFloor[3]; suit3 <= m_suitCeil[3]; suit3++)
                                {
				  switch (m_suitType[3])
                                    {
				    case New:
				      if (suit3 == suit0 || suit3 == suit1 || suit3 == suit2) {
					dbg_printf("skipping suit3: %d, suit0: %d, suit1: %d, suit2: %d\n", suit3, suit0, suit1, suit2);
					continue; // we want only mismatched suits
				      }
				      break;
				    case Current:
				      if (suit3 != suit2) {
					dbg_printf("skipping suit3: %d, suit2: %d\n", suit3, suit2);
					continue; // we want only matching suits
				      }
				      break;
				    case Specific:
				      break;
				    case Any:
				      break;
                                    }
				  card4 = StdDeck_MASK( StdDeck_MAKE_CARD(rank3, suit3) );
				  if (StdDeck_CardMask_ANY_SET(used3, card4))
				    continue; // in use card

				  // now check filters one by one
				  if (m_isNoPair && !funcIsNoPair(rank0, rank1, rank2, rank3))
				    continue; // at least a pair is present
				  if (m_isOnePair && !funcIsOnePair(rank0, rank1, rank2, rank3))
				    continue; // one pair is not present
				  if (m_isTwoPair && !funcIsTwoPair(rank0, rank1, rank2, rank3))
				    continue; // two pair is not present
				  if (m_isNoTrips && funcIsTrips(rank0, rank1, rank2, rank3))
				    continue; // at least a trips is present
				  if (m_isTrips && !funcIsTrips(rank0, rank1, rank2, rank3))
				    continue; // no trips is present
				  if (m_isNoQuads && funcIsQuads(rank0, rank1, rank2, rank3))
				    continue; // at least quads is present
				  if (m_isQuads && !funcIsQuads(rank0, rank1, rank2, rank3))
				    continue; // no quads is present
				  if (m_isAtLeastOnePair && !funcIsAtLeastOnePair(rank0, rank1, rank2, rank3))
				    continue; // not at least one pair
				  if (m_isAtLeastTrips && !funcIsAtLeastTrips(rank0, rank1, rank2, rank3))
				    continue; // not at least trips
				  if (m_isThreeOfSuit && !funcIsThreeOfSuit(suit0, suit1, suit2, suit3))
				    continue; // not three of suit
				  if (m_isRainbow && !funcIsRainbow(suit0, suit1, suit2, suit3))
				    continue; // not rainbox
				  if (m_isOneSuited && !funcIsOneSuited(suit0, suit1, suit2, suit3))
				    continue;
				  if (m_isSingleSuited && !funcIsSingleSuited(suit0, suit1, suit2, suit3))
				    continue;
				  if (m_isDoubleSuited && !funcIsDoubleSuited(suit0, suit1, suit2, suit3))
				    continue;
				  if (m_isMonotone && !funcIsMonotone(suit0, suit1, suit2, suit3))
				    continue;
				  if (m_isAtLeastSingleSuit && !funcIsAtLeastSingleSuit(suit0, suit1, suit2, suit3))
				    continue;
				  if (m_isAtLeastThreeSuit && !funcIsAtLeastThreeSuit(suit0, suit1, suit2, suit3))
				    continue;
				  if (m_isSuitedAce && !funcIsSuitedAce(rank0, rank1, rank2, rank3,
								      suit0, suit1, suit2, suit3))
				    continue;
				  if (m_isSuitedNonAce && !funcIsSuitedNonAce(rank0, rank1, rank2, rank3,
									    suit0, suit1, suit2, suit3))
				    continue;

				  StdDeck_CardMask_RESET(hand);
				  StdDeck_CardMask_OR(hand, hand, card1);
				  dbg_printf("card1: "); dbg_printMask(card1); dbg_printf("\n");
				  StdDeck_CardMask_OR(hand, hand, card2);
				  dbg_printf("card2: "); dbg_printMask(card2); dbg_printf("\n");
				  StdDeck_CardMask_OR(hand, hand, card3);
				  dbg_printf("card3: "); dbg_printMask(card3); dbg_printf("\n");
				  StdDeck_CardMask_OR(hand, hand, card4);
				  dbg_printf("card4: "); dbg_printMask(card4); dbg_printf("\n");
				  if (!StdDeck_CardMask_ANY_SET(deadCards, hand))
                                    {
				      specificHands.push_back(hand);
				      dbg_printf("HandText: %s ", handText);
				      dbg_printMask(hand);
				      dbg_printf("\n");
				      combos++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
  return combos;
}

///////////////////////////////////////////////////////////////////////////////
// Take the "XxXxXxXx" (random/unknown) agnostic hand and convert it to it's
// specific constituent hands. Now, if no dead cards are specified, a random
// hand always contains 1,326 possibilities. If one or more dead cards are
// specified, that number will be less.
//
// Returns the number of specific hands contained in the agnostic hand.
///////////////////////////////////////////////////////////////////////////////
int OmahaAgnosticHand::InstantiateRandom(StdDeck_CardMask deadCards, vector<StdDeck_CardMask>& specificHands)
{
  StdDeck_CardMask curHand;
  DECK_ENUMERATE_4_CARDS_D(StdDeck, curHand, deadCards, specificHands.push_back(curHand); );
  return specificHands.size();
}

///////////////////////////////////////////////////////////////////////////////
// This static function is just a quick way to look at a given textual hand
// and determine if it's a specific/known hand such as "AhKhTh2d" or "2d2c2s2d".
// This implementation is ugly - sorry.
///////////////////////////////////////////////////////////////////////////////
bool OmahaAgnosticHand::IsSpecificHand(const char* handText)
{
	if (strlen(handText) == 8)
	{
		return (NULL != strchr("SsHhDdCc", handText[1]) && 
			NULL != strchr("SsHhDdCc", handText[3]) &&
			NULL != strchr("SsHhDdCc", handText[5]) &&
			NULL != strchr("SsHhDdCc", handText[7]) &&
			NULL != strchr("23456789TtJjQqKkAa", handText[0]) &&
			NULL != strchr("23456789TtJjQqKkAa", handText[2]) &&
			NULL != strchr("23456789TtJjQqKkAa", handText[4]) &&
			NULL != strchr("23456789TtJjQqKkAa", handText[6]));
	}

	return false;
}
