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

#pragma once

///////////////////////////////////////////////////////////////////////////////
// Single hands
// To specify a single hand for example type AsQc7h3d
//
// All combos of a hand
// To get all 16 combos of a hand, for example an A, K, Q and a J, enter 
// AKQJ without suits.
//
// Suits
// To specify suited cards, enclosing them in square brackets, e.g. [AK]xx will
// give all hands containing suited AK and two other cards.
// [AK][AK] will give A's and K's double suited. [JT][98] will give double
// suited JT98 hands.
// [XXX]X will give all hands with three of a suit.
// [X][X][X][X] will give all hands with four different suits (rainbox).
// To specify off-suit cards, enclose one of the cards in square brackets, e.g.
// [A]KQJ will give all hands with an A of a different suit from all other cards.
// [Ah]KQJ will give all hands with an Ah and all other cards of a different suit.
//
// Pairs
// QQxx will give all hands containing two queens
// [QT+][QT+] double suited queens with two other T+ cards.
//
// Card types
// x = random, all ranks, 2..A
// 2 .. T J Q K A
// [] = group suited cards
//
// Ranges from high to low ranks, e.g. for each of the four cards, you can
// enter a range, e.g. K-Txxx would give Txxx, Jxxx Qxxx, Kxxx, Axxx.
//
// Ranges can be combined for all four cards, e.g. 3+9-7Q+K-T where first card is 3 or
// better, second card is between 7-9, third card is greater than or equal to Q, and
// the fourth cards is between T and K.
//
// Percentile (todo)
// 15% would give the top 15% of hands (ProPokerTools ranking).
// 10-25% would give the top 10% to 25% of hands (ProPokerTools ranking).
//
///////////////////////////////////////////////////////////////////////////////

class OmahaAgnosticHand
{
public:
	static int Instantiate(const char* handText, const char* deadCards, vector<StdDeck_CardMask>& hands);
	static int Instantiate(const char* handText, StdDeck_CardMask deadCards, vector<StdDeck_CardMask>& hands);

private:
	static int InstantiateRandom(StdDeck_CardMask deadCards, vector<StdDeck_CardMask>& specificHands);
};
