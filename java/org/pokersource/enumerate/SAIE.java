// $Id: SAIE.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate;

import org.pokersource.game.Deck;
import org.pokersource.util.NestedLoopEnumeration;
import org.pokersource.util.NestedLoopSampling;

import java.util.Enumeration;
import java.util.Iterator;
import java.util.Map;
import java.util.TreeMap;

/** Algorithms for computing subjective all-in equity.  SAIE is a player's pot
 equity given particular beliefs about the possible hands of the
 opponent(s) and assuming no further betting.  Beliefs about an opponent's
 hand distribution are represented by a BeliefVector object which maps each
 possible opponent hand to the probability of its occurrence.
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class SAIE {
  private SAIE() {    // don't let anybody instantiate us
  }

  private static void accumulateOrderings(Map orderings,
                                          int[][] orderKeys,
                                          int[] orderVals) {
    for (int i = 0; i < orderKeys.length; i++) {
      int newTotal = orderVals[i];
      RankOrdering ord = new RankOrdering(orderKeys[i]);
      Integer prevTotal = (Integer) orderings.get(ord);
      if (prevTotal != null)
        newTotal += prevTotal.intValue();
      orderings.put(ord, new Integer(newTotal));
    }
  }

  /** Compute the subjective all-in equity of each player based on a
   belief distribution for each player's hands.  Typical usage is
   to fix one player's cards and allow the other players' cards to
   range over a distribution; however, it is valid for all players to
   have multiple possible hands.
   @param gameType One of Enumerate.GAME_HOLDEM, etc.
   @param nmatchups number of matchups to sample (if zero, full enumeration)
   [Note: matchups are counted before they are tested for feasibility, that
   is, whether they share cards.  So the total number of matchups that
   contribute to the SAIE estimate may be less than nmatchups.]
   @param noutcomes number of boards to sample for each matchup (if zero,
   full enumeration)
   @param handDistribs the hand distribution belief vector for each player
   @param board bitmask of cards already dealt to board (can be zero)
   @param dead bitmask of cards that cannot appear in any hand or on
   the board (can be zero)
   @param ev output: ev[i] player i's all-in pot equity
   @param matchups output: map of {HandMatchup, MatchupOutcome}
   pairs, one for each matchup.  Skipped if matchups is null.
   @param orderings output: map of {RankOrdering, Integer} pairs.
   Skipped if orderings is null.
   */
  public static void FlopGameSAIE(int gameType, int nmatchups, int noutcomes,
                                  BeliefVector[] handDistribs, long board,
                                  long dead, double ev[], Map matchups,
                                  Map orderings) {
    if (matchups != null)
      matchups.clear();
    if (orderings != null)
      orderings.clear();
    int nplayers = handDistribs.length;
    long[][] hands = new long[nplayers][];
    int[] nhands = new int[nplayers];
    for (int i = 0; i < nplayers; i++) {
      hands[i] = handDistribs[i].getHands();
      nhands[i] = hands[i].length;
      ev[i] = 0;
    }
    long unavail1 = dead | board;
    double totalprob = 0;
    double[] matchev = new double[nplayers];
    long[] curhands = new long[nplayers];
    Enumeration enumerate;
    if (nmatchups == 0) {
      enumerate = new NestedLoopEnumeration(nhands);
    } else {
      enumerate = new NestedLoopSampling(nhands);
    }
    int nvisited = 0;
    mainloop:
      // loop over all hand matchups, or over a sample of matchups
      // TODO: if no more cards to come, this is better done by evaluating
      // each player's hands just once and then comparing values in matchups;
      // probably makes sense as a separate method.
      while (enumerate.hasMoreElements() &&
              (nmatchups == 0 || nvisited < nmatchups)) {
        int[] indices = (int[]) enumerate.nextElement();
        long unavail2 = unavail1;
        double matchprob = 1;     // the probability of this matchup
        for (int i = 0; i < nplayers; i++) {
          // get a particular hand for each player to use in this matchup
          curhands[i] = hands[i][indices[i]];
          if ((curhands[i] & unavail2) != 0) // already used one of these cards?
            continue mainloop;    	   // if so, this matchup cannot occur
          unavail2 |= curhands[i];
          matchprob *= handDistribs[i].getBeliefProb(curhands[i]);
          if (matchprob == 0)
            continue mainloop;
        }

        // heavy lifting for this matchup: enumerate all outcomes, keeping track
        // of relative hand rank orderings if requested
        if (orderings == null) {
          // simply compute EV, no need to track rank orderings
          Enumerate.PotEquity(gameType, noutcomes, curhands, board, dead,
                  matchev);
        } else {
          // compute EV and also maintain histogram of rank orderings
          int[][][] orderKeys = new int[1][][];
          int[][] orderVals = new int[1][];
          Enumerate.PotEquity(gameType, noutcomes, curhands, board, dead,
                  matchev, orderKeys, orderVals);
          accumulateOrderings(orderings, orderKeys[0], orderVals[0]);
        }

        // save this matchup to a Collection, if requested
        if (matchups != null) {
          HandMatchup matchup = new HandMatchup(curhands);
          MatchupOutcome outcome = new MatchupOutcome(matchprob, matchev);
          MatchupOutcome existing = (MatchupOutcome) matchups.get(matchup);
          if (existing != null)
            existing.merge(outcome);
          else
            matchups.put(matchup, outcome);
        }

        // accumulate this matchup into totals
        for (int i = 0; i < nplayers; i++)
          ev[i] += matchev[i] * matchprob;
        totalprob += matchprob;
        nvisited++;
      }
    if (nvisited == 0 || totalprob == 0)
      throw new IllegalArgumentException("no matchups sampled");

    // Scale by the total probability of all matchups (this factor is less
    // than one when the hand distributions are not disjoint).
    for (int i = 0; i < nplayers; i++)
      ev[i] /= totalprob;
    if (matchups != null) {
      for (Iterator iter = matchups.values().iterator(); iter.hasNext();) {
        MatchupOutcome outcome = (MatchupOutcome) iter.next();
        outcome.matchProb /= totalprob;
      }
    }
  }

  public static void main(String[] args) {
    int nmatchups = Integer.parseInt(args[0]);
    int noutcomes = Integer.parseInt(args[1]);
    int nplayers = args.length - 4;
    System.out.println("nplayers=" + nplayers +
            ", nmatchups=" + nmatchups +
            ((nmatchups == 0) ? " (enumerate)" : " (sample)") +
            ", noutcomes=" + noutcomes +
            ((noutcomes == 0) ? " (enumerate)" : " (sample)"));
    HoldemBeliefVector[] beliefs = new HoldemBeliefVector[nplayers];
    for (int i = 0; i < nplayers; i++) {
      beliefs[i] = new HoldemBeliefVector(args[i + 2]);
      System.out.println("beliefs[" + i + "].toString = " +
              beliefs[i].toString());
      System.out.println("beliefs[" + i + "].toStringAtomic = " +
              beliefs[i].toStringAtomic());
    }
    long board = Deck.parseCardMask(args[args.length - 2]);
    long dead = Deck.parseCardMask(args[args.length - 1]);
    System.out.println("board = " + Deck.cardMaskString(board));
    System.out.println("dead = " + Deck.cardMaskString(dead));

    double[] totalev = new double[nplayers];
    TreeMap orderings = new TreeMap();
    FlopGameSAIE(Enumerate.GAME_HOLDEM8, nmatchups, noutcomes,
            beliefs, board, dead, totalev, null, orderings);
    for (int i = 0; i < nplayers; i++) {
      System.out.println("FlopGameSAIE: totalev[" + i + "] = " + totalev[i]);
    }
    System.out.println("FlopGameSAIE: relative rank ordering histogram:");
    for (Iterator iter = orderings.keySet().iterator(); iter.hasNext();) {
      RankOrdering ranks = (RankOrdering) iter.next();
      Integer count = (Integer) orderings.get(ranks);
      System.out.println("\t" + ranks + " " + count);
    }
  }
}
