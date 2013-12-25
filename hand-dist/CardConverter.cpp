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
#include "CardConverter.h"

StdDeck_CardMask CardConverter::PokerEvalCards[53] = 
{ 
    0,
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_2, StdDeck_Suit_CLUBS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_3, StdDeck_Suit_CLUBS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_4, StdDeck_Suit_CLUBS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_5, StdDeck_Suit_CLUBS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_6, StdDeck_Suit_CLUBS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_7, StdDeck_Suit_CLUBS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_8, StdDeck_Suit_CLUBS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_9, StdDeck_Suit_CLUBS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_TEN, StdDeck_Suit_CLUBS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_JACK, StdDeck_Suit_CLUBS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_QUEEN, StdDeck_Suit_CLUBS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_KING, StdDeck_Suit_CLUBS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_ACE, StdDeck_Suit_CLUBS)),

    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_2, StdDeck_Suit_DIAMONDS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_3, StdDeck_Suit_DIAMONDS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_4, StdDeck_Suit_DIAMONDS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_5, StdDeck_Suit_DIAMONDS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_6, StdDeck_Suit_DIAMONDS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_7, StdDeck_Suit_DIAMONDS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_8, StdDeck_Suit_DIAMONDS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_9, StdDeck_Suit_DIAMONDS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_TEN, StdDeck_Suit_DIAMONDS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_JACK, StdDeck_Suit_DIAMONDS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_QUEEN, StdDeck_Suit_DIAMONDS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_KING, StdDeck_Suit_DIAMONDS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_ACE, StdDeck_Suit_DIAMONDS)),

    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_2, StdDeck_Suit_HEARTS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_3, StdDeck_Suit_HEARTS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_4, StdDeck_Suit_HEARTS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_5, StdDeck_Suit_HEARTS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_6, StdDeck_Suit_HEARTS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_7, StdDeck_Suit_HEARTS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_8, StdDeck_Suit_HEARTS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_9, StdDeck_Suit_HEARTS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_TEN, StdDeck_Suit_HEARTS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_JACK, StdDeck_Suit_HEARTS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_QUEEN, StdDeck_Suit_HEARTS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_KING, StdDeck_Suit_HEARTS)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_ACE, StdDeck_Suit_HEARTS)),

    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_2, StdDeck_Suit_SPADES)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_3, StdDeck_Suit_SPADES)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_4, StdDeck_Suit_SPADES)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_5, StdDeck_Suit_SPADES)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_6, StdDeck_Suit_SPADES)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_7, StdDeck_Suit_SPADES)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_8, StdDeck_Suit_SPADES)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_9, StdDeck_Suit_SPADES)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_TEN, StdDeck_Suit_SPADES)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_JACK, StdDeck_Suit_SPADES)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_QUEEN, StdDeck_Suit_SPADES)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_KING, StdDeck_Suit_SPADES)),
    StdDeck_MASK(StdDeck_MAKE_CARD(StdDeck_Rank_ACE, StdDeck_Suit_SPADES))
};

