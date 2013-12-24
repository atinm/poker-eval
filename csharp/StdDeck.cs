//
// Copyright (C) 2004-2006 Mekensleep
//
// Mekensleep <licensing@mekensleep.com>
// 24 rue vieille du temple, 75004 Paris
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
using System.Text;

namespace PokerEval
{
  public class StdDeck
  {
    public static string CardToString(int card)
    {
      const int maxCardStringLength = 256;
      StringBuilder sb = new StringBuilder(maxCardStringLength);
      API.StdDeck_cardToString(card, sb);
      return sb.ToString();
    }
    public static int StringToCard(string cardStr)
    {
      int card;
      API.StdDeck_stringToCard(cardStr, out card);
      return card;
    }
    public struct CardMask
    {
      public Int64 mask;
      public void Set(int card)
      {
	mask = API.wrap_StdDeck_CardMask_SET(this, card).mask;
      }
      public void Set(string card)
      {
	Set(StringToCard(card));
      }
      public void Reset()
      {
	mask = API.wrap_StdDeck_CardMask_RESET().mask;
      }
    }
  }
}