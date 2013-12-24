// $Id: HandValuation.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate;

import org.pokersource.util.ValueSortedMap;

import java.io.IOException;
import java.io.InputStream;
import java.util.Iterator;
import java.util.List;

/**
 Maintains a mapping from HandGroup to numeric value; defines methods for
 sorting and selecting subsets of hand groups based on their numeric value.

 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class HandValuation {
  /** ValueSortedMap of {HandGroup, Double} giving value of each hand. */
  private ValueSortedMap values;

  public HandValuation() {
    values = new ValueSortedMap();
  }

  public HandValuation(String path) throws IOException {
    this();
    fromFile(path);
  }

  public HandValuation(InputStream stream) throws IOException {
    this();
    fromStream(stream);
  }

  /** Subclasses should implement this according to their own file format. */
  public void fromFile(String path) throws IOException {
    // rather than make the whole class abstract, let's do this:
    throw new UnsupportedOperationException();
  }

  /** Subclasses should implement this according to their own file format. */
  public void fromStream(InputStream stream) throws IOException {
    // rather than make the whole class abstract, let's do this:
    throw new UnsupportedOperationException();
  }

  /** Set the numeric value of the hand group. */
  public void setValue(HandGroup group, double value) {
    values.put(group, new Double(value));
  }

  /** Get the numeric value of the hand group. */
  public double getValue(HandGroup group) {
    Double d = (Double) values.get(group);
    return d.doubleValue();
  }

  /** Get all groups whose numeric value is less than the threshold.  The
   groups will be sorted in increasing order of their numeric value. */
  public HandGroup[] less(double threshold) {
    List l = values.less(new Double(threshold));
    return (HandGroup[]) l.toArray(new HandGroup[l.size()]);
  }

  /** Get all groups whose numeric value is less than or equal to the
   threshold.  The groups will be sorted in increasing order of their
   numeric value. */
  public HandGroup[] lessEqual(double threshold) {
    List l = values.less(new Double(threshold + 1e-10));
    return (HandGroup[]) l.toArray(new HandGroup[l.size()]);
  }

  /** Get all groups whose numeric value is greater than the threshold.  The
   groups will be sorted in increasing order of their numeric value. */
  public HandGroup[] greater(double threshold) {
    List l = values.greaterEqual(new Double(threshold + 1e-10));
    return (HandGroup[]) l.toArray(new HandGroup[l.size()]);
  }

  /** Get all groups whose numeric value is greater than or equal to the
   threshold.  The groups will be sorted in increasing order of their
   numeric value. */
  public HandGroup[] greaterEqual(double threshold) {
    List l = values.greaterEqual(new Double(threshold));
    return (HandGroup[]) l.toArray(new HandGroup[l.size()]);
  }

  public String toString() {
    StringBuffer buf = new StringBuffer();
    for (Iterator iter = values.keyList().iterator(); iter.hasNext();) {
      HandGroup hg = (HandGroup) iter.next();
      Double d = (Double) values.get(hg);
      if (buf.length() > 0)
        buf.append(" ");
      buf.append(hg + ":" + d);
    }
    return buf.toString();
  }

  public static void main(String[] args) {
    HandValuation hv = new HandValuation();
    HandGroup hg1 = HoldemHandGroupFactory.getInstance("AKs");
    hv.setValue(hg1, 80);
    HandGroup hg2 = HoldemHandGroupFactory.getInstance("AQ");
    hv.setValue(hg2, 70);
    HandGroup hg3 = HoldemHandGroupFactory.getInstance("T8");
    hv.setValue(hg3, 30);
    HandGroup hg4 = HoldemHandGroupFactory.getInstance("32s");
    hv.setValue(hg4, 10);
    HandGroup[] groups;

    System.out.println("HandValuation = [" + hv + "]");

    groups = hv.less(30);
    System.out.print("Groups scoring <  30: ");
    for (int i = 0; i < groups.length; i++)
      System.out.print(groups[i] + " ");
    System.out.println();

    groups = hv.lessEqual(30);
    System.out.print("Groups scoring <= 30: ");
    for (int i = 0; i < groups.length; i++)
      System.out.print(groups[i] + " ");
    System.out.println();

    groups = hv.greater(30);
    System.out.print("Groups scoring >  30: ");
    for (int i = 0; i < groups.length; i++)
      System.out.print(groups[i] + " ");
    System.out.println();

    groups = hv.greaterEqual(30);
    System.out.print("Groups scoring >= 30: ");
    for (int i = 0; i < groups.length; i++)
      System.out.print(groups[i] + " ");
    System.out.println();
  }
}
