// $Id: ThresholdHandGroup.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate;

import org.apache.oro.text.regex.*;

import java.util.HashMap;
import java.util.HashSet;

/**
 Given a mapping from hand groups G_i to numeric values, this class allows
 you to build a new hand group as the union of those G_i whose numeric
 values are less than or greater than a certain threshold.  For example, you
 could assign a numeric 'strength' to each canonical hand G_i and then
 create a new group consisting of those G_i whose strength exceeds some
 minimum value.

 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
*/

public class ThresholdHandGroup extends BaseHandGroup {
  private static Perl5Compiler compiler;
  private static Perl5Matcher matcher;
  private static Pattern pattern;

  static {
    compiler = new Perl5Compiler();
    matcher = new Perl5Matcher();
    try {
      pattern = compiler.compile("^(\\w+)(<=?|>=?)(\\d+|\\.\\d+|\\d+\\.\\d+)$");
    } catch (MalformedPatternException e) {
      throw new RuntimeException("BUG: " + e.toString());
    }
  }

  /** Map of {String, HandValuation} pairs that have been registered via
   registerHandValuation(). */
  private static HashMap valuations = new HashMap();

  /** Register a new HandValuation that will be referred to as valuationName.
   For example, if you register a valuation as 'MYVAL', one can later
   instantiate a hand group using a syntax like 'MYVAL>0.80'; the hand
   group will include all hands to which the registered valuation assigns
   a numeric value greater than 0.80.  */
  public static void registerHandValuation(String valuationName,
                                           HandValuation valuation) {
    valuations.put(valuationName, valuation);
  }

  /** Create a set of hands corresponding to those whose value compares either
   less than or greater than a threshold value, where the value of each
   hand is defined by a HandValuation.
   @param groupSpec Defines a hand valuation, a threshold value, and a
   comparator.  The hand valuation is referred to by a string previously
   registered with registerHandValuation().  The threshold value is a
   number in decimal form.  The comparator is one of '<', '<=', '>', '>='.
   */
  public ThresholdHandGroup(String groupSpec) {
    myspec = groupSpec;
    myhands = new HashSet();
    if (matcher.matches(groupSpec, pattern)) {
      MatchResult result = matcher.getMatch();
      String valuationName = result.group(1);
      String comparator = result.group(2);
      String threshStr = result.group(3);
      double thresh;
      try {
        thresh = Double.parseDouble(threshStr);
      } catch (NumberFormatException e) {
        throw new IllegalArgumentException("threshold unparsed: " + groupSpec);
      }

      // get the registered hand valuation corresponding to valuationName
      HandValuation hv = (HandValuation) valuations.get(valuationName);
      if (hv == null)
        throw new IllegalArgumentException("unknown valuation: " + groupSpec);

      // get the set of groups meeting the threshold test
      HandGroup[] groups;
      if (comparator.equals("<"))
        groups = hv.less(thresh);
      else if (comparator.equals("<="))
        groups = hv.lessEqual(thresh);
      else if (comparator.equals(">"))
        groups = hv.greater(thresh);
      else if (comparator.equals(">="))
        groups = hv.greaterEqual(thresh);
      else
        throw new IllegalArgumentException("unknown comparator: " + groupSpec);
      if (groups.length == 0)
        throw new IllegalArgumentException("no groups passed: " + groupSpec);

      // now form the union of atomic hands of all groups meeting threshold
      for (int i = 0; i < groups.length; i++)
        myhands.addAll(groups[i].getHandSet());

    } else {
      throw new IllegalArgumentException("unable to parse: " + groupSpec);
    }
  }

  public static void main(String[] args) {
    String path = args[0];
    String valuationName = args[1];
    String groupSpec = args[2];
    try {
      HandValuation hv = new HoldemHandOrdering(path);
      registerHandValuation(valuationName, hv);
      ThresholdHandGroup g = new ThresholdHandGroup(groupSpec);
      System.out.println("spec=" + groupSpec + ", parsed=" + g.toString() +
              ", atomic=" + g.toStringAtomic());
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}
