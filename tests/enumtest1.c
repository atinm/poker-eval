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
/* $Id: enumtest1.c 5146 2008-12-04 03:11:22Z bkuhn $ */

#include <stdio.h>
#include <stdlib.h>

#include "combinations.h"

int
main(int argc, char **argv)
{
  int nuniv;
  int nelem;
  int ncombo, *elems, cnum, i;
  Combinations vp;

  if (argc != 3) {
    printf("usage: %s nuniv nelem\n", argv[0]);
    return 1;
  }
  nuniv = atoi(argv[1]);
  nelem = atoi(argv[2]);
  vp = init_combinations(nuniv, nelem);
  if (vp == NULL) {
    printf("init_combinations failed\n");
    return 1;
  }
  ncombo = num_combinations(vp);
  elems = (int *) malloc(nelem * sizeof(int));
  for (cnum=0; cnum<ncombo; cnum++) {
    get_combination(vp, cnum, elems);
    for (i=0; i<nelem; i++)
      printf("%d ", elems[i] + 1);
    printf("\n");
  }
  free_combinations(vp);
  return 0;
}
