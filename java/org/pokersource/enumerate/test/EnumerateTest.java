// $Id: EnumerateTest.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate.test;

import junit.framework.TestCase;
import org.pokersource.enumerate.Enumerate;
import org.pokersource.game.Deck;

/**
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class EnumerateTest extends TestCase {
  private int[][] pocketRanks;
  private int[][] pocketSuits;
  private int[] boardRanks;
  private int[] boardSuits;
  private int[] deadRanks;
  private int[] deadSuits;
  private long[] pockets;
  private long board;
  private long dead;

  public EnumerateTest(String name) {
    super(name);
  }

  public static void main(String args[]) {
    junit.textui.TestRunner.run(EnumerateTest.class);
  }

  protected void setUp() {
    pocketRanks = new int[3][2];
    pocketSuits = new int[3][2];
    boardRanks = new int[3];
    boardSuits = new int[3];
    deadRanks = new int[2];
    deadSuits = new int[2];
    pockets = new long[3];

    // player 0 has Ks Kh
    pocketRanks[0][0] = Deck.RANK_KING;
    pocketSuits[0][0] = Deck.SUIT_SPADES;
    pocketRanks[0][1] = Deck.RANK_KING;
    pocketSuits[0][1] = Deck.SUIT_HEARTS;
    pockets[0] = Deck.createCardMask(pocketRanks[0], pocketSuits[0]);

    // player 1 has Ad Td
    pocketRanks[1][0] = Deck.RANK_ACE;
    pocketSuits[1][0] = Deck.SUIT_DIAMONDS;
    pocketRanks[1][1] = Deck.RANK_TEN;
    pocketSuits[1][1] = Deck.SUIT_DIAMONDS;
    pockets[1] = Deck.createCardMask(pocketRanks[1], pocketSuits[1]);

    // player 2 has 9c 8c
    pocketRanks[2][0] = Deck.RANK_9;
    pocketSuits[2][0] = Deck.SUIT_CLUBS;
    pocketRanks[2][1] = Deck.RANK_8;
    pocketSuits[2][1] = Deck.SUIT_CLUBS;
    pockets[2] = Deck.createCardMask(pocketRanks[2], pocketSuits[2]);

    // the board is Kd Jd Th
    boardRanks[0] = Deck.RANK_KING;
    boardSuits[0] = Deck.SUIT_DIAMONDS;
    boardRanks[1] = Deck.RANK_JACK;
    boardSuits[1] = Deck.SUIT_DIAMONDS;
    boardRanks[2] = Deck.RANK_TEN;
    boardSuits[2] = Deck.SUIT_HEARTS;
    board = Deck.createCardMask(boardRanks, boardSuits);

    // another player folded As 2h
    deadRanks[0] = Deck.RANK_ACE;
    deadSuits[0] = Deck.SUIT_SPADES;
    deadRanks[1] = Deck.RANK_2;
    deadSuits[1] = Deck.SUIT_HEARTS;
    dead = Deck.createCardMask(deadRanks, deadSuits);
  }

  public void assertEquals(int[][] expected,
                           int[][] observed) {
    assertEquals(expected.length, observed.length);
    for (int i = 0; i < expected.length; i++) {
      assertEquals(expected[i].length, observed[i].length);
      for (int j = 0; j < expected[i].length; j++) {
        assertEquals(expected[i][j], observed[i][j]);
      }
    }
  }

  public void assertEquals(int[][][] expected,
                           int[][][] observed) {
    assertEquals(expected.length, observed.length);
    for (int i = 0; i < expected.length; i++) {
      assertEquals(expected[i].length, observed[i].length);
      for (int j = 0; j < expected[i].length; j++) {
        assertEquals(expected[i][j].length, observed[i][j].length);
        for (int k = 0; k < expected[i][j].length; k++) {
          assertEquals(expected[i][j][k], observed[i][j][k]);
        }
      }
    }
  }

  public void testPotEquity1() {
    // Compare to "pokenum -h ks kh ad td 9c 8c -- kd jd th / As 2h".
    double[] ev = new double[pockets.length];
    Enumerate.PotEquity(Enumerate.GAME_HOLDEM, 0, pockets, board, dead, ev);
    assertEquals(0.531707317073, ev[0], 1e-10);
    assertEquals(0.392682926829, ev[1], 1e-10);
    assertEquals(0.075609756098, ev[2], 1e-10);
  }

  public void testPotEquity2() {
    // Compare to "pokenum -h -O ks kh ad td 9c 8c -- kd jd th / As 2h".
    double[] ev = new double[pockets.length];
    int[][][] orderKeys = new int[1][][];
    int[][] orderVals = new int[1][];
    Enumerate.PotEquity(Enumerate.GAME_HOLDEM, 0, pockets, board, dead, ev,
            orderKeys, orderVals);
    assertEquals(0.531707317073, ev[0], 1e-10);
    assertEquals(0.392682926829, ev[1], 1e-10);
    assertEquals(0.075609756098, ev[2], 1e-10);
    int[][][] expectedKeys = {{{0, 0, 0},
                               {0, 1, 2},
                               {0, 2, 1},
                               {1, 0, 1},
                               {1, 0, 2},
                               {1, 2, 0},
                               {2, 0, 1}}};
    int[][] expectedVals = {{6, 407, 27, 14, 154, 60, 152}};
    assertEquals(expectedKeys, orderKeys);
    assertEquals(expectedVals, orderVals);
  }

  public void testPotEquity3() {
    // Compare to "pokenum -h8 -O Ah 2h 5s 3s 8h 8d -- 2s 4c 4d".
    pockets[0] = Deck.parseCardMask("Ah 2h");
    pockets[1] = Deck.parseCardMask("5s 3s");
    pockets[2] = Deck.parseCardMask("8h 8d");
    board = Deck.parseCardMask("2s 4c 4d");
    dead = 0;
    double[] ev = new double[pockets.length];
    int[][][] orderKeys = new int[1][][];
    int[][] orderVals = new int[1][];
    Enumerate.PotEquity(Enumerate.GAME_HOLDEM8, 0, pockets, board, dead, ev,
            orderKeys, orderVals);
    assertEquals(0.119601328904, ev[0], 1e-10);
    assertEquals(0.373754152824, ev[1], 1e-10);
    assertEquals(0.506644518272, ev[2], 1e-10);
    int[][][] expectedKeys = {{{0, 1, 2, 3, 0, 3},
                               {0, 2, 1, 0, 3, 1},
                               {0, 2, 1, 3, 0, 3},
                               {0, 2, 1, 3, 3, 3},
                               {1, 0, 2, 3, 0, 1},
                               {1, 0, 2, 3, 0, 3},
                               {1, 1, 0, 3, 3, 3},
                               {1, 2, 0, 1, 0, 3},
                               {1, 2, 0, 3, 0, 3},
                               {1, 2, 0, 3, 3, 3},
                               {2, 0, 1, 0, 1, 2},
                               {2, 0, 1, 1, 0, 2},
                               {2, 0, 1, 3, 0, 3},
                               {2, 0, 1, 3, 3, 3},
                               {2, 1, 0, 0, 1, 2},
                               {2, 1, 0, 0, 1, 3},
                               {2, 1, 0, 1, 0, 3},
                               {2, 1, 0, 3, 0, 3},
                               {2, 1, 0, 3, 3, 3}}};
    int[][] expectedVals = {{17, 9, 13, 54, 42, 61, 4, 7, 140, 220,
                             24, 16, 92, 16, 24, 12, 9, 11, 132}};
    assertEquals(expectedKeys, orderKeys);
    assertEquals(expectedVals, orderVals);
  }
}
