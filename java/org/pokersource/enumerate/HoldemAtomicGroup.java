// $Id: HoldemAtomicGroup.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate;

import org.apache.oro.text.regex.*;
import org.pokersource.game.Deck;

import java.util.HashSet;

/** A holdem hand group representing one exact starting hand such as
 "AhKh".
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class HoldemAtomicGroup extends BaseHandGroup
        implements HoldemHandGroup {
  private static Perl5Compiler compiler;
  private static Perl5Matcher matcher;
  private static Pattern atomicPattern;

  static {
    compiler = new Perl5Compiler();
    matcher = new Perl5Matcher();
    try {
      atomicPattern = compiler.compile
              ("^([AKQJT98765432])([shdc])([AKQJT98765432])([shdc])$");
    } catch (MalformedPatternException e) {
      throw new RuntimeException("BUG: " + e.toString());
    }
  }

  /** Convert specific starting hand to HoldemAtomicGroup object.
   @param groupSpec starting hand (e.g., AhKd, 8h3s)
   */
  public HoldemAtomicGroup(String groupSpec) {
    myspec = groupSpec;
    myhands = new HashSet();
    MatchResult result;
    if (matcher.matches(groupSpec, atomicPattern)) {
      result = matcher.getMatch();
      int rank1 = Deck.parseRank(result.group(1));
      int suit1 = Deck.parseSuit(result.group(2));
      int rank2 = Deck.parseRank(result.group(3));
      int suit2 = Deck.parseSuit(result.group(4));
      addAtomic(rank1, suit1, rank2, suit2);
    } else {
      throw new IllegalArgumentException("unable to parse groupSpec: " +
              groupSpec);
    }
  }

  private void addAtomic(int rank1, int suit1, int rank2, int suit2) {
    if (rank1 == rank2 && suit1 == suit2)
      throw new IllegalArgumentException("atomic cards are identical");
    long card1 = Deck.createCardMask(rank1, suit1);
    long card2 = Deck.createCardMask(rank2, suit2);
    long hand = card1 | card2;
    myhands.add(new Long(hand));
  }

  public static void main(String[] args) {
    String groupSpec = args[0];
    HoldemAtomicGroup g = new HoldemAtomicGroup(groupSpec);
    System.out.println("spec=" + groupSpec + ", parsed=" + g.toString() +
            ", atomic=" + g.toStringAtomic());
  }
}
