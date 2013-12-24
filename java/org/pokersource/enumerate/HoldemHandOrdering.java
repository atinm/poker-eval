// $Id: HoldemHandOrdering.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate;

import java.io.*;
import java.util.StringTokenizer;

/**
 Defines an ordering (from 'weakest' to 'strongest') of holdem hand groups.
 Staring with a sequence of disjoint hand groups G_i (i=1..N), assigns to
 each group G_i a numeric value equal to the fraction of all atomic hands in
 groups G_1 through G_{i-1}.  A typical usage is define the G_i as the 169
 canonical hand groups.

 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class HoldemHandOrdering extends HandValuation {
  private static final int TOTAL_HANDS = 1326;

  public HoldemHandOrdering(String path) throws IOException {
    super(path);
  }

  public HoldemHandOrdering(InputStream stream) throws IOException {
    super(stream);
  }

  /** Populates self from an input stream.  The format is a text file with
   whitespace separated tokens; each token represents a hand group.
   Comments begin with the '#' character and are terminated by end of line.
   The first hand group in the file is the 'least' or 'weakest'; the last
   hand group in the file is the 'greatest' or 'strongest'.  The hand
   groups must be disjoint and their union must include all 1326 atomic
   holdem hands. */
  public void fromStream(InputStream stream) throws IOException {
    InputStreamReader isr = new InputStreamReader(stream);
    BufferedReader br = new BufferedReader(isr);
    String line;
    int nseen = 0;
    lineloop:
      while ((line = br.readLine()) != null) {
        StringTokenizer t = new StringTokenizer(line, " \t");
        while (t.hasMoreTokens()) {
          String handstr = t.nextToken();
          if (handstr.startsWith("#"))
            continue lineloop;
          HandGroup group;
          try {
            group = HoldemHandGroupFactory.getInstance(handstr);
          } catch (IllegalArgumentException e) {
            throw new RuntimeException("unable to parse group: " + handstr);
          }
          double value = (double) nseen / TOTAL_HANDS;
          setValue(group, value);
          nseen += group.numHands();
        }
      }
    if (nseen != TOTAL_HANDS)
      throw new RuntimeException("missing hands in input");
  }

  /** @see #fromStream(InputStream) */
  public void fromFile(String path) throws IOException {
    fromStream(new FileInputStream(path));
  }

  public static void main(String[] args) {
    String path = args[0];
    String threshstr = args[1];
    double thresh = Double.parseDouble(threshstr);
    HandValuation hv;
    try {
      hv = new HoldemHandOrdering(path);
    } catch (IOException e) {
      e.printStackTrace();
      return;
    }
    System.out.println("HandValuation = [" + hv + "]");
    System.out.println("hv[>=" + threshstr + "] = ");
    HandGroup[] groups = hv.greaterEqual(thresh);
    for (int i = 0; i < groups.length; i++)
      System.out.print(groups[i] + " ");
    System.out.println();
  }

}
