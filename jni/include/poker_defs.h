/*
 * Copyright (C) 1999-2006 
 *           Brian Goetz <brian@quiotix.com>
 *           Loic Dachary <loic@dachary.org>
 *           Igor Kravtchenko <igor@obraz.net>
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
#ifndef __POKER_DEFS_H__
#define __POKER_DEFS_H__


/* Compiler-specific junk */

#if defined(_MSC_VER)
#  define UINT64_TYPE unsigned __int64
#  define inline __inline
#  define thread __declspec( thread )
#else
#  define thread 
#  include "poker_config.h"
#endif

#ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif
#ifdef HAVE_INTTYPES_H
#  include <inttypes.h>
#else
#  ifdef HAVE_STDINT_H
#    include <stdint.h>
#  endif
#endif

/* 64-bit integer junk */

#undef USE_INT64
#ifdef HAVE_UINT64_T
typedef uint64_t		uint64;
#define USE_INT64 1
#elif defined(HAVE_LONG_LONG)
typedef unsigned long long      uint64;
#define USE_INT64 1
#elif SIZEOF_LONG == 8
typedef unsigned long           uint64;
#define USE_INT64 1
#elif defined(UINT64_TYPE)
typedef UINT64_TYPE             uint64;
#define USE_INT64 1
#endif

#ifdef USE_INT64
#define LongLong_OP(result, op1, op2, operation) \
  do { result = (op1) operation (op2); } while (0)
#define LongLong_OR(result, op1, op2)  LongLong_OP(result, op1, op2, |)
#define LongLong_AND(result, op1, op2) LongLong_OP(result, op1, op2, &)
#define LongLong_XOR(result, op1, op2) LongLong_OP(result, op1, op2, ^)
#endif


typedef unsigned char  uint8;
#ifndef HAVE_INT8
typedef signed char   int8;
#endif
typedef unsigned short uint16;
typedef unsigned int   uint32;

#include "deck.h"
#include "handval.h"
#include "handval_low.h"
#include "enumerate.h"

#include "game_std.h"

#endif

