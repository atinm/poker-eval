/* $Id: enumord.c 5146 2008-12-04 03:11:22Z bkuhn $ */
/*
  See comments in enumord.h.
  Copyright (C) Jun 2002, Michael Maurer.
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

#include <stdlib.h>
#include "enumord.h"

int enum_nbits[ENUM_ORDERING_MAXPLAYERS+1] = {0, 1, 2, 2, 3, 3, 3, 3};

typedef struct {
  int index;
  int handval;
} enum_rankelem_t;

static int
enum_rankelem_compare(const void *p1, const void *p2) {
  enum_rankelem_t *e1 = (enum_rankelem_t *) p1;
  enum_rankelem_t *e2 = (enum_rankelem_t *) p2;
  if (e1->handval < e2->handval)
    return -1;
  else if (e1->handval == e2->handval)
    return 0;
  else
    return 1;
}

void
enum_ordering_rank(HandVal *hands, int noqual, int nplayers, int *ranks,
                   int reverse)
{
  enum_rankelem_t elems[ENUM_ORDERING_MAXPLAYERS];
  int i;
  int currank, lastval;
  if (reverse)
    noqual = -noqual;
  for (i=0; i<nplayers; i++) {
    elems[i].index = i;
    elems[i].handval = reverse ? -hands[i] : hands[i];
  }
  /* TODO: we may find that the large number of function calls that qsort()
     makes to enum_rankelem_compare(), even for small arrays, is too slow
     to tolerate.  After all, this function is called in the inner loop of
     a hand outcome enumeration.  Let's cross that bridge if we come to it. */
  qsort(elems, nplayers, sizeof(enum_rankelem_t), enum_rankelem_compare);
  currank = -1;
  lastval = -1;
  for (i=nplayers-1; i>=0; i--) {
    if (elems[i].handval != lastval) {
      currank = nplayers - i - 1;
      lastval = elems[i].handval;
    }
    if (elems[i].handval == noqual)
      currank = nplayers;
    ranks[elems[i].index] = currank;
  }
}

