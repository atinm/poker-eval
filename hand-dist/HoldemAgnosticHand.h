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

///////////////////////////////////////////////////////////////////////////////
//
// An "agnostic hand" is a Texas Hold'em starting hand devoid of specific
// suit information -- for example, "AKs" or "T9o" or "33". A given agnostic
// hand boils down to N *specific* hands, for example, AKs consists of:
//
//			- AsKs
//			- AhKh
//			- AdKd
//			- AcKc
//
// An agnostic hand can also include range information. For example:
//
//			- A2s+
//			- 77+
//			- JJ-88
//			- T8o-64o
// 
///////////////////////////////////////////////////////////////////////////////
class HoldemAgnosticHand
{
public:
	static int Parse(const char* handText, const char* deadCards);
	static int Parse(const char* handText, StdDeck_CardMask deadCards);

	static int Instantiate(const char* handText, const char* deadCards, vector<StdDeck_CardMask>& hands);
	static int Instantiate(const char* handText, StdDeck_CardMask deadCards, vector<StdDeck_CardMask>& hands);

	static bool IsSpecificHand(const char* handText);

private:

	static int InstantiateRandom(StdDeck_CardMask deadCards, vector<StdDeck_CardMask>& specificHands);
	static bool IsSuited(const char*);
	static bool IsOffSuit(const char*);
	static bool IsInclusive(const char*);
	static bool IsPair(const char*);
};
