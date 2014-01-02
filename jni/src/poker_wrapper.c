/*
 *  Copyright 2006 Loic Dachary <loic@dachary.org> 
 *
 * This program gives you software freedom; you can copy, convey,
 * propagate, redistribute and/or modify this program under the terms of
 * the GNU General Public License (GPL) as published by the Free Software
 * Foundation (FSF), either version 3 of the License, or (at your option)
 * any later version of the GPL published by the FSF.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program in a file in the toplevel directory called "GPLv3".
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <poker_defs.h>
#include <poker_wrapper.h>

unsigned int wrap_StdDeck_N_CARDS(void) { return StdDeck_N_CARDS; }

StdDeck_CardMask wrap_StdDeck_MASK(int index) { return StdDeck_MASK(index); }

unsigned int wrap_StdDeck_Rank_2(void) { return StdDeck_Rank_2; }
unsigned int wrap_StdDeck_Rank_3(void) { return StdDeck_Rank_3; }
unsigned int wrap_StdDeck_Rank_4(void) { return StdDeck_Rank_4; }
unsigned int wrap_StdDeck_Rank_5(void) { return StdDeck_Rank_5; }
unsigned int wrap_StdDeck_Rank_6(void) { return StdDeck_Rank_6; }
unsigned int wrap_StdDeck_Rank_7(void) { return StdDeck_Rank_7; }
unsigned int wrap_StdDeck_Rank_8(void) { return StdDeck_Rank_8; }
unsigned int wrap_StdDeck_Rank_9(void) { return StdDeck_Rank_9; }
unsigned int wrap_StdDeck_Rank_TEN(void) { return StdDeck_Rank_TEN; }
unsigned int wrap_StdDeck_Rank_JACK(void) { return StdDeck_Rank_JACK; }
unsigned int wrap_StdDeck_Rank_QUEEN(void) { return StdDeck_Rank_QUEEN; }
unsigned int wrap_StdDeck_Rank_KING(void) { return StdDeck_Rank_KING; }
unsigned int wrap_StdDeck_Rank_ACE(void) { return StdDeck_Rank_ACE; }
unsigned int wrap_StdDeck_Rank_COUNT(void) { return StdDeck_Rank_COUNT; }
unsigned int wrap_StdDeck_Rank_FIRST(void) { return StdDeck_Rank_FIRST; }
unsigned int wrap_StdDeck_Rank_LAST(void) { return StdDeck_Rank_LAST; }
unsigned int wrap_StdDeck_RANK(unsigned int index) { return StdDeck_RANK(index); }
unsigned int wrap_StdDeck_SUIT(unsigned int index) { return StdDeck_SUIT(index); }
unsigned int wrap_StdDeck_MAKE_CARD(unsigned int rank, unsigned int suit) { return StdDeck_MAKE_CARD(rank, suit); }
unsigned int wrap_StdDeck_Suit_HEARTS(void) { return StdDeck_Suit_HEARTS; }
unsigned int wrap_StdDeck_Suit_DIAMONDS(void) { return StdDeck_Suit_DIAMONDS; }
unsigned int wrap_StdDeck_Suit_CLUBS(void) { return StdDeck_Suit_CLUBS; }
unsigned int wrap_StdDeck_Suit_SPADES(void) { return StdDeck_Suit_SPADES; }
unsigned int wrap_StdDeck_Suit_FIRST(void) { return StdDeck_Suit_FIRST; }
unsigned int wrap_StdDeck_Suit_LAST(void) { return StdDeck_Suit_LAST; }
unsigned int wrap_StdDeck_Suit_COUNT(void) { return StdDeck_Suit_COUNT; }

unsigned int wrap_StdDeck_CardMask_SPADES(StdDeck_CardMask cm) { return StdDeck_CardMask_SPADES(cm); }
unsigned int wrap_StdDeck_CardMask_CLUBS(StdDeck_CardMask cm) { return StdDeck_CardMask_CLUBS(cm); }
unsigned int wrap_StdDeck_CardMask_DIAMONDS(StdDeck_CardMask cm) { return StdDeck_CardMask_DIAMONDS(cm); }
unsigned int wrap_StdDeck_CardMask_HEARTS(StdDeck_CardMask cm) { return StdDeck_CardMask_HEARTS(cm); }
StdDeck_CardMask wrap_StdDeck_CardMask_SET_HEARTS(StdDeck_CardMask cm, unsigned int ranks) { StdDeck_CardMask_SET_HEARTS(cm, ranks); return cm; }
StdDeck_CardMask wrap_StdDeck_CardMask_SET_DIAMONDS(StdDeck_CardMask cm, unsigned int ranks) { StdDeck_CardMask_SET_DIAMONDS(cm, ranks); return cm; }
StdDeck_CardMask wrap_StdDeck_CardMask_SET_CLUBS(StdDeck_CardMask cm, unsigned int ranks) { StdDeck_CardMask_SET_CLUBS(cm, ranks); return cm; }
StdDeck_CardMask wrap_StdDeck_CardMask_SET_SPADES(StdDeck_CardMask cm, unsigned int ranks) { StdDeck_CardMask_SET_SPADES(cm, ranks); return cm; }
StdDeck_CardMask wrap_StdDeck_CardMask_NOT(StdDeck_CardMask cm) { StdDeck_CardMask_NOT(cm, cm); return cm; }
StdDeck_CardMask wrap_StdDeck_CardMask_OR(StdDeck_CardMask op1, StdDeck_CardMask op2) { StdDeck_CardMask_OR(op1, op1, op2); return op1; } 
StdDeck_CardMask wrap_StdDeck_CardMask_AND(StdDeck_CardMask op1, StdDeck_CardMask op2) { StdDeck_CardMask_AND(op1, op1, op2); return op1; } 
StdDeck_CardMask wrap_StdDeck_CardMask_XOR(StdDeck_CardMask op1, StdDeck_CardMask op2) { StdDeck_CardMask_XOR(op1, op1, op2); return op1; } 
StdDeck_CardMask wrap_StdDeck_CardMask_SET(StdDeck_CardMask mask, unsigned int index) { StdDeck_CardMask_SET(mask, index); return mask; } ;
StdDeck_CardMask wrap_StdDeck_CardMask_UNSET(StdDeck_CardMask mask, unsigned int index) { StdDeck_CardMask_UNSET(mask, index); return mask; }
int wrap_StdDeck_CardMask_CARD_IS_SET(StdDeck_CardMask mask, unsigned int index) { return StdDeck_CardMask_CARD_IS_SET(mask, index); }
int wrap_StdDeck_CardMask_ANY_SET(StdDeck_CardMask mask1, StdDeck_CardMask mask2) { return StdDeck_CardMask_ANY_SET(mask1, mask2); }
StdDeck_CardMask wrap_StdDeck_CardMask_RESET(void) { StdDeck_CardMask mask; StdDeck_CardMask_RESET(mask); return mask; }
int wrap_StdDeck_CardMask_IS_EMPTY(StdDeck_CardMask mask) { return StdDeck_CardMask_IS_EMPTY(mask); }
int wrap_StdDeck_CardMask_EQUAL(StdDeck_CardMask mask1, StdDeck_CardMask mask2) { return StdDeck_CardMask_EQUAL(mask1, mask2); }
