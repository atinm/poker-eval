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
#include <stdio.h>
#include "poker_defs.h"

/*
 * This number MUST be higher than any of the *_N_CARDS constants
 * listed in the *_deck.h files in the include directory, 52 for
 * a standard deck for instance. 
 */
#define STRING_CARDS 100

int
GenericDeck_maskToString(Deck *deck, void *cardMask, char *outString) {
  int cards[STRING_CARDS], n, i;
  char *p;

  n = (*deck->maskToCards)(cardMask, cards);
  
  p = outString;
  for (i=0; i<n; i++) {
    if (i > 0) 
      *p++ = ' ';
    p += (*deck->cardToString)(cards[i], p);
  };
  *p = '\0';
  return (outString - p);
}


int 
GenericDeck_printMask(Deck *deck, void *cardMask) {
  char outString[300];
  int r;

  r = GenericDeck_maskToString(deck, cardMask, outString);
  printf("%s", outString);
  return r;
}


char *
GenericDeck_maskString(Deck *deck, void *cardMask) {
  static thread char outString[150];

  GenericDeck_maskToString(deck, cardMask, outString);
  return outString;
}


int
GenericDeck_numCards(Deck *deck, void *cardMask) {
  return (*deck->numCards)(cardMask);
}


char *
GenericDeck_cardString(Deck *deck, int cardIndex) {
  static thread char outString[16];

  (*deck->cardToString)(cardIndex, outString);
  return outString;
}


int
GenericDeck_printCard(Deck *deck, int cardIndex) {
  char outString[16];
  int ret;

  ret = (*deck->cardToString)(cardIndex, outString);
  if (ret) 
    printf("%s", outString);
  return ret;
}


