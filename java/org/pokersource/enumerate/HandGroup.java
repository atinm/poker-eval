// $Id: HandGroup.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate;

import java.util.Set;

/** A HandGroup represents a set of poker hands that can be referred to by
 name.  The specific way that hands are assigned to named groups is
 specific to each poker game.  For example, in Holdem, named groups
 include specific holdings like "AhAd", "Kh2h"; canonical starting
 hands like "AKs", "TT"; or abdulian groups like "Q8s+".  Subclasses
 like HoldemHandGroup define these groups for specific games.
 @see BaseHandGroup
 @see HoldemHandGroup
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public interface HandGroup {
  String getGroupSpec();

  int numHands();

  long[] getHands();

  Set getHandSet();

  boolean isHandInGroup(long hand);

  String toString();

  String toStringAtomic();
}
