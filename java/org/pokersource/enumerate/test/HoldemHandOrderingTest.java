// $Id: HoldemHandOrderingTest.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate.test;

import junit.framework.TestCase;
import org.pokersource.enumerate.HandGroup;
import org.pokersource.enumerate.HoldemHandOrdering;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

/**
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class HoldemHandOrderingTest extends TestCase {
  private static final String SAMPLE_PATH = "sample1.hho";
  private HoldemHandOrdering hho1;

  public HoldemHandOrderingTest(String name) {
    super(name);
  }

  public static void main(String args[]) {
    junit.textui.TestRunner.run(HoldemHandOrderingTest.class);
  }

  protected void setUp() {
    try {
      ClassLoader cl = ClassLoader.getSystemClassLoader();
      InputStream stream = cl.getResourceAsStream(SAMPLE_PATH);
      if (stream == null)
        throw new FileNotFoundException(SAMPLE_PATH);
      hho1 = new HoldemHandOrdering(stream);
    } catch (IOException e) {
      throw new RuntimeException("caught " + e);
    }
  }

  public void testToString() {
    assertTrue(hho1.toString().startsWith("32:0.0 42:0.00904977"));
  }

  public void testGreaterEqual() {
    HandGroup[] groups = hho1.greaterEqual(0.95);
    // SAMPLE1>0.95 is [AJs AQs 77 AKs 88 99 TT JJ QQ KK AA]
    assertEquals(11, groups.length);
    int i = 0;
    assertEquals("AJs", groups[i++].toString());
    assertEquals("AQs", groups[i++].toString());
    assertEquals("77", groups[i++].toString());
    assertEquals("AKs", groups[i++].toString());
    assertEquals("88", groups[i++].toString());
    assertEquals("99", groups[i++].toString());
    assertEquals("TT", groups[i++].toString());
    assertEquals("JJ", groups[i++].toString());
    assertEquals("QQ", groups[i++].toString());
    assertEquals("KK", groups[i++].toString());
    assertEquals("AA", groups[i++].toString());
  }
}
