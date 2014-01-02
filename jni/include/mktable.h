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
extern void
MakeTable_begin(const char *tableName,
                const char *fileName, 
                const char *tableType,
                int tableSize);

extern void
MakeTable_comment(const char *commentString);

extern void
MakeTable_extraCode(const char *codeString);

extern void 
MakeTable_outputString(const char *string);

#ifdef USE_INT64
  extern void 
  MakeTable_outputUInt64(uint64 arg);
#endif

extern void 
MakeTable_outputUInt32(uint32 arg);

extern void 
MakeTable_outputUInt16(uint16 arg);

extern void 
MakeTable_outputUInt8(uint8 arg);

extern void 
MakeTable_end(void);
    

extern uint32 n_bits_func(uint32 arg);
extern uint32 top_card_func(uint32 arg);
extern uint32 bottom_card_func(uint32 arg);
extern uint32 bottom_bit_func(uint32 arg);
extern uint32 top_bit_func(uint32 arg);
extern uint32 straight_func(uint32 arg);
