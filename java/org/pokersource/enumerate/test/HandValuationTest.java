// $Id: HandValuationTest.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate.test;

import junit.framework.TestCase;
import org.pokersource.enumerate.HandGroup;
import org.pokersource.enumerate.HandValuation;
import org.pokersource.enumerate.HoldemHandGroupFactory;

/**
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class HandValuationTest extends TestCase {
  private static final String SAMPLE_PATH = "sample2.hev";
  private HandValuation hv;

  public HandValuationTest(String name) {
    super(name);
  }

  public static void main(String args[]) {
    junit.textui.TestRunner.run(HandValuationTest.class);
  }

  protected void setUp() {
    hv = new HandValuation();
    HandGroup hg1 = HoldemHandGroupFactory.getInstance("AKs");
    hv.setValue(hg1, 80);
    HandGroup hg2 = HoldemHandGroupFactory.getInstance("AQ");
    hv.setValue(hg2, 70);
    HandGroup hg3 = HoldemHandGroupFactory.getInstance("T8");
    hv.setValue(hg3, 70);
    HandGroup hg4 = HoldemHandGroupFactory.getInstance("32s");
    hv.setValue(hg4, 10);
  }

  public void testToString() {
    assertEquals("32s:10.0 AQ:70.0 T8:70.0 AKs:80.0", hv.toString());
  }

  public void testLess() {
    HandGroup[] groups;
    groups = hv.less(70);
    assertEquals(1, groups.length);
    assertEquals("32s", groups[0].toString());
    groups = hv.less(71);
    assertEquals(3, groups.length);
    assertEquals("32s", groups[0].toString());
    assertEquals("AQ", groups[1].toString());
    assertEquals("T8", groups[2].toString());
  }

  public void testLessEqual() {
    HandGroup[] groups;
    groups = hv.lessEqual(69);
    assertEquals(1, groups.length);
    assertEquals("32s", groups[0].toString());
    groups = hv.lessEqual(70);
    assertEquals(3, groups.length);
    assertEquals("32s", groups[0].toString());
    assertEquals("AQ", groups[1].toString());
    assertEquals("T8", groups[2].toString());
  }

  public void testGreater() {
    HandGroup[] groups;
    groups = hv.greater(69);
    assertEquals(3, groups.length);
    assertEquals("AQ", groups[0].toString());
    assertEquals("T8", groups[1].toString());
    assertEquals("AKs", groups[2].toString());
    groups = hv.greater(70);
    assertEquals(1, groups.length);
    assertEquals("AKs", groups[0].toString());
  }

  public void testGreaterEqual() {
    HandGroup[] groups;
    groups = hv.greaterEqual(70);
    assertEquals(3, groups.length);
    assertEquals("AQ", groups[0].toString());
    assertEquals("T8", groups[1].toString());
    assertEquals("AKs", groups[2].toString());
    groups = hv.greaterEqual(71);
    assertEquals(1, groups.length);
    assertEquals("AKs", groups[0].toString());
  }
}
