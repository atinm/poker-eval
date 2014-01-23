/*
 * Copyright (C) 2002 Michael Maurer <mjmaurer@yahoo.com>
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
/* eval_omaha.h -- Omaha High and Omaha High/Low 8-or-better hand evaluators
 *
 * Uses brute-force enumeration of legal hole-card board-card combinations,
 * and is probably at least 60 times slower than Holdem evaluation.  A legal
 * hole-card board-card combination uses exactly 2 hole cards and 3 board
 * cards.
 *
 * Michael Maurer, Mar 2002.
 */

#ifndef __EVAL_OMAHA_H__
#define __EVAL_OMAHA_H__

#include <errno.h>
#include <string.h>

#include "inlines/eval.h"
#include "inlines/eval_low8.h"
#include "deck_std.h"
#include "rules_std.h"

extern int omahaHiHandRanks[5720184];
extern int omahaLow8HandRanks[5720184];

#define OMAHA_MINHOLE 4		/* must be at least 2 */
#define OMAHA_MAXHOLE 4		/* could be larger */
#define OMAHA_MINBOARD 3	/* must be at least 3 */
#define OMAHA_MAXBOARD 5	/* could be larger */

/* Evaluate an omaha hand for both high and low.  Return nonzero on error.
   If hival is NULL, skips high evaluation; if loval is NULL, skips
   low evaluation.  Low eval could be sped up with 256x256 rank table. */

static inline int
StdDeck_OmahaHiLow8_EVAL(StdDeck_CardMask hole, StdDeck_CardMask board,
                         HandVal *hival, LowHandVal *loval) {
  StdDeck_CardMask allcards;
  LowHandVal allval;
  HandVal curhi, besthi;
  LowHandVal curlo, bestlo;
  StdDeck_CardMask hole1[OMAHA_MAXHOLE];
  StdDeck_CardMask board1[OMAHA_MAXBOARD];
  StdDeck_CardMask n1, n2, n3, n4, n5;
  int nhole, nboard;
  int eligible = 0;
  int i, h1, h2, b1, b2, b3;

  /* pluck out individual cards from hole and board masks, save in arrays */
  nhole = nboard = 0;
  for (i=0; i<StdDeck_N_CARDS; i++) {
    if (StdDeck_CardMask_CARD_IS_SET(hole, i)) {
      if (nhole >= OMAHA_MAXHOLE)
        return 1;                               /* too many hole cards */
      StdDeck_CardMask_RESET(hole1[nhole]);
      StdDeck_CardMask_SET(hole1[nhole], i);
      nhole++;
    }
    if (StdDeck_CardMask_CARD_IS_SET(board, i)) {
      if (StdDeck_CardMask_CARD_IS_SET(hole, i)) /* same card in hole and board */
        return 2;
      if (nboard >= OMAHA_MAXBOARD)
        return 3;                               /* too many board cards */
      StdDeck_CardMask_RESET(board1[nboard]);
      StdDeck_CardMask_SET(board1[nboard], i);
      nboard++;
    }
  }

  if (nhole < OMAHA_MINHOLE || nhole > OMAHA_MAXHOLE)
    return 4;                                   /* wrong # of hole cards */
  if (nboard < OMAHA_MINBOARD || nboard > OMAHA_MAXBOARD)
    return 5;                                   /* wrong # of board cards */

  /* quick test in case no low is possible with all 9 cards */
  if (loval != NULL) {
    StdDeck_CardMask_OR(allcards, hole, board);
    allval = StdDeck_Lowball8_EVAL(allcards, nhole + nboard);
    eligible = (allval != LowHandVal_NOTHING);
  }

  /* loop over all combinations of hole with board (60 for 4 hole cards
     and 5 board cards). */
  besthi = HandVal_NOTHING;
  bestlo = LowHandVal_NOTHING;
  /* {h1,h2} loop over all hole card combinations */
  for (h1=0; h1<nhole-1; h1++) {
    StdDeck_CardMask_RESET(n1);
    StdDeck_CardMask_OR(n1, n1, hole1[h1]);
    for (h2=h1+1; h2<nhole; h2++) {
      StdDeck_CardMask_OR(n2, n1, hole1[h2]);
      /* {b1,b2,b3} loop over all board card combinations */
      for (b1=0; b1<nboard-2; b1++) {
        StdDeck_CardMask_OR(n3, n2, board1[b1]);
        for (b2=b1+1; b2<nboard-1; b2++) {
          StdDeck_CardMask_OR(n4, n3, board1[b2]);
          for (b3=b2+1; b3<nboard; b3++) {
	    StdDeck_CardMask_OR(n5, n4, board1[b3]);
	    if (hival != NULL) {
              curhi = StdDeck_StdRules_EVAL_N(n5, 5);
              if (curhi > besthi || besthi == HandVal_NOTHING)
                besthi = curhi;
            }
            if (loval != NULL && eligible) {
              curlo = StdDeck_Lowball8_EVAL(n5, 5);
              if (curlo < bestlo || bestlo == LowHandVal_NOTHING)
                bestlo = curlo;
            }
          }
        }
      }
    }
  }
  if (hival != NULL) *hival = besthi;
  if (loval != NULL) *loval = bestlo;
  return 0;
}

