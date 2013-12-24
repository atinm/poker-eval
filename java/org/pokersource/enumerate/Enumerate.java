// $Id: Enumerate.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate;

import org.pokersource.game.Deck;

/** Algorithms for enumerating or sampling the outcomes of a poker hand
 matchup.
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class Enumerate {
  private Enumerate() {    // don't let anybody instantiate us
  }

  // must match enum_game_t definitions in enumdefs.h
  public static int GAME_HOLDEM = 0;
  public static int GAME_HOLDEM8 = 1;
  public static int GAME_OMAHA = 2;
  public static int GAME_OMAHA8 = 3;
  public static int GAME_7STUD = 4;
  public static int GAME_7STUD8 = 5;
  public static int GAME_7STUDNSQ = 6;
  public static int GAME_RAZZ = 7;
  public static int GAME_5DRAW = 8;
  public static int GAME_5DRAW8 = 9;
  public static int GAME_5DRAWNSQ = 10;
  public static int GAME_LOWBALL = 11;
  public static int GAME_LOWBALL27 = 12;

  static {
    System.loadLibrary("poker");
    System.loadLibrary("pokerjni");
  }

  /**
   Compute all-in pot equity of each player's hand, either by complete
   enumeration of outcomes or by monte carlo sampling.
   @param gameType specifies game (Enumerate.GAME_HOLDEM, etc)
   @param nsamples number of monte carlo samples; if 0, full enumeration
   @param pockets pockets[i] is bitmask of player i's cards
   @param board board is bitmask of board cards
   @param dead dead is bitmask of dead cards
   @param ev output: ev[i] is pot equity of player i
   */
  public static native void PotEquity(int gameType,
                                      int nsamples,
                                      long[] pockets,
                                      long board,
                                      long dead,
                                      double[] ev);

  /**
   Compute all-in pot equity of each player's hand, either by complete
   enumeration of outcomes or by monte carlo sampling.
   @param gameType specifies game (Enumerate.GAME_HOLDEM, etc)
   @param nsamples number of monte carlo samples; if 0, full enumeration
   @param pockets pockets[i] is bitmask of player i's cards
   @param board board is bitmask of board cards
   @param dead dead is bitmask of dead cards
   @param ev output: ev[i] is pot equity of player i
   @param orderKeys output: orderKeys[0][j] corresponds to a particular
   relative ordering of the players' hands at showdown, such that
   orderKeys[0][j][k] is the relative rank of player k's hand, where rank=0
   means best, rank=nplayers-1 means worst, and rank=nplayers indicates a
   non-qualifying hand.  For high-low games, orderKeys[0][j][k+nplayers]
   gives corresponding information for the low hand.  Before calling
   the method, allocate orderKeys as a new int[1][][].
   @param orderVals output: orderVals[0][j] the number of outcomes
   corresponding to the relative rank ordering in orderKeys[0][j].  Before
   calling the method, allocate orderVals as a new int[1][].*/
  public static native void PotEquity(int gameType,
                                      int nsamples,
                                      long[] pockets,
                                      long board,
                                      long dead,
                                      double[] ev,
                                      int[][][] orderKeys,
                                      int[][] orderVals);

  /**
   A simple test of Enumerate methods.
   Compare to "pokenum -h ks kh ad td 9c 8c -- kd jd th / As 2h".
   and "pokenum -O -h ks kh ad td 9c 8c -- kd jd th / As 2h".
   */
  public static void main(String[] args) {
    long[] pockets = new long[3];
    long board;
    long dead;
    // player 0 has Ks Kh
    pockets[0] = Deck.parseCardMask("Ks Kh");

    // player 1 has Ad Td
    pockets[1] = Deck.parseCardMask("Ad Td");

    // player 2 has 9c 8c
    pockets[2] = Deck.parseCardMask("9c 8c");

    // the board is Kd Jd Th
    board = Deck.parseCardMask("Kd Jd Th");

    // another player folded As 2h
    dead = Deck.parseCardMask("As 2h");

    double[] ev = new double[pockets.length];
    int[][][] orderKeys = new int[1][][];
    int[][] orderVals = new int[1][];
    try {

      PotEquity(GAME_HOLDEM, 0, pockets, board, dead, ev);
      for (int i = 0; i < ev.length; i++) {
        System.out.println("In Java: Player " + i + " ev=" + ev[i]);
      }
      System.out.println();

      PotEquity(GAME_HOLDEM, 0, pockets, board, dead, ev,
              orderKeys, orderVals);
      for (int i = 0; i < ev.length; i++) {
        System.out.println("In Java: Player " + i + " ev=" + ev[i]);
      }
      for (int j = 0; j < orderKeys[0].length; j++) {
        for (int k = 0; k < orderKeys[0][j].length; k++) {
          int place = orderKeys[0][j][k] + 1;
          System.out.print(" " + place);
        }
        System.out.println(": " + orderVals[0][j]);
      }

    } catch (Exception e) {
      System.out.println("In Java: caught exception: " + e);
      e.printStackTrace();
    }
  }
}
