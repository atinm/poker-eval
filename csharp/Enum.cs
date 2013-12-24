//
// Copyright (C) 2004-2006 Mekensleep
//
// Mekensleep <licensing@mekensleep.com>
// 24 rue vieille du temple,  75004 Paris
//       
// This program gives you software freedom; you can copy, convey,
// propagate, redistribute and/or modify this program under the terms of
// the GNU General Public License (GPL) as published by the Free Software
// Foundation (FSF), either version 3 of the License, or (at your option)
// any later version of the GPL published by the FSF.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program in a file in the toplevel directory called "GPLv3".
// If not, see <http://www.gnu.org/licenses/>.
//
// Authors:
//  Johan Euphrosine <johan@mekensleep.com>
//
// 

using System;

namespace PokerEval
{
  public class Enum
  {
    static public EnumResult Sample(Game game, StdDeck.CardMask[] pockets, StdDeck.CardMask board, StdDeck.CardMask dead, int npockets, int nboard, int niter, int orderflag)
    {
      EnumResult result = new EnumResult();
      API.enumSample(game, pockets, board, dead, npockets, nboard, niter, orderflag, ref result);
      return result;
    }
  }
}
