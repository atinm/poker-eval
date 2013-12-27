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
#include "Card.h"

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
int OmahaAgnosticHand::Instantiate(const char* handText, const char* deadText, vector<StdDeck_CardMask>& specificHands)
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

// CardSuit type, when relating suits to previous cards in hand
typedef enum { New, Current, Specific, Any } CardSuit;

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
	if (strcmp(handText, "XxXxXxXx") == 0)
	{
		return InstantiateRandom(deadCards, specificHands);
	}

    const char *p = handText;
    StdDeck_CardMask hand;
    int combos = 0;

    // Each card is handled separately
    int rankFloor[4], rankCeil[4];
    int suitFloor[4], suitCeil[4];
    CardSuit curSuit[4] = {Any, Any, Any, Any};
    int gap[4] = {0, 0, 0, 0};
    bool isSuited = false;
    int seenCards = 0;

    for (int cur = 0; cur < OMAHA_MAXHOLE; cur++)
    {
        while (*p == ' ') p++;

        if (*p == '\0')
            break;

        if (*p == '[')
        {
            curSuit[cur] = New;
            isSuited = true;
            p++; while (*p == ' ') p++;
            cur--;
        }
        else if (*p == ']')
        {
            if (isSuited)
            {
                isSuited = false;
                curSuit[cur] = Any;
            }
            else
                goto error;
            p++; while (*p == ' ') p++;
            cur--;
        }
        else if (*p == ':' && cur > 0)
        {
            p++; while (*p == ' ') p++;
            if (isdigit(*p))
            {
                gap[cur] = atoi(p);
                if (gap[cur] > 12 || gap[cur] < 0)
                    goto error;
                
                if (gap[cur] < 10)
                {
                    p++; while (*p == ' ') p++;
                }
                else
                {
                    p++; p++; while (*p == ' ') p++; // two chars
                }
                gap[cur]++; // need to be 1  more than specified as Q-J is 0 gap, but 1 rank difference
                
                // can be any rank and suit as long as rank gap is ok
                rankFloor[cur] = Card::Two;
                rankCeil[cur] = Card::Ace;
                suitFloor[cur] = 0;
                suitCeil[cur] = 3;

                seenCards++;
            }
            else
                goto error;
        }
        else if (NULL != strchr("AaKkQqJjTt98765432BbRrFfMmZzLlNnYyXx", *p))
        {
            if (NULL != strchr("AaKkQqJjTt98765432", *p))
            {
                if (p[1] == '-')
                {
                    rankFloor[cur] = Card::CharToRank(*p);
                    p++; while (*p == ' ') p++;
                    p++; while (*p == ' ') p++;
                    if (NULL != strchr("AaKkQqJjTt98765432", *p))
                        rankCeil[cur] = Card::CharToRank(*p);
                    else
                        goto error;
                }
                else if (p[1] == '+')
                {
                    rankFloor[cur] = Card::CharToRank(*p);
                    p++; while (*p == ' ') p++;
                    p++; while (*p == ' ') p++;
                    rankCeil[cur] = Card::Ace;
                }
                else
                {
                    rankFloor[cur] = Card::CharToRank(*p);
                    rankCeil[cur] = rankFloor[cur];
                    gap[cur] = -1; // any
                    p++; while (*p == ' ') p++;
                }
            }
            else
            {
                switch (*p)
                {
                    case 'B':
                    case 'b':
                        rankFloor[cur] = Card::Jack;
                        rankCeil[cur] = Card::Ace;
                        break;
                    case 'R':
                    case  'r':
                        rankFloor[cur] = Card::Ten;
                        rankCeil[cur] = Card::Ace;
                        break;
                    case 'F':
                    case 'f':
                        rankFloor[cur] = Card::Jack;
                        rankCeil[cur] = Card::King;
                        break;
                    case 'M':
                    case 'm':
                        rankFloor[cur] = Card::Seven;
                        rankCeil[cur] = Card::Ten;
                        break;
                    case 'Z':
                    case 'z':
                        rankFloor[cur] = Card::Two;
                        rankCeil[cur] = Card::Six;
                        break;
                    case 'L':
                    case 'l':
                        rankFloor[cur] = Card::Two;
                        rankCeil[cur] = Card::Eight;
                        break;
                    case 'N':
                    case 'n':
                        rankFloor[cur] = Card::Nine;
                        rankCeil[cur] = Card::King;
                        break;
                    case 'Y':
                    case 'y':
                        rankFloor[cur] = Card::Two;
                        rankCeil[cur] = Card::Five;
                        break;
                    case 'X':
                    case 'x':
                        rankFloor[cur] = Card::Two;
                        rankCeil[cur] = Card::Ace;
                        break;
                    default:
                        goto error;
                }
                    
                p++; while (*p == ' ') p++;
            }
                
            if (*p != '\0' && NULL != strchr("CcDdHhSs", *p))
            {
                if (isSuited)
                    goto error;

                curSuit[cur] = Specific;
                suitCeil[cur] = Card::CharToSuit(*p);
                suitFloor[cur] = Card::CharToSuit(*p);

                p++; while (*p == ' ') p++;
            }
            else
            {
                suitCeil[cur] = 3; // max suit
                suitFloor[cur] = 0; // min suit

                if (isSuited)
                {
                    if (curSuit[cur] != New)
                        curSuit[cur] = Current;
                }
                else
                    curSuit[cur] = Any;
            }
            seenCards++;
        }
        else
            goto error;
    }

    if (seenCards < 4)
    {
        printf("Need to specify 4 cards per hand\n");
        goto error;
    }

    // // filters
    // if (*p == '/')
    // {
    //     while (*p != '\0')
    //     {
    //         // handle filters
    //         ++p; while (*p == ' ') p++;
    //     }
    // }
    // else
    // {
    //     goto error;
    // }

    StdDeck_CardMask card1, card2, card3, card4;

    StdDeck_CardMask_RESET(hand);

    // first card
    for (int rank0 = rankFloor[0]; rank0 <= rankCeil[0]; rank0++)
    {
        StdDeck_CardMask used1;
        StdDeck_CardMask_RESET(used1);

        for(int suit0 = suitFloor[0]; suit0 <= suitCeil[0]; suit0++)
        {
            card1 = StdDeck_MASK( StdDeck_MAKE_CARD(rank0, suit0) );
            StdDeck_CardMask_OR(used1, used1, card1);

            // second card
            for (int rank1 = rankFloor[1]; rank1 <= rankCeil[1]; rank1++)
            {
                StdDeck_CardMask used2;
                StdDeck_CardMask_RESET(used2);

                if (gap[1] > 0 && gap[1] != (rank0-rank1)) continue;

                for(int suit1 = suitFloor[1]; suit1 <= suitCeil[1]; suit1++)
                {
                    switch (curSuit[1])
                    {
                        case New:
                            if (suit1 == suit0)
                                continue; // we want only mismatched suits
                            break;
                        case Current:
                            if (suit1 != suit0)
                                continue; // we want only matching suits
                            break;
                        case Specific:
                            break;
                        case Any:
                            break;
                    }
                    
                    card2 = StdDeck_MASK( StdDeck_MAKE_CARD(rank1, suit1) );
                    if (!StdDeck_CardMask_ANY_SET(used1, card2))
                    {
                        StdDeck_CardMask_OR(used2, used1, card2);
                    }
                    else
                        continue; // in use card
                    
                    // third card
                    for (int rank2 = rankFloor[2]; rank2 <= rankCeil[2]; rank2++)
                    {
                        StdDeck_CardMask used3;
                        StdDeck_CardMask_RESET(used3);

                        if (gap[2] > 0 && gap[2] != (rank1-rank2)) continue;

                        for(int suit2 = suitFloor[2]; suit2 <= suitCeil[2]; suit2++)
                        {
                            switch (curSuit[2])
                            {
                                case New:
                                    if (suit2 == suit0 || suit2 == suit1)
                                        continue; // we want only mismatched suits
                                    break;
                                case Current:
                                    if (suit2 != suit1)
                                        continue; // we want only matching suits
                                    break;
                                case Specific:
                                    break;
                                case Any:
                                    break;
                            }

                            card3 = StdDeck_MASK( StdDeck_MAKE_CARD(rank2, suit2) );
                            if (!StdDeck_CardMask_ANY_SET(used2, card3))
                            {
                                StdDeck_CardMask_OR(used3, used2, card3);
                            }
                            else
                                continue; // in use card

                            // fourth card
                            for (int rank3 = rankFloor[3]; rank3 <= rankCeil[3]; rank3++)
                            {
                                if (gap[3] > 0 && gap[3] != (rank2-rank3)) continue;

                                for(int suit3 = suitFloor[3]; suit3 <= suitCeil[3]; suit3++)
                                {
                                    switch (curSuit[3])
                                    {
                                        case New:
                                            if (suit3 == suit0 || suit3 == suit1 || suit3 == suit2)
                                                continue; // we want only mismatched suits
                                            break;
                                        case Current:
                                            if (suit3 != suit2)
                                                continue; // we want only matching suits
                                            break;
                                        case Specific:
                                            break;
                                        case Any:
                                            break;
                                    }
                                    card4 = StdDeck_MASK( StdDeck_MAKE_CARD(rank3, suit3) );
                                    if (StdDeck_CardMask_ANY_SET(used3, card4))
                                        continue; // in use card

                                    StdDeck_CardMask_OR(hand, hand, card1);
                                    StdDeck_CardMask_OR(hand, hand, card2);
                                    StdDeck_CardMask_OR(hand, hand, card3);
                                    StdDeck_CardMask_OR(hand, hand, card4);
                                    if (!StdDeck_CardMask_ANY_SET(deadCards, hand))
                                    {
                                        // if (!filtered(hand)...
                                        specificHands.push_back(hand);
                                        printf("HandText: %s ", handText);
                                        StdDeck_printMask(hand);
                                        printf("\n");
                                        combos++;
                                        StdDeck_CardMask_RESET(hand);
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

  error:
    printf("Error parsing %s, parsing at %s\n", handText, p);
    return -1;
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
