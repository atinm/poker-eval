// $Id: RankOrdering.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate;

import org.pokersource.util.IntArray;

/**
 Represents the relative hand rank order of one hand matchup outcome.
 If values[i]==0, then player i has the best hand (possibly tying),
 if values[i]==1, then one player has a better hand than player i, etc.
 If values[i]==values.length then player i does not have a qualifying hand.

 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class RankOrdering extends IntArray {
  public RankOrdering(int values[]) {
    super(values);
  }
}
