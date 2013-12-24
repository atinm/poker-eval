// $Id: HoldemUniversalGroupTest.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate.test;

import junit.framework.TestCase;
import org.pokersource.enumerate.HoldemUniversalGroup;
import org.pokersource.game.Deck;

/**
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class HoldemUniversalGroupTest extends TestCase {
  private HoldemUniversalGroup univ;
  private long AhAd;
  private long KhQh;
  private long KhQs;
  private long Td9d;
  private long Td9c;

  public HoldemUniversalGroupTest(String name) {
    super(name);
  }

  public static void main(String args[]) {
    junit.textui.TestRunner.run(HoldemUniversalGroupTest.class);
  }

  protected void setUp() {
    univ = new HoldemUniversalGroup("<any>");
    AhAd = Deck.parseCardMask("AhAd");
    KhQh = Deck.parseCardMask("KhQh");
    KhQs = Deck.parseCardMask("KhQs");
    Td9d = Deck.parseCardMask("Td9d");
    Td9c = Deck.parseCardMask("Td9c");
  }

  public void testToString() {
    assertEquals("<any>", univ.toString());
  }

  public void testIsHandInGroup() {
    assertTrue(univ.isHandInGroup(AhAd));
    assertTrue(univ.isHandInGroup(KhQh));
    assertTrue(univ.isHandInGroup(KhQs));
    assertTrue(univ.isHandInGroup(Td9d));
    assertTrue(univ.isHandInGroup(Td9c));
  }

  public void testGetHands() {
    assertEquals(1326, univ.getHands().length);
  }

}
