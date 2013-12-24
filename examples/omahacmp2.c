/*
 *  omahacmp2.c: a program to compare two pairs of omaha hole cards at any
 *           point of the game (pre-flop, on the flop, turn or river).
 *              
 *  Example:
 *
 *      omahacmp2  tc ac  3h ah  8c 6h 7h
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "poker_defs.h"
#include "inlines/eval_omaha.h"

int monteCarlo = 0;
int gNCommon, gNDead, gNIter;
CardMask gDeadCards, gCommonCards, gPlayerCards[2];


static void
parseArgs(int argc, char **argv) {
    int i, count = 0, c;

    for (i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "-d") == 0) {
                if (++i == argc) goto error;
                if (StdDeck_stringToCard(argv[i], &c) == 0)
                    goto error;
                if (!CardMask_CARD_IS_SET(gDeadCards, c)) {
                    ++gNDead;
                    StdDeck_CardMask_SET(gDeadCards, c);
                };
            } 
            else if (strcmp(argv[i], "-m") == 0) {
                if (++i == argc) goto error;
                gNIter = strtol(argv[i], NULL, 10);
                if (gNIter == EINVAL || gNIter == ERANGE) {
                    printf("bad option m: %s\n", argv[i]);
                    goto error;
                }
                monteCarlo = 1;
            }
            else 
                goto error;
        } else {
            if (StdDeck_stringToCard(argv[i], &c) == 0)
                goto error;
            if (count < 4) 
                StdDeck_CardMask_SET(gPlayerCards[0], c);
            else if (count < 8) 
                StdDeck_CardMask_SET(gPlayerCards[1], c);
            else {
                StdDeck_CardMask_SET(gCommonCards, c);
                ++gNCommon;
            };
            ++count;
        }
    }
    if (count < 8) goto error;
    if (gNCommon > 5) goto error;

    return;

  error:
    fprintf(stderr, "Usage: omahacmp2 [ -d dead-card ] p1-cards p2-cards [ common-cards ]\n");
    exit(0);
}


int main( int argc, char *argv[] )
{
    CardMask cards, board;
    HandVal h0, h1;
    int h0_count=0, h1_count=0, tie_count=0, count=0;

    CardMask_RESET(gDeadCards);
    CardMask_RESET(gCommonCards);
    CardMask_RESET(gPlayerCards[0]);
    CardMask_RESET(gPlayerCards[1]);
    parseArgs(argc, argv);

    CardMask_OR(gDeadCards, gDeadCards, gCommonCards);
    CardMask_OR(gDeadCards, gDeadCards, gPlayerCards[0]);
    CardMask_OR(gDeadCards, gDeadCards, gPlayerCards[1]);

    if (!monteCarlo) {
        ENUMERATE_N_CARDS_D(cards, 5-gNCommon, gDeadCards, 
                            {
                                ++count;
                                CardMask_OR(board, gCommonCards, cards);

                                int ret = StdDeck_OmahaHi_EVAL(gPlayerCards[0], board, &h0);
                                if (ret) {
                                    printf("Error calculating OmahaHi: %d\n", ret);
                                    exit(-1);
                                }

                                ret = StdDeck_OmahaHi_EVAL(gPlayerCards[1], board, &h1);
                                if (ret) {
                                    printf("Error calculating OmahaHi: %d\n", ret);
                                    exit(-1);
                                }

                                if (h0 > h1)
                                    ++h0_count;
                                else if (h1 > h0)
                                    ++h1_count;
                                else
                                    ++tie_count;
                            }
                            );
    }
    else {
        /* Monte-Carlo simulation */
        DECK_MONTECARLO_N_CARDS_D(StdDeck, cards, gDeadCards, 5-gNCommon, gNIter,
                                  {
                                      ++count;
                                      CardMask_OR(board, gCommonCards, cards);

                                      int ret = StdDeck_OmahaHi_EVAL(gPlayerCards[0], board, &h0);
                                      if (ret) {
                                          printf("Error calculating OmahaHi: %d\n", ret);
                                          exit(-1);
                                      }

                                      ret = StdDeck_OmahaHi_EVAL(gPlayerCards[1], board, &h1);
                                      if (ret) {
                                          printf("Error calculating OmahaHi: %d\n", ret);
                                          exit(-1);
                                      }

                                      if (h0 > h1)
                                          ++h0_count;
                                      else if (h1 > h0)
                                          ++h1_count;
                                      else
                                          ++tie_count;
                                  }
                                  );
    }
    printf("%d boards", count);
    if (gNCommon > 0) 
        printf(" containing %s ", Deck_maskString(gCommonCards));
    if (gNDead) 
        printf(" with %s removed ", Deck_maskString(gDeadCards));
    printf("\n");

    printf("  cards            win  %%win       loss  %%lose       tie  %%tie      EV\n");
    printf("  %s  %7d %6.2f   %7d %6.2f   %7d %6.2f     %5.2f%%\n", 
           Deck_maskString(gPlayerCards[0]), 
           h0_count, 100.0*h0_count/count, 
           h1_count, 100.0*h1_count/count, 
           tie_count, 100.0*tie_count/count, 
           ((1.0*h0_count + (tie_count/2.0)) / count) * 100.0);

    printf("  %s  %7d %6.2f   %7d %6.2f   %7d %6.2f     %5.2f%%\n", 
           Deck_maskString(gPlayerCards[1]), 
           h1_count, 100.0*h1_count/count, 
           h0_count, 100.0*h0_count/count, 
           tie_count, 100.0*tie_count/count, 
           ((1.0*h1_count + (tie_count/2.0)) / count) * 100.0);

    return 0;
}