/* Evaluate an omaha hand for high only.  Return nonzero on error. */

static inline int
StdDeck_OmahaHi_EVAL(StdDeck_CardMask hole, StdDeck_CardMask board,
                     HandVal *hival) {
  return StdDeck_OmahaHiLow8_EVAL(hole, board, hival, NULL);
}

/* Evaluate an omaha hand for high only, using 2+2 evaluator.  Return nonzero on error. */
/* 
       Kev/2+2:
       2c =  1    2d =  2    2h =  3    2s =  4
       3c =  5    3d =  6    3h =  7    3s =  8
       4c =  9    4d = 10    4h = 11    4s = 12
       5c = 13    5d = 14    5h = 15    5s = 16
       6c = 17    6d = 18    6h = 19    6s = 20
       7c = 21    7d = 22    7h = 23    7s = 24
       8c = 25    8d = 26    8h = 27    8s = 28
       9c = 29    9d = 30    9h = 31    9s = 32
       Tc = 33    Td = 34    Th = 35    Ts = 36
       Jc = 37    Jd = 38    Jh = 39    Js = 40
       Qc = 41    Qd = 42    Qh = 43    Qs = 44
       Kc = 45    Kd = 46    Kh = 47    Ks = 48
       Ac = 49    Ad = 50    Ah = 51    As = 52

       Poker source and Kev order the suits in differently, so after
       my conversion of pokersource card index to Kev index:
       card = (4 * StdDeck_RANK(playerCards[index])) + StdDeck_SUIT(playerCards[index]) + 1
       gives me (this works for poker as suits aren't ordered)
       
       2h =  1    2d =  2    2c =  3    2s =  4
       3h =  5    3d =  6    3c =  7    3s =  8
       4h =  9    4d = 10    4c = 11    4s = 12
       5h = 13    5d = 14    5c = 15    5s = 16
       6h = 17    6d = 18    6c = 19    6s = 20
       7h = 21    7d = 22    7c = 23    7s = 24
       8h = 25    8d = 26    8c = 27    8s = 28
       9h = 29    9d = 30    9c = 31    9s = 32
       Th = 33    Td = 34    Tc = 35    Ts = 36
       Jh = 37    Jd = 38    Jc = 39    Js = 40
       Qh = 41    Qd = 42    Qc = 43    Qs = 44
       Kh = 45    Kd = 46    Kc = 47    Ks = 48
       Ah = 49    Ad = 50    Ac = 51    As = 52
*/

