// $Id: SAIEMain.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate;

import gnu.getopt.Getopt;
import org.pokersource.game.Deck;

import java.io.FileNotFoundException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.StreamTokenizer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.TreeMap;

/** An executable program that provides access to the SAIE calculator.
 <pre>
 Usage: java org.pokersource.enumerate.SAIEMain [options] bv1 bv2 ...
 where bv1 is a HoldemBeliefVector in string form
 Options:
 -b board           Community cards
 -d dead            Dead cards
 -m nmatchups       Number of matchups to sample [default all]
 -n noutcomes       Number of outcomes for each matchup [default all]
 -r NAME:file       Register hand ordering in file as NAME
 -O                 Track relative hand rank orderings
 -t                 Terse output [default human-readable]
 -i                 Read hands from stdin instead of command line
 -v level           Verbosity level [default 0]

 Streaming usage: java pokeval.SAIEMain -i
 followed by standard input of command lines as above

 Examples:
 java org.pokersource.enumerate.SAIEMain -b Ah3d2c9d JsJd 'AQs+ TT+'

 java org.pokersource.enumerate.SAIEMain -m 100 -n 100 'AQs+ TT+' AK

 java org.pokersource.enumerate.SAIEMain -O -r SAMP:sample1.hho \
 -m 100 -n 100 'AhTd' 'SAMP>0.80' 'SAMP>0.95'
 </pre>
 @see HoldemHandGroup HoldemHandGroup (and the classes that implement it,
 for the syntax of hand groups)
 @see HoldemBeliefVector HoldemBeliefVector (for the syntax of belief
 vectors)
 @see HoldemHandOrdering HoldemHandOrdering (for the format of hand rank
 ordering files)
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class SAIEMain {
  private SAIEMain() {    // don't let anybody instantiate us
  }

  /** How chatty to be on System.out */
  private static int verbose = 0;

  /** The number of matchups to sample (0 = full enumeration) */
  private static int nmatchups = 0;

  /** The number of outcomes for each matchup (0 = full enumeration) */
  private static int noutcomes = 0;

  /** Flag for terse (machine-readable) output */
  private static boolean terseFlag = false;

  /** Flag for including the relative rank ordering histogram in output */
  private static boolean orderingFlag = false;

  /** Flag for reading hands from stdin instead of command line */
  private static boolean stdinFlag = false;

  /** A Map of {String path, HoldemHandOrdering hho} for all registered hand
   orderings */
  private static HashMap orderingObjs = new HashMap();

  /** For each player, a distribution of hands */
  private static ArrayList beliefs = new ArrayList();

  /** A cache of belief vectors we've already instantiated, map of {String,
   HoldemBeliefVector} */
  private static HashMap bvcache = new HashMap();

  /** Community board cards */
  private static long board = 0;

  /** Dead cards not in play */
  private static long dead = 0;

  private static void usage() {
    String[] u = {
      "Usage: java org.pokersource.enumerate.SAIEMain [options] bv1 bv2 ...",
      "  where bv1 is a HoldemBeliefVector in string form",
      "Options:",
      "  -b board           Community cards",
      "  -d dead            Dead cards",
      "  -m nmatchups       Number of matchups to sample [default all]",
      "  -n noutcomes       Number of outcomes for each matchup [default all]",
      "  -r NAME:file       Register hand ordering in file as NAME",
      "  -O                 Track relative hand rank orderings",
      "  -t                 Terse output [default human-readable]",
      "  -i                 Read hands from stdin instead of command line",
      "  -v level           Verbosity level [default 0]",
      "",
      " Streaming usage: java pokeval.SAIEMain -i",
      "  followed by standard input of command lines as above",
      "",
      " Examples:",
      "  java org.pokersource.enumerate.SAIEMain -b Ah3d2c9d JsJd 'AQs+ TT+'",
      "",
      "   java org.pokersource.enumerate.SAIEMain -m 100 -n 100 'AQs+ TT+' AK",
      "",
      "   java org.pokersource.enumerate.SAIEMain -O -r SAMP:sample1.hho \\",
      "               -m 100 -n 100 'AhTd' 'SAMP>0.80' 'SAMP>0.95'",
    };
    for (int i = 0; i < u.length; i++)
      System.err.println(u[i]);
  }

  /** @return True indicates success */
  private static boolean parseArgs(String[] argv) {
    if (verbose >= 1) {
      System.out.print("Parsing:");
      for (int i = 0; i < argv.length; i++)
        System.out.print(" " + argv[i]);
      System.out.println();
    }
    // the following do not persist across invocations
    orderingFlag = terseFlag = false;
    nmatchups = noutcomes = 0;
    board = dead = 0;
    beliefs.clear();
    try {
      Getopt g = new Getopt("SAIEMain", argv, "b:d:m:n:r:Otiv:");
      int c;
      while ((c = g.getopt()) != -1) {
        switch (c) {
          case 'b':
            board = Deck.parseCardMask(g.getOptarg());
            break;
          case 'd':
            dead = Deck.parseCardMask(g.getOptarg());
            break;
          case 'm':
            nmatchups = Integer.parseInt(g.getOptarg());
            break;
          case 'n':
            noutcomes = Integer.parseInt(g.getOptarg());
            break;
          case 'r':
            String arg = g.getOptarg();
            int delimpos = arg.indexOf(":");
            if (delimpos < 0)
              throw new IllegalArgumentException("cannot parse " + arg);
            String name = arg.substring(0, delimpos);
            String path = arg.substring(delimpos + 1, arg.length());
            if (verbose >= 1)
              System.out.println("Registering " + arg + " as " + path);
            HoldemHandOrdering hho = (HoldemHandOrdering) orderingObjs.get(path);
            if (hho == null) {
              if (verbose >= 1)
                System.out.println("Loading and parsing " + path);
              ClassLoader cl = ClassLoader.getSystemClassLoader();
              InputStream stream = cl.getResourceAsStream(path);
              if (stream == null)
                throw new FileNotFoundException(path);
              hho = new HoldemHandOrdering(stream);
              orderingObjs.put(path, hho);
            }
            ThresholdHandGroup.registerHandValuation(name, hho);
            break;
          case 'O':
            orderingFlag = true;
            break;
          case 't':
            terseFlag = true;
            break;
          case 'i':
            stdinFlag = true;
            break;
          case 'v':
            verbose = Integer.parseInt(g.getOptarg());
            break;
          case '?':
            return false;
          default:
            System.err.print("getopt() returned " + c + "\n");
            return false;
        }
      }
      // now parse non-option arguments: beliefs
      for (int i = g.getOptind(); i < argv.length; i++) {
        HoldemBeliefVector bv = (HoldemBeliefVector) bvcache.get(argv[i]);
        if (bv != null) {
          if (verbose >= 1)
            System.out.println("Using cached belief vector " + argv[i]);
        } else {
          if (verbose >= 1)
            System.out.println("Creating new belief vector " + argv[i]);
          bv = new HoldemBeliefVector(argv[i]);
          bvcache.put(argv[i], bv);
        }
        beliefs.add(bv);
      }
    } catch (Exception e) {
      e.printStackTrace(System.err);
      return false;
    }
    return true;
  }

  private static void chat1() {
    if (verbose >= 1) {
      int nplayers = beliefs.size();
      System.out.println("nplayers=" + nplayers +
              ", nmatchups=" + nmatchups +
              ((nmatchups == 0) ? " (enumerate)" : " (sample)") +
              ", noutcomes=" + noutcomes +
              ((noutcomes == 0) ? " (enumerate)" : " (sample)"));
      long prodnh = 1;
      for (int i = 0; i < nplayers; i++) {
        HoldemBeliefVector bv = (HoldemBeliefVector) beliefs.get(i);
        System.out.println("bv[" + i + "] = " + bv +
                " (" + bv.numHands() + " atomic hands)");
        prodnh *= bv.numHands();
      }
      System.out.println("board = " + Deck.cardMaskString(board));
      System.out.println("dead = " + Deck.cardMaskString(dead));
      System.out.println("Upper bound on matchups: " + prodnh);
      int nd = 52 - Deck.numCards(board) - Deck.numCards(dead) - 2 * nplayers;
      int nboards = 0;
      if (Deck.numCards(board) == 0)
        nboards = nd * (nd - 1) * (nd - 2) * (nd - 3) * (nd - 4) / (5 * 4 * 3 * 2);
      else if (Deck.numCards(board) == 3)
        nboards = nd * (nd - 1) / 2;
      else if (Deck.numCards(board) == 4)
        nboards = nd;
      else if (Deck.numCards(board) == 5)
        nboards = 1;
      System.out.println("Outcomes per matchup: " + nboards);
      long maxeval = (long) nboards * prodnh * nplayers;
      System.out.println("Upper bound on hand evals: " + maxeval);
    }
  }

  private static void execute() {
    chat1();
    if (verbose >= 1) {
      System.gc();
      System.runFinalization();
    }
    long timeBegin = System.currentTimeMillis();
    int nplayers = beliefs.size();
    HoldemBeliefVector[] bvv = (HoldemBeliefVector[])
            beliefs.toArray(new HoldemBeliefVector[nplayers]);
    double[] ev = new double[nplayers];
    TreeMap orderings = null;
    if (orderingFlag)
      orderings = new TreeMap();

    // do heavy lifting
    try {
      SAIE.FlopGameSAIE(Enumerate.GAME_HOLDEM, nmatchups, noutcomes,
              bvv, board, dead, ev, null, orderings);
    } catch (RuntimeException e) {
      System.out.println("ERROR internal error");
      e.printStackTrace(System.err);
      return;
    }

    // print pot equity of each player
    if (verbose >= 1)
      System.out.println("Player pot equity:");
    System.out.println(":\tEV " + nplayers);
    for (int i = 0; i < nplayers; i++) {
      System.out.println(":\t" + i + "\t" + ev[i]);
    }

    // print relative rank ordering histogram, if requested
    if (orderings != null) {
      if (verbose >= 1)
        System.out.println("Relative rank ordering histogram:");
      System.out.println("::\tORD " + orderings.size() + " " + nplayers);
      for (Iterator iter = orderings.keySet().iterator(); iter.hasNext();) {
        RankOrdering ranks = (RankOrdering) iter.next();
        Integer count = (Integer) orderings.get(ranks);
        System.out.println("::\t" + ranks + " " + count);
      }
    }
    long timeElapsed = System.currentTimeMillis() - timeBegin;
    if (verbose >= 1) {
      System.out.println("Elapsed time: " + timeElapsed + "ms");
      System.out.println();
    }
  }

  public static void main(String[] argv) {
    if (!parseArgs(argv)) {
      usage();
      return;
    }
    if (!stdinFlag) {
      execute();
    } else {
      System.out.println("Reading stdin...");
      try {
        InputStreamReader isr = new InputStreamReader(System.in);
        StreamTokenizer st = new StreamTokenizer(isr);
        st.resetSyntax();
        st.wordChars(0, 255);
        st.whitespaceChars(0, ' ');
        st.quoteChar('\'');
        st.quoteChar('"');
        st.eolIsSignificant(true);
        ArrayList argl = new ArrayList();
        int tok;
        do {
          tok = st.nextToken();
          if (tok == st.TT_WORD || tok == '"' || tok == '\'') {
            argl.add(new String(st.sval));
          } else if (tok == st.TT_EOL || tok == st.TT_EOF) {
            if (argl.size() > 0) {
              String[] args = (String[]) argl.toArray(new String[argl.size()]);
              if (!parseArgs(args))
                System.out.println("ERROR failed to parse arguments");
              else
                execute();
              argl.clear();
            } else if (tok == st.TT_EOL) {
              System.out.println("ERROR no arguments found");
            }
            System.out.flush();
          } else {
            throw new RuntimeException("unexpected tokenizing bug");
          }
        } while (tok != st.TT_EOF);
      } catch (Exception e) {
        e.printStackTrace(System.err);
        return;
      }
    }
  }
}
