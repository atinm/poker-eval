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

#pragma once

///////////////////////////////////////////////////////////////////////////////
// A distribution containing one or more specific Omaha hands. We create
// one of these for each player involved in the matchup, EVEN IF THE PLAYER
// HAS A SPECIFIC HAND. A specific hand is just a distribution containing
// exactly *one* hand.
///////////////////////////////////////////////////////////////////////////////
class OmahaHandDistribution
{
public:
	OmahaHandDistribution();
	OmahaHandDistribution(const char* hand);
	OmahaHandDistribution(const char* hand, StdDeck_CardMask deadCards);
	virtual ~OmahaHandDistribution(void);

	int Init(const char* hand);
	int Init(const char* hand, StdDeck_CardMask dead);
	StdDeck_CardMask Choose(StdDeck_CardMask deadCards, bool& bCollisionError);
	StdDeck_CardMask Get(int index) const { return m_hands[index]; }
	StdDeck_CardMask Current() const { return m_current; }
	void SetCurrent( StdDeck_CardMask cur) { m_current = cur; }
	const char* GetText() const { return m_handText.c_str(); }


	static bool IsSpecificHand(const char* handText);
	int GetCount() const { return m_hands.size(); }
	bool IsUnary() const { return m_hands.size() == 1; }

	friend class OmahaCalculator; // terrible programmer...

private:
	static bool CardMaskGreaterThan( StdDeck_CardMask a, StdDeck_CardMask b );
	static bool CardMaskEqual( StdDeck_CardMask a, StdDeck_CardMask b );
	OmahaHandDistribution* Next() const { return m_pNext; }

	string m_handText;
	OmahaHandDistribution* m_pNext;
	vector<StdDeck_CardMask> m_hands;
	StdDeck_CardMask m_current;
};
