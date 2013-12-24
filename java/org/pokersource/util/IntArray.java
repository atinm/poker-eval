// $Id: IntArray.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.util;

/**
 A wrapper around an int[] that is suitable for use as a key in a Map.  The
 equals() method is defined such that A.equals(B) iff A.values[i] ==
 B.values[i] for all elements i.  Similarly, the hashCode() method is
 defined so that A.equals(B) implies A.hashCode() == B.hashCode().  The
 compareTo() method is defined lexicographically, with shorter arrays
 comparing less than longer arrays having the same starting elements.

 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class IntArray implements Comparable {
  public int values[];
  private int hash;

  public IntArray(int values[]) {
    this.values = (int[]) values.clone();
    computeHash();
  }

  private void computeHash() {
    // hash = values.hashCode()  --- WRONG: not equal for equal arrays
    hash = values.length;
    for (int i = 0; i < values.length && i < 32; i++)
      hash = 31 * hash + values[i];
  }

  public int hashCode() {
    return hash;
  }

  public boolean equals(Object o) {
    IntArray other = (IntArray) o;
    return java.util.Arrays.equals(this.values, other.values);
  }

  public int compareTo(Object o) {
    IntArray other = (IntArray) o;
    for (int i = 0; i < this.values.length; i++) {
      if (i >= other.values.length)
        return 1;
      else if (this.values[i] < other.values[i])
        return -1;
      else if (this.values[i] > other.values[i])
        return 1;
    }
    // if we get here, the first this.values.length elements are equal
    if (this.values.length < other.values.length)
      return -1;
    else
      return 0;
  }

  public String toString() {
    StringBuffer buf = new StringBuffer();
    for (int i = 0; i < values.length; i++) {
      if (i > 0)
        buf.append(" ");
      buf.append(values[i]);
    }
    return buf.toString();
  }
}
