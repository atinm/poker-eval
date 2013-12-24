// $Id: NestedLoopEnumerationTest.java 3118 2007-06-19 19:31:11Z jvivenot $

package org.pokersource.util.test;

import junit.framework.TestCase;
import org.pokersource.util.NestedLoopEnumeration;

/**
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class NestedLoopEnumerationTest extends TestCase {
  public NestedLoopEnumerationTest(String name) {
    super(name);
  }

  public static void main(String args[]) {
    junit.textui.TestRunner.run(NestedLoopEnumerationTest.class);
  }

  public void testBasic() {
    int[] limits = {2, 3, 2};
    int[][] expected = {
      {0, 0, 0},
      {0, 0, 1},
      {0, 1, 0},
      {0, 1, 1},
      {0, 2, 0},
      {0, 2, 1},
      {1, 0, 0},
      {1, 0, 1},
      {1, 1, 0},
      {1, 1, 1},
      {1, 2, 0},
      {1, 2, 1}
    };
    NestedLoopEnumeration enumerate = new NestedLoopEnumeration(limits);
    for (int i = 0; i < expected.length; i++) {
      assertTrue(enumerate.hasMoreElements());
      int[] elem = (int[]) enumerate.nextElement();
      assertEquals(expected[i].length, elem.length);
      for (int j = 0; j < expected[i].length; j++)
        assertEquals(expected[i][j], elem[j]);
    }
    assertTrue(!enumerate.hasMoreElements());
  }

}
