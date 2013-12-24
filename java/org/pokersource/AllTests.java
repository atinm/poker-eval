// $Id: AllTests.java 3111 2007-06-19 19:14:23Z jvivenot $

package org.pokersource;

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
    TestSuite suite = new TestSuite("All org.pokersource tests");
    suite.addTest(org.pokersource.game.test.AllTests.suite());
    suite.addTest(org.pokersource.eval.test.AllTests.suite());
    suite.addTest(org.pokersource.enumerate.test.AllTests.suite());
    suite.addTest(org.pokersource.util.test.AllTests.suite());
    return suite;
  }
}
