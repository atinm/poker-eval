// $Id: LongArrayTest.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.util.test;

import junit.framework.TestCase;
import org.pokersource.util.LongArray;

/**
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class LongArrayTest extends TestCase {
  LongArray a1, a2, a3, a4;

  public LongArrayTest(String name) {
    super(name);
  }

  public static void main(String args[]) {
    junit.textui.TestRunner.run(LongArrayTest.class);
  }

  public void setUp() {
    a1 = new LongArray(new long[]{1, 1, 1});
    a2 = new LongArray(new long[]{1, 1, 1});
    a3 = new LongArray(new long[]{1, 2, 3});
    a4 = new LongArray(new long[]{1, 1});
  }

  public void testEquals() {
    assertTrue(a1.equals(a1));
    assertTrue(a1.equals(a2));
    assertTrue(!a1.equals(a3));
    assertTrue(!a1.equals(a4));
    assertTrue(a2.equals(a1));
    assertTrue(!a3.equals(a1));
    assertTrue(!a4.equals(a1));
  }

  public void testHashCode() {
    assertTrue(a1.hashCode() == a2.hashCode());
  }

  public void testCompareTo() {
    assertEquals(0, a1.compareTo(a1));
    assertEquals(0, a1.compareTo(a2));
    assertEquals(-1, a1.compareTo(a3));
    assertEquals(1, a1.compareTo(a4));

    assertEquals(0, a2.compareTo(a1));
    assertEquals(0, a2.compareTo(a2));
    assertEquals(-1, a2.compareTo(a3));
    assertEquals(1, a2.compareTo(a4));

    assertEquals(1, a3.compareTo(a1));
    assertEquals(1, a3.compareTo(a2));
    assertEquals(0, a3.compareTo(a3));
    assertEquals(1, a3.compareTo(a4));

    assertEquals(-1, a4.compareTo(a1));
    assertEquals(-1, a4.compareTo(a2));
    assertEquals(-1, a4.compareTo(a3));
    assertEquals(0, a4.compareTo(a4));
  }

  public void testToString() {
    assertEquals("1 1 1", a1.toString());
    assertEquals("1 1 1", a2.toString());
    assertEquals("1 2 3", a3.toString());
    assertEquals("1 1", a4.toString());
  }
}
