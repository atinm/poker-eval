/*
 * Copyright (C) 1999-2006 
 *           Brian Goetz <brian@quiotix.com>
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
#ifndef GAME_STD_H
#define GAME_STD_H

#define DECK_STANDARD
#define RULES_STANDARD

#include "deck_std.h"
#include "rules_std.h"

#undef Hand_EVAL_N
#undef Hand_EVAL_LOW
#undef Hand_EVAL_LOW8

#define Hand_EVAL_N     StdDeck_StdRules_EVAL_N
#define Hand_EVAL_TYPE  StdDeck_StdRules_EVAL_TYPE
#define Hand_EVAL_X5    StdDeck_StdRules_EVAL_X5
#define Hand_EVAL_X7    StdDeck_StdRules_EVAL_X7

#define Hand_EVAL_LOW   StdDeck_Lowball_EVAL
#define Hand_EVAL_LOW8  StdDeck_Lowball8_EVAL

#undef  DECK_STANDARD
#undef  RULES_STANDARD

#endif /* GAME_STD_H */
