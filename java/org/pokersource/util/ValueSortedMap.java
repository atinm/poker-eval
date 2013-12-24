// $Id: ValueSortedMap.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.util;

import java.util.*;

/**
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class ValueSortedMap {
  private static class ValueEntry implements Comparable {
    public Object key;
    public Comparable value;

    public ValueEntry(Object key, Comparable value) {
      this.key = key;
      this.value = value;
    }

    public int compareTo(Object o) {
      ValueEntry other = (ValueEntry) o;
      int cmp = this.value.compareTo(other.value);
      if (cmp == 0) {	// if values equal, look to keys
        // if keys are null, we say they are equal; that allows us to
        // search by value
        if (this.key != null && other.key != null) {
          // if keys are not null, we try to compare them
          if (this.key.getClass() == other.key.getClass() &&
                  this.key instanceof Comparable &&
                  other.key instanceof Comparable) {
            cmp = ((Comparable) this.key).compareTo(other.key);
          } else {
            int h1 = this.key.hashCode();
            int h2 = other.key.hashCode();
            cmp = (h1 < h2) ? -1 : (h1 > h2) ? 1 : 0;
          }
        }
      }
      return cmp;
    }
  }

  /** Map of {Object, ValueEntry)} giving value of each key. */
  private HashMap values;

  public ValueSortedMap() {
    values = new HashMap();
  }

  public void put(Object key, Comparable value) {
    values.put(key, new ValueEntry(key, value));
  }

  public Comparable get(Object key) {
    ValueEntry ve = (ValueEntry) values.get(key);
    return ve.value;
  }

  private List toKeyList(List velist) {
    ArrayList keylist = new ArrayList(velist.size());
    for (int i = 0; i < velist.size(); i++) {
      ValueEntry dve = (ValueEntry) velist.get(i);
      keylist.add(i, dve.key);
    }
    return keylist;
  }

  public List keyList() {
    ArrayList al = new ArrayList(values.values());
    Collections.sort(al);
    return toKeyList(al);
  }

  public List less(Comparable threshold) {
    ArrayList al = new ArrayList(values.values());
    Collections.sort(al);
    ValueEntry thresh = new ValueEntry(null, threshold);
    int upper = 0;
    for (int i = 0; i < al.size(); i++) {
      ValueEntry ve = (ValueEntry) al.get(i);
      if (ve.compareTo(thresh) >= 0)
        break;
      upper++;
    }
    return toKeyList(al.subList(0, upper));
  }

  public List lessEqual(Comparable threshold) {
    ArrayList al = new ArrayList(values.values());
    Collections.sort(al);
    ValueEntry thresh = new ValueEntry(null, threshold);
    int upper = 0;
    for (int i = 0; i < al.size(); i++) {
      ValueEntry ve = (ValueEntry) al.get(i);
      if (ve.compareTo(thresh) > 0)
        break;
      upper++;
    }
    return toKeyList(al.subList(0, upper));
  }

  public List greater(Comparable threshold) {
    ArrayList al = new ArrayList(values.values());
    Collections.sort(al);
    ValueEntry thresh = new ValueEntry(null, threshold);
    int lower = al.size();
    for (int i = al.size() - 1; i >= 0; i--) {
      ValueEntry ve = (ValueEntry) al.get(i);
      if (ve.compareTo(thresh) <= 0)
        break;
      lower--;
    }
    return toKeyList(al.subList(lower, al.size()));
  }

  public List greaterEqual(Comparable threshold) {
    ArrayList al = new ArrayList(values.values());
    Collections.sort(al);
    ValueEntry thresh = new ValueEntry(null, threshold);
    int lower = al.size();
    for (int i = al.size() - 1; i >= 0; i--) {
      ValueEntry ve = (ValueEntry) al.get(i);
      if (ve.compareTo(thresh) < 0)
        break;
      lower--;
    }
    return toKeyList(al.subList(lower, al.size()));
  }

  public String toString() {
    StringBuffer buf = new StringBuffer();
    TreeSet ts = new TreeSet(values.values());
    for (Iterator iter = ts.iterator(); iter.hasNext();) {
      ValueEntry dve = (ValueEntry) iter.next();
      if (buf.length() > 0)
        buf.append(" ");
      buf.append(dve.key + ":" + dve.value);
    }
    return buf.toString();
  }

  public static void main(String[] args) {
    ValueSortedMap hv = new ValueSortedMap();
    hv.put("A", new Double(80));
    hv.put("B", new Double(70));
    hv.put("C", new Double(30));
    hv.put("D", new Double(10));
    System.out.println("ValueSortedMap = [" + hv + "]");

    List lkeys;
    Object[] keys;
    lkeys = hv.less(new Double(30));
    keys = lkeys.toArray();
    System.out.print("Groups scoring <  30: ");
    for (int i = 0; i < keys.length; i++)
      System.out.print(keys[i] + " ");
    System.out.println();

    lkeys = hv.greaterEqual(new Double(30));
    keys = lkeys.toArray();
    System.out.print("Groups scoring >= 30: ");
    for (int i = 0; i < keys.length; i++)
      System.out.print(keys[i] + " ");
    System.out.println();
  }
}
