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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <assert.h>

#include <poker_defs.h>
#include <enumdefs.h>

static int verbose = 1;

static CardMask Strings2CardMask(int strings_count, char* strings[])
{
  CardMask cards;
  CardMask dead;

  CardMask_RESET(cards);
  CardMask_RESET(dead);

  int card;
  int i;
  for(i = 0; i < strings_count; i++) {
    card = -1;
    assert(Deck_stringToCard(strings[i], &card) != 0);
    assert(StdDeck_CardMask_CARD_IS_SET(dead, card) == 0);
    StdDeck_CardMask_SET(cards, card);
  }

  return cards;
}

int main(int argc, char* argv[]) {
  enum_game_t game = game_razz;
  {
    enum_result_t result;
    StdDeck_CardMask board;
    StdDeck_CardMask dead;
    StdDeck_CardMask pockets[2];
    char* hand0[] = { "Ad", "2d", "3d", "4d", "5d", "Tc", "Th" };
    //char* hand1[] = { "Ac", "2c", "3c", "4c", "5c", "Tc", "Th" };
    char* hand1[] = { "4d", "4c", "8d", "8c", "9d", "9c", "9h" };

    enumResultClear(&result);
    CardMask_RESET(board);
    CardMask_RESET(dead);
    pockets[0] = Strings2CardMask(7, hand0);
    pockets[1] = Strings2CardMask(7, hand1);

    assert(enumExhaustive(game, pockets, board, dead, 2, 0 /* nboard */,
                          0 /* orderflag */, &result) == 0);

    if(verbose) enumResultPrint(&result, pockets, board);

    assert(result.ev[0] == 1.0);
    assert(result.ev[1] == 0.0);
  }
  
  {
    /* http://shipitfish.livejournal.com/59671.html 

        If after removing straights and flushes, if hand X beats hand
        Y under normal poker rules, the hand Y beats hand X under
        razz.

        There should *never* be a case where one hand wins over
        another in both normal poker rules and razz (after removing
        straights and flushes).

        Therefore, 6s full of 5s loses 4s full of 8s in razz and
        likewise by the same logic 6s and 5s beat 4s and 8s in razz.

        4s and 8s should definately *NOT* beat 6s and 5s in both
        7-card stud and razz. If one hand wins in 7-card stud, the
        other hand wins in razz. It is that simple.

     */
    
    enum_result_t result;
    StdDeck_CardMask board;
    StdDeck_CardMask dead;
    StdDeck_CardMask pockets[2];
    char* hand0[] = { "5d", "5c", "6d", "6c", "7d", "7c", "7h" };
    char* hand1[] = { "4d", "4c", "8d", "8c", "9d", "9c", "9h" };

    enumResultClear(&result);
    CardMask_RESET(board);
    CardMask_RESET(dead);
    pockets[0] = Strings2CardMask(7, hand0);
    pockets[1] = Strings2CardMask(7, hand1);

    assert(enumExhaustive(game, pockets, board, dead, 2, 0 /* nboard */,
                          0 /* orderflag */, &result) == 0);

    if(verbose) enumResultPrint(&result, pockets, board);

    assert(result.ev[0] == 1.0);
    assert(result.ev[1] == 0.0);
  }

  return 0;
}
