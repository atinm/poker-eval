// $Id: StandardEvalTest.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.eval.test;

import junit.framework.TestCase;
import org.pokersource.eval.StandardEval;
import org.pokersource.game.Deck;

/**
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class StandardEvalTest extends TestCase {
  private int[] ranks;
  private int[] suits;

  public StandardEvalTest(String name) {
    super(name);
  }

  public static void main(String args[]) {
    junit.textui.TestRunner.run(StandardEvalTest.class);
  }

  protected void setUp() {
    ranks = new int[5];
    suits = new int[5];
    ranks[0] = Deck.RANK_3;
    suits[0] = Deck.SUIT_HEARTS;
    ranks[1] = Deck.RANK_4;
    suits[1] = Deck.SUIT_HEARTS;
    ranks[2] = Deck.RANK_5;
    suits[2] = Deck.SUIT_HEARTS;
    ranks[3] = Deck.RANK_6;
    suits[3] = Deck.SUIT_HEARTS;
    ranks[4] = Deck.RANK_7;
    suits[4] = Deck.SUIT_HEARTS;
  }

  public void testEvalHigh() {
    long hival = StandardEval.EvalHigh(ranks, suits);
    assertEquals(134545408, hival);
  }

  public void testEvalLow() {
    long loval = StandardEval.EvalLow(ranks, suits);
    assertEquals(414770, loval);
  }

  public void testEvalLow8() {
    long lo8val = StandardEval.EvalLow8(ranks, suits);
    assertEquals(414770, lo8val);
  }
}
