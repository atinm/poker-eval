// $Id: BaseHandGroup.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate;

import org.pokersource.game.Deck;

import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

/** Implements the HandGroup interface in a way that will work for most
 poker games.  Subclasses can override if necessary.
 @see HandGroup
 @see HoldemHandGroup
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class BaseHandGroup implements HandGroup, Comparable {
  /** String representation of hand group.  Subclasses should accept this
   string in the constructor and save it here. */
  String myspec;

  /** Set of Long objects, each a bitmask for one hand.  Subclasses should,
   in their constructor, convert myspec into the set of corresponding
   hands.  The set should be immutable once set in the constructor. */
  HashSet myhands;

  // subclasses should have constructor of form: <init>(String groupSpec);

  public String getGroupSpec() {
    return myspec;
  }

  public int numHands() {
    return myhands.size();
  }

  /** Returns an array of atomic hands (each encoded as a long).
   @see org.pokersource.game.Deck
   */
  public long[] getHands() {
    long[] hands = new long[myhands.size()];
    int nhands = 0;
    for (Iterator i = myhands.iterator(); i.hasNext();)
      hands[nhands++] = ((Long) i.next()).longValue();
    return hands;
  }

  public Set getHandSet() {
    return myhands;
  }

  public boolean isHandInGroup(long hand) {
    return myhands.contains(new Long(hand));
  }

  public String toString() {
    return myspec;
  }

  public String toStringAtomic() {
    StringBuffer buf = new StringBuffer();
    for (Iterator i = myhands.iterator(); i.hasNext();) {
      long hand = ((Long) i.next()).longValue();
      if (buf.length() > 1)
        buf.append(" ");
      buf.append(Deck.cardMaskString(hand, ""));
    }
    return buf.toString();
  }

  // Comparable interface
  /** Define default sort order based on string representation. */
  public int compareTo(Object o) {
    BaseHandGroup other = (BaseHandGroup) o;
    return this.myspec.compareTo(other.myspec);
  }
}
