// $Id: AllTests.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate.test;

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
    TestSuite suite = new TestSuite("All org.pokersource.enum tests");
    suite.addTest(new TestSuite(EnumerateTest.class));
    suite.addTest(new TestSuite(HandMatchupTest.class));
    suite.addTest(new TestSuite(HandValuationTest.class));
    suite.addTest(new TestSuite(HoldemAbdulGroupTest.class));
    suite.addTest(new TestSuite(HoldemAtomicGroupTest.class));
    suite.addTest(new TestSuite(HoldemBeliefVectorTest.class));
    suite.addTest(new TestSuite(HoldemCanonGroupTest.class));
    suite.addTest(new TestSuite(HoldemHandGroupFactoryTest.class));
    suite.addTest(new TestSuite(HoldemHandOrderingTest.class));
    suite.addTest(new TestSuite(HoldemSMGroupTest.class));
    suite.addTest(new TestSuite(HoldemThresholdGroupTest.class));
    suite.addTest(new TestSuite(HoldemUniversalGroupTest.class));
    suite.addTest(new TestSuite(SAIETest.class));
    return suite;
  }
}
