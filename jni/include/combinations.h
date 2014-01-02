/*
 * Copyright (C) 2002-2006 
 *           Michael Maurer <mjmaurer@yahoo.com>
 *           Loic Dachary <loic@dachary.org>
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

/* Public declarations for combinations.c */
/* Michael Maurer, Jun 2002 */

#ifndef COMBINATIONS_H
#define COMBINATIONS_H

#include "pokereval_export.h"

typedef void *Combinations;

extern POKEREVAL_EXPORT void free_combinations(Combinations c);
extern POKEREVAL_EXPORT Combinations init_combinations(int nuniv, int nelem);
extern POKEREVAL_EXPORT int num_combinations(Combinations c);
extern POKEREVAL_EXPORT void get_combination(Combinations c, int cnum, int *elems);

#endif
