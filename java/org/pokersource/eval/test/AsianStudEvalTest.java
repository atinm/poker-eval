// $Id: AsianStudEvalTest.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.eval.test;

import junit.framework.TestCase;
import org.pokersource.eval.AsianStudEval;
import org.pokersource.game.Deck;

/**
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class AsianStudEvalTest extends TestCase {
  private int[] ranks;
  private int[] suits;

  public AsianStudEvalTest(String name) {
    super(name);
  }

  public static void main(String args[]) {
    junit.textui.TestRunner.run(AsianStudEvalTest.class);
  }

  protected void setUp() {
    ranks = new int[5];
    suits = new int[5];
    ranks[0] = Deck.RANK_ACE;
    suits[0] = Deck.SUIT_HEARTS;
    ranks[1] = Deck.RANK_7;
    suits[1] = Deck.SUIT_HEARTS;
    ranks[2] = Deck.RANK_8;
    suits[2] = Deck.SUIT_HEARTS;
    ranks[3] = Deck.RANK_9;
    suits[3] = Deck.SUIT_SPADES;
    ranks[4] = Deck.RANK_TEN;
    suits[4] = Deck.SUIT_SPADES;
  }

  public void testEvalHigh() {
    long hival = AsianStudEval.EvalHigh(ranks, suits);
    assertEquals(67633152, hival);
  }
}
