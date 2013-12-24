/* $Id: combinations.c 5439 2009-01-02 10:53:19Z loic $
   Combinations (N choose R).

   Provides an enumeration of all (N choose R) combinations of R elements from
   a set of N.  The get_combination() function returns a specific combination
   as an integer array with R elements in [0..N-1].

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

#include <stdio.h>
#include <stdlib.h>
#include "combinations.h"

typedef struct {
  int	nelem;		/* for i in [0..nelem-1] */
  int	ncombo;		/* for j in [0..ncombo-1] */
  int	**combos;	/* combos[i][j] is ith element of jth combo */
} combinations_t;

void
free_combinations(Combinations vp)
{
  combinations_t *combo = (combinations_t *) vp;
  int i;
  if (combo != NULL) {
    if (combo->combos != NULL) {
      for (i=0; i<combo->nelem; i++)
        if (combo->combos[i] != NULL)
          free(combo->combos[i]);
      free(combo->combos);
    }
    free(combo);
  }
}

Combinations
init_combinations(int nuniv, int nelem)
{
  combinations_t *combo;
  int ncombo;
  int i, j;

  if (nelem > nuniv)
    return NULL;
  ncombo = 1;
  for (i=0; i<nelem; i++) {	/* compute (nuniv CHOOSE nelem) */
    ncombo *= nuniv - i;
    ncombo /= i + 1;
  } 
  combo = (combinations_t *) malloc(sizeof(combinations_t));
  if (combo == NULL)
    return NULL;
  combo->nelem = nelem;
  combo->ncombo = ncombo;
  combo->combos = (int **) malloc(combo->nelem * sizeof(int *));
  if (combo->combos == NULL) {
    free_combinations(combo);
    return NULL;
  }
  for (i=0; i<combo->nelem; i++) {
    combo->combos[i] = (int *) malloc(combo->ncombo * sizeof(int));
    if (combo->combos[i] == NULL) {
      free_combinations(combo);
      return NULL;
    }
  }
  /* initialize first element */
  for (i=0; i<combo->nelem; i++)
    combo->combos[i][0] = i;
  for (j=1; j<combo->ncombo; j++) {
    int firstIncr = -1;
    for (i=combo->nelem-1; i>=0; i--) {
      if (combo->combos[i][j-1] + 1 <= nuniv - (nelem - i)) {
        combo->combos[i][j] = combo->combos[i][j-1] + 1;
        firstIncr = i;
        break;
      }
    }
    if (firstIncr == -1) 
      return NULL;
    for (i=0; i<firstIncr; i++)
      combo->combos[i][j] = combo->combos[i][j-1];
    for (i=firstIncr+1; i<combo->nelem; i++)
      combo->combos[i][j] = combo->combos[i-1][j] + 1;
  }
  return (Combinations) combo;
}

int
num_combinations(Combinations vp)
{
  combinations_t *combo = (combinations_t *) vp;
  return combo->ncombo;
}

void
get_combination(Combinations vp, int cnum, int *elems)
{
  combinations_t *combo = (combinations_t *) vp;
  int i;
  for (i=0; i<combo->nelem; i++)
    elems[i] = combo->combos[i][cnum];
}
  
