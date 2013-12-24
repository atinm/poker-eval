// $Id: MatchupOutcome.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate;

/**
 Encodes the probability and pot equity for each player of a single
 matchup, where a matchup is a confrontation between players who each
 hold a single known hand.
 @see HandMatchup
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class MatchupOutcome {
  public double matchProb;           // probability of this matchup
  public double matchEV[];           // matchEV[i] is player i's all-in equity

  public MatchupOutcome(double matchProb,
                        double matchEV[]) {
    this.matchProb = matchProb;
    this.matchEV = (double[]) matchEV.clone();
  }

  /** Merge the results of two outcomes for the same matchup.  The merged
   EV is the weighted average of the two outcome EVs, with the weight
   being the probability assigned to each matchup.  This method can be
   used to keep a running average of outcomes for a specific matchup;
   this is useful only when the outcome EV is estimated by a monte
   carlo sample of outcomes. */
  public void merge(MatchupOutcome other) {
    double oldWeight = this.matchProb / (this.matchProb + other.matchProb);
    double newWeight = other.matchProb / (this.matchProb + other.matchProb);
    for (int i = 0; i < this.matchEV.length; i++) {
      this.matchEV[i] = (oldWeight * this.matchEV[i] +
              newWeight * other.matchEV[i]);
    }
    this.matchProb += other.matchProb;
  }
}
