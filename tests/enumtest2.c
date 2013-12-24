/*
 *  Copyright 2006 Michael Maurer <mjmaurer@yahoo.com>, 
 *                 Brian Goetz <brian@quiotix.com>, 
 *                 Loic Dachary <loic@dachary.org>, 
 *                 Tim Showalter <tjs@psaux.com>
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
/* $Id: enumtest2.c 5146 2008-12-04 03:11:22Z bkuhn $
   enumtest1.c -- test enumerate macros
*/

#include	<stdio.h>
#include	<stdlib.h>
#include	"poker_defs.h"
#include        "enumerate.h"

#define NPLAYERS 3

int main(void)
{
  int niter;
  StdDeck_CardMask set_var[NPLAYERS];
  int num_sets = NPLAYERS;
  int set_sizes[NPLAYERS];
  StdDeck_CardMask dead_cards;
  StdDeck_CardMask_RESET(dead_cards);

  set_sizes[0] = 1;
  set_sizes[1] = 0;
  set_sizes[2] = 0;
  niter = 0;
  DECK_ENUMERATE_PERMUTATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("PERMUMATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);
  niter = 0;
  DECK_ENUMERATE_COMBINATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("COMBINATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);

  set_sizes[0] = 2;
  set_sizes[1] = 0;
  set_sizes[2] = 0;
  niter = 0;
  DECK_ENUMERATE_PERMUTATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("PERMUTATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);
  niter = 0;
  DECK_ENUMERATE_COMBINATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("COMBINATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);

  set_sizes[0] = 1;
  set_sizes[1] = 1;
  set_sizes[2] = 0;
  niter = 0;
  DECK_ENUMERATE_PERMUTATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("PERMUTATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);
  niter = 0;
  DECK_ENUMERATE_COMBINATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("COMBINATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);

  set_sizes[0] = 1;
  set_sizes[1] = 2;
  set_sizes[2] = 0;
  niter = 0;
  DECK_ENUMERATE_PERMUTATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("PERMUTATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);
  niter = 0;
  DECK_ENUMERATE_COMBINATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("COMBINATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);

  set_sizes[0] = 1;
  set_sizes[1] = 3;
  set_sizes[2] = 0;
  niter = 0;
  DECK_ENUMERATE_PERMUTATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("PERMUTATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);
  niter = 0;
  DECK_ENUMERATE_COMBINATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("COMBINATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);

  set_sizes[0] = 1;
  set_sizes[1] = 1;
  set_sizes[2] = 1;
  niter = 0;
  DECK_ENUMERATE_PERMUTATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("PERMUTATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);
  niter = 0;
  DECK_ENUMERATE_COMBINATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("COMBINATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);

  set_sizes[0] = 1;
  set_sizes[1] = 1;
  set_sizes[2] = 2;
  niter = 0;
  DECK_ENUMERATE_PERMUTATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("PERMUTATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);
  niter = 0;
  DECK_ENUMERATE_COMBINATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("COMBINATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);
  return 0;
}


