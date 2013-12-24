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
/* $Id: enumtest3.c 6030 2009-07-07 07:23:30Z loic $ */

#include <stdio.h>
#include <stdlib.h>
#include "poker_defs.h"
#include "enumerate.h"

int main(void) {
  StdDeck_CardMask set_var[2];
  int num_sets = 2;
  int set_sizes[2] = {2, 1};
  StdDeck_CardMask dead_cards;
  int i;
  
  StdDeck_CardMask_RESET(dead_cards);
  for (i=0; i<39; i++) /* remove three suits to reduce output */
    StdDeck_CardMask_SET(dead_cards, i);
  DECK_ENUMERATE_COMBINATIONS_D(StdDeck, set_var, num_sets,
                                set_sizes, dead_cards,
  { 
    for (i=0; i<num_sets; i++) { 
      printf("%s | ", DmaskString(StdDeck, set_var[i])); 
    } 
    printf("\n"); 
  } 
    );
  return 0;
}
