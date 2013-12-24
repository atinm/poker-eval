// $Id: ValueSortedMapTest.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.util.test;

import junit.framework.TestCase;
import org.pokersource.util.ValueSortedMap;

import java.util.List;

/**
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class ValueSortedMapTest extends TestCase {
  ValueSortedMap vsm1;

  public ValueSortedMapTest(String name) {
    super(name);
  }

  public static void main(String args[]) {
    junit.textui.TestRunner.run(ValueSortedMapTest.class);
  }

  public void setUp() {
    vsm1 = new ValueSortedMap();
    vsm1.put("A", new Double(80));
    vsm1.put("B", new Double(70));
    vsm1.put("C", new Double(30));
    vsm1.put("D", new Double(40));
    vsm1.put("E", new Double(40));
    vsm1.put("F", new Double(40));
  }

  public void testKeyList() {
    List kl = vsm1.keyList();
    assertEquals(6, kl.size());
    assertEquals("C", kl.get(0));
    assertEquals("D", kl.get(1));
    assertEquals("E", kl.get(2));
    assertEquals("F", kl.get(3));
    assertEquals("B", kl.get(4));
    assertEquals("A", kl.get(5));
  }

  public void testLess() {
    List kl = vsm1.less(new Double(40));
    assertEquals(1, kl.size());
    assertEquals("C", kl.get(0));
    kl = vsm1.less(new Double(41));
    assertEquals(4, kl.size());
    assertEquals("C", kl.get(0));
    assertEquals("D", kl.get(1));
    assertEquals("E", kl.get(2));
    assertEquals("F", kl.get(3));
  }

  public void testLessEqual() {
    List kl = vsm1.lessEqual(new Double(39));
    assertEquals(1, kl.size());
    assertEquals("C", kl.get(0));
    kl = vsm1.lessEqual(new Double(40));
    assertEquals(4, kl.size());
    assertEquals("C", kl.get(0));
    assertEquals("D", kl.get(1));
    assertEquals("E", kl.get(2));
    assertEquals("F", kl.get(3));
  }

  public void testGreater() {
    List kl = vsm1.greater(new Double(39));
    assertEquals(5, kl.size());
    assertEquals("D", kl.get(0));
    assertEquals("E", kl.get(1));
    assertEquals("F", kl.get(2));
    assertEquals("B", kl.get(3));
    assertEquals("A", kl.get(4));
    kl = vsm1.greater(new Double(40));
    assertEquals(2, kl.size());
    assertEquals("B", kl.get(0));
    assertEquals("A", kl.get(1));
  }

  public void testGreaterEqual() {
    List kl = vsm1.greaterEqual(new Double(40));
    assertEquals(5, kl.size());
    assertEquals("D", kl.get(0));
    assertEquals("E", kl.get(1));
    assertEquals("F", kl.get(2));
    assertEquals("B", kl.get(3));
    assertEquals("A", kl.get(4));
    kl = vsm1.greaterEqual(new Double(41));
    assertEquals(2, kl.size());
    assertEquals("B", kl.get(0));
    assertEquals("A", kl.get(1));
  }

  public void testToString() {
    assertEquals("C:30.0 D:40.0 E:40.0 F:40.0 B:70.0 A:80.0",
            vsm1.toString());
  }
}