static inline int
StdDeck_OmahaHiLow8_EVAL_LUT(StdDeck_CardMask hole, StdDeck_CardMask board,
                             HandVal *hival, LowHandVal *loval) {
    int playerCards[4], nPlayerCards, boardCards[5], nBoardCards;
    int h1, h2, b1, b2, b3, i;
    int b11, b12, b13, h11, h12;
    int c11, c12, c13, l11, l12;
    HandVal besthi = HandVal_NOTHING;
    LowHandVal bestlo = LowHandVal_NOTHING;

    nPlayerCards = StdDeck.maskToCards(&hole, playerCards);
    for (i=0;i<nPlayerCards;i++) {
        playerCards[i] = (4 * StdDeck_RANK(playerCards[i])) + StdDeck_SUIT(playerCards[i]) + 1;
    }
    nBoardCards = StdDeck.maskToCards(&board, boardCards);
    for (i=0;i<nBoardCards;i++) {
        boardCards[i] = (4 * StdDeck_RANK(boardCards[i])) + StdDeck_SUIT(boardCards[i]) + 1;
    }
    if (nPlayerCards < OMAHA_MINHOLE || nPlayerCards > OMAHA_MAXHOLE)
        return 4;                                   /* wrong # of hole cards */
    if (nBoardCards < OMAHA_MINBOARD || nBoardCards > OMAHA_MAXBOARD)
        return 5;                                   /* wrong # of board cards */
    
    for (b1=0; b1<nBoardCards; b1++) {
        b11 =  omahaHiHandRanks[53 + boardCards[b1]];
        c11 =  omahaLow8HandRanks[53 + boardCards[b1]];
        for (b2=b1+1; b2<nBoardCards; b2++) {
            b12 = omahaHiHandRanks[b11 + boardCards[b2]];
            c12 = omahaLow8HandRanks[c11 + boardCards[b2]];
            for (b3=b2+1; b3<nBoardCards; b3++) {
                b13 = omahaHiHandRanks[b12 + boardCards[b3]];
                c13 = omahaLow8HandRanks[c12 + boardCards[b3]];
                for (h1=0; h1<nPlayerCards; h1++) {
                    h11 = omahaHiHandRanks[b13 + playerCards[h1]];
                    l11 = omahaLow8HandRanks[c13 + playerCards[h1]];
                    for (h2=h1+1; h2<nPlayerCards; h2++) {
                        h12 = omahaHiHandRanks[h11 + playerCards[h2]];
                        l12 = omahaLow8HandRanks[l11 + playerCards[h2]];
                        if (besthi == HandVal_NOTHING || h12 > besthi)
                            besthi = h12;
                        if (bestlo == LowHandVal_NOTHING || l12 < bestlo)
                            bestlo = l12;
                    }
                }
            }
        }
    }
    *hival = besthi;
    if (loval != NULL)
        *loval = bestlo;

    return 0;
}

static inline int
StdDeck_OmahaHi_EVAL_LUT(StdDeck_CardMask hole, StdDeck_CardMask board,
                     HandVal *hival) {
    int playerCards[4], nPlayerCards, boardCards[5], nBoardCards;
    int h1, h2, b1, b2, b3, i;
    int b11, b12, b13, h11, h12;
    HandVal besthi = HandVal_NOTHING;

    nPlayerCards = StdDeck.maskToCards(&hole, playerCards);
    for (i=0;i<nPlayerCards;i++) {
        playerCards[i] = (4 * StdDeck_RANK(playerCards[i])) + StdDeck_SUIT(playerCards[i]) + 1;
    }
    nBoardCards = StdDeck.maskToCards(&board, boardCards);
    for (i=0;i<nBoardCards;i++) {
        boardCards[i] = (4 * StdDeck_RANK(boardCards[i])) + StdDeck_SUIT(boardCards[i]) + 1;
    }
    if (nPlayerCards < OMAHA_MINHOLE || nPlayerCards > OMAHA_MAXHOLE)
        return 4;                                   /* wrong # of hole cards */
    if (nBoardCards < OMAHA_MINBOARD || nBoardCards > OMAHA_MAXBOARD)
        return 5;                                   /* wrong # of board cards */
    
    for (b1=0; b1<nBoardCards; b1++) {
        b11 =  omahaHiHandRanks[53 + boardCards[b1]];
        for (b2=b1+1; b2<nBoardCards; b2++) {
            b12 = omahaHiHandRanks[b11 + boardCards[b2]];
            for (b3=b2+1; b3<nBoardCards; b3++) {
                b13 = omahaHiHandRanks[b12 + boardCards[b3]];
                for (h1=0; h1<nPlayerCards; h1++) {
                    h11 = omahaHiHandRanks[b13 + playerCards[h1]];
                    for (h2=h1+1; h2<nPlayerCards; h2++) {
                        h12 = omahaHiHandRanks[h11 + playerCards[h2]];
                        if (besthi == HandVal_NOTHING || h12 > besthi)
                            besthi = h12;
                    }
                }
            }
        }
    }
    *hival = besthi;

    return 0;
}

#endif
