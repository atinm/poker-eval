// $Id: AllTests.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.game.test;

import junit.framework.Test;
import junit.framework.TestSuite;

/**
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class AllTests {
  public static void main(String[] args) {
    junit.textui.TestRunner.run(suite());
  }

  public static Test suite() {
    TestSuite suite = new TestSuite("All org.pokersource.game tests");
    suite.addTest(new TestSuite(DeckTest.class));
    return suite;
  }
}
