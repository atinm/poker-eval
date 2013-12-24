// $Id: HoldemCanonGroup.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate;

import org.apache.oro.text.regex.*;
import org.pokersource.game.Deck;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;

/** A holdem hand group representing sets of starting hands defined using
 canonical notation: "KK", "AQs", "T9".
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class HoldemCanonGroup extends BaseHandGroup
        implements HoldemHandGroup {
  private static Perl5Compiler compiler;
  private static Perl5Matcher matcher;
  private static Pattern pairPattern, suitedPattern, offsuitPattern;

  static {
    compiler = new Perl5Compiler();
    matcher = new Perl5Matcher();
    try {
      pairPattern = compiler.compile("^([AKQJT98765432])(\\1)$");
      suitedPattern = compiler.compile("^([AKQJT98765432])([AKQJT98765432])s$");
      offsuitPattern = compiler.compile("^([AKQJT98765432])([AKQJT98765432])$");
    } catch (MalformedPatternException e) {
      throw new RuntimeException("BUG: " + e.toString());
    }
  }

  /** Convert canonical holdem starting hand notation to a HoldemCanonGroup
   object.
   @param groupSpec starting hand (e.g., AA, AKs, T9)
   */
  public HoldemCanonGroup(String groupSpec) {
    myspec = groupSpec;
    myhands = new HashSet();
    if (matcher.matches(groupSpec, pairPattern)) {
      MatchResult result = matcher.getMatch();
      int rank = Deck.parseRank(result.group(1));
      addPair(rank);
    } else if (matcher.matches(groupSpec, suitedPattern)) {
      MatchResult result = matcher.getMatch();
      int rank1 = Deck.parseRank(result.group(1));
      int rank2 = Deck.parseRank(result.group(2));
      addSuited(rank1, rank2);
    } else if (matcher.matches(groupSpec, offsuitPattern)) {
      MatchResult result = matcher.getMatch();
      int rank1 = Deck.parseRank(result.group(1));
      int rank2 = Deck.parseRank(result.group(2));
      addOffsuit(rank1, rank2);
    } else {
      throw new IllegalArgumentException("unable to parse groupSpec: " +
              groupSpec);
    }
  }

  private void addPair(int rank) {
    for (int suit1 = 0; suit1 < Deck.SUIT_COUNT; suit1++) {
      long card1 = Deck.createCardMask(rank, suit1);
      for (int suit2 = suit1 + 1; suit2 < Deck.SUIT_COUNT; suit2++) {
        long card2 = Deck.createCardMask(rank, suit2);
        long hand = card1 | card2;
        myhands.add(new Long(hand));
      }
    }
  }

  private void addSuited(int rank1, int rank2) {
    for (int suit1 = 0; suit1 < Deck.SUIT_COUNT; suit1++) {
      long card1 = Deck.createCardMask(rank1, suit1);
      long card2 = Deck.createCardMask(rank2, suit1);
      long hand = card1 | card2;
      myhands.add(new Long(hand));
    }
  }

  private void addOffsuit(int rank1, int rank2) {
    for (int suit1 = 0; suit1 < Deck.SUIT_COUNT; suit1++) {
      long card1 = Deck.createCardMask(rank1, suit1);
      for (int suit2 = 0; suit2 < Deck.SUIT_COUNT; suit2++) {
        if (suit1 == suit2) continue;
        long card2 = Deck.createCardMask(rank2, suit2);
        long hand = card1 | card2;
        myhands.add(new Long(hand));
      }
    }
  }

  public static Iterator allGroups() {
    String[] ranks = {"A", "K", "Q", "J", "T", "9", "8",
                      "7", "6", "5", "4", "3", "2"};
    ArrayList groups = new ArrayList(169);
    for (int rank1 = 0; rank1 < ranks.length; rank1++) {
      for (int rank2 = rank1; rank2 < ranks.length; rank2++) {
        String canonSpec = ranks[rank1] + ranks[rank2];
        HoldemCanonGroup canon = (HoldemCanonGroup)
                HoldemHandGroupFactory.getInstance(canonSpec,
                        HoldemCanonGroup.class);
        groups.add(canon);
        if (rank1 != rank2) {
          canonSpec = canonSpec + "s";
          canon = (HoldemCanonGroup)
                  HoldemHandGroupFactory.getInstance(canonSpec,
                          HoldemCanonGroup.class);
          groups.add(canon);
        }
      }
    }
    return groups.iterator();
  }

  public static void main(String[] args) {
    String groupSpec = args[0];
    HoldemCanonGroup g = new HoldemCanonGroup(groupSpec);
    System.out.println("spec=" + groupSpec + ", parsed=" + g.toString() +
            ", atomic=" + g.toStringAtomic());
  }
}
