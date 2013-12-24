// $Id: HoldemHandGroupFactoryTest.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate.test;

import junit.framework.TestCase;
import org.pokersource.enumerate.HoldemHandGroup;
import org.pokersource.enumerate.HoldemHandGroupFactory;
import org.pokersource.enumerate.HoldemHandOrdering;
import org.pokersource.enumerate.ThresholdHandGroup;
import org.pokersource.game.Deck;

/**
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class HoldemHandGroupFactoryTest extends TestCase {
  public HoldemHandGroupFactoryTest(String name) {
    super(name);
  }

  public static void main(String args[]) {
    junit.textui.TestRunner.run(HoldemHandGroupFactoryTest.class);
  }

  protected void setUp() {
  }

  public void testCache() {
    HoldemHandGroup g1 = HoldemHandGroupFactory.getInstance("AK");
    HoldemHandGroup g2 = HoldemHandGroupFactory.getInstance("Q9s+");
    HoldemHandGroup g3 = HoldemHandGroupFactory.getInstance("AK");
    assertTrue(g1 != g2);
    assertTrue(!g1.equals(g2));
    assertTrue(g1 == g3);
    assertTrue(g1.equals(g3));
  }

  public void testException() {
    try {
      HoldemHandGroup g1 = HoldemHandGroupFactory.getInstance("JUNK");
      fail("Should raise IllegalArgumentException");
    } catch (IllegalArgumentException e) {
    } catch (RuntimeException e) {
      fail("Should raise IllegalArgumentException");
    }
  }

  public void testThreshold() {
    try {
      HoldemHandOrdering hho = new HoldemHandOrdering("sample1.hho");
      ThresholdHandGroup.registerHandValuation("SAMPLE1", hho);
      HoldemHandGroup gt = HoldemHandGroupFactory.getInstance("SAMPLE1>0.95");
      // SAMPLE1>0.95 is [AJs AQs 77 AKs 88 99 TT JJ QQ KK AA]
      assertEquals(4 + 4 + 6 + 4 + 6 + 6 + 6 + 6 + 6 + 6 + 6, gt.numHands());
      assertTrue(!gt.isHandInGroup(Deck.parseCardMask("6h6d")));
      assertTrue(!gt.isHandInGroup(Deck.parseCardMask("AhTh")));
      assertTrue(!gt.isHandInGroup(Deck.parseCardMask("AhKd")));
      assertTrue(!gt.isHandInGroup(Deck.parseCardMask("KcQc")));
      assertTrue(gt.isHandInGroup(Deck.parseCardMask("AhJh")));
      assertTrue(gt.isHandInGroup(Deck.parseCardMask("AsQs")));
      assertTrue(gt.isHandInGroup(Deck.parseCardMask("7s7d")));
      assertTrue(gt.isHandInGroup(Deck.parseCardMask("AsKs")));
      assertTrue(gt.isHandInGroup(Deck.parseCardMask("8s8d")));
      assertTrue(gt.isHandInGroup(Deck.parseCardMask("9s9d")));
      assertTrue(gt.isHandInGroup(Deck.parseCardMask("TsTd")));
      assertTrue(gt.isHandInGroup(Deck.parseCardMask("JhJd")));
      assertTrue(gt.isHandInGroup(Deck.parseCardMask("QhQs")));
      assertTrue(gt.isHandInGroup(Deck.parseCardMask("KhKs")));
      assertTrue(gt.isHandInGroup(Deck.parseCardMask("AhAs")));
    } catch (Exception e) {
      fail("exception " + e);
    }
  }
}
