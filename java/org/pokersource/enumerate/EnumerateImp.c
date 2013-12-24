/* $Id: EnumerateImp.c 367 2002-07-14 07:07:08Z mjmaurer $ */

#undef DEBUG

#include <stdio.h>
#include <jni.h>
#include "pokerjni.h"	/* javah output for us to implement */
#include "jniutil.h"	/* JNI help like exception throwing */
#include "poker_defs.h"	/* poker-eval basics */
#include "pokutil.h"	/* poker-eval help like card parsing */

#include "poker_defs.h"
#include "enumdefs.h"


/* copyOrdering()

   Allocate arrays on the Java side and copy the relative hand rank ordering
   information in result->ordering into them.  If something fails we return
   with an Exception pending, so the caller should check whether
   (*env)->ExceptionOccurred(env) is set before proceeding.

   We expect that before we are called orderKeys is an int[][][] already
   allocated with "new int[1][][]".  On exit, we have allocated
   new sub-arrays.  orderKeys[0][j] corresponds to a particular relative
   ordering of the players' hands at showdown, such that orderKeys[0][j][k] is
   the relative rank of player k's hand, where rank=0 means best,
   rank=nplayers-1 means worst, and rank=nplayers indicates a non-qualifying
   hand.  For high-low games, orderKeys[0][j][k+nplayers] gives corresponding
   information for the low hand.  

   Similarly we require that orderVals is an int[][] already allocated with
   "new int[1][]".  orderVals[0][j] the number of outcomes corresponding to
   the relative rank ordering in orderKeys[0][j].
  */
static void
copyOrdering(JNIEnv *env, enum_result_t *result,
             jobjectArray orderKeys, jobjectArray orderVals)
{
  /* allocate ordering arrays and copy from result */
  int i, j, k;
  int nord;
  jclass clazz;
  jobjectArray keys;
  jintArray vals;
  jintArray ranks;
  jint *valsBuf;
  jint *ranksBuf;
  int nranks;
  int nplayers;
  nranks = ((result->ordering->mode == enum_ordering_mode_hilo ? 2 : 1)
            * result->ordering->nplayers);
  nplayers = result->ordering->nplayers;

  /* Find out how many orderings we have */
  for (i=nord=0; i<result->ordering->nentries; i++)
    if (result->ordering->hist[i] > 0)
      nord++;

  /* Allocate arrays, equivalent to
       [JAVA:] keys = new int[nord][];
       [JAVA:] vals = new int[nord];
     Later we will say
       [JAVA:] orderKeys[0] = keys;
       [JAVA:] orderVals[0] = vals;
  */
  clazz = (*env)->FindClass(env, "[I");
  if (clazz == 0) {
    jniThrow(env, "BUG");
    return;
  } 
  keys = (*env)->NewObjectArray(env, nord, clazz, 0);
  if (keys == NULL) {
    jniThrow(env, "out of memory");
    return;
  }
  vals = (*env)->NewIntArray(env, nord);
  if (vals == NULL) {
    jniThrow(env, "out of memory");
    return;
  }

  /* Get pointer into vals */
  valsBuf = (*env)->GetIntArrayElements(env, vals, NULL);
  if (valsBuf == NULL) {
    jniThrow(env, "failed to get array elements");
    return;
  }

  /* Loop over orderings, allocating keys[nord] and assigning vals[nord]
     for each */
  for (i=nord=0; i<result->ordering->nentries; i++) {
    if (result->ordering->hist[i] > 0) {
      /* [JAVA:] ranks = new int[nranks]; */
      ranks = (*env)->NewIntArray(env, nranks);
      if (ranks == NULL) {
        jniThrow(env, "out of memory");
        return;
      }

      /* Decode ordering key i and copy into ranks[] */
      ranksBuf = (*env)->GetIntArrayElements(env, ranks, NULL);
      if (ranksBuf == NULL) {
        jniThrow(env, "failed to get array elements");
        return;
      }
      if (result->ordering->mode == enum_ordering_mode_hilo) {
        for (k=0; k<nplayers; k++) {
          ranksBuf[k] = ENUM_ORDERING_DECODE_HILO_K_HI(i, nplayers, k);
        }
        for (k=0; k<nplayers; k++) {
          ranksBuf[k + nplayers] =
            ENUM_ORDERING_DECODE_HILO_K_LO(i, nplayers, k);
        }
      } else {
        for (k=0; k<nplayers; k++) {
          ranksBuf[k] = ENUM_ORDERING_DECODE_K(i, nplayers, k);
        }
      }
      (*env)->ReleaseIntArrayElements(env, ranks, ranksBuf, 0);      

      /* [JAVA:] keys[nord] = ranks; */
      (*env)->SetObjectArrayElement(env, keys, nord, ranks);
      if ((*env)->ExceptionOccurred(env) != NULL)
        return;
      (*env)->DeleteLocalRef(env, ranks);
      if ((*env)->ExceptionOccurred(env) != NULL)
        return;

      /* [JAVA:] vals[nord] = hist[i]; */
      valsBuf[nord] = result->ordering->hist[i];
      nord++;
    }    
  }
  (*env)->ReleaseIntArrayElements(env, vals, valsBuf, 0);

  /* [JAVA:] orderVals[0] = vals; */
  (*env)->SetObjectArrayElement(env, orderVals, 0, vals);
  if ((*env)->ExceptionOccurred(env) != NULL)
    return;

  /* [JAVA:] orderKeys[0] = keys; */
  (*env)->SetObjectArrayElement(env, orderKeys, 0, keys);
  if ((*env)->ExceptionOccurred(env) != NULL)
    return;
}

static void
Enumerate_PotEquity(JNIEnv *env, jint gameType, jint nsamples,
                    jlongArray lpockets, jlong jboard, jlong jdead,
                    jdoubleArray ev, jobjectArray orderKeys,
                    jobjectArray orderVals)
{
  int i, nplayers, nboard, orderflag, err;
  jlong *jpockets;
  StdDeck_CardMask board, dead, pockets[ENUM_MAXPLAYERS];
  enum_result_t result;

  enumResultClear(&result);
  nplayers = (*env)->GetArrayLength(env, lpockets);
  if (nplayers > ENUM_MAXPLAYERS) {
    jniThrow(env, "too many players");
    return;
  }

  /* parse list of dead cards */
  dead = parseLongStandard(jdead);

  /* parse list of pocket cards for each player, also add to dead cards */
  jpockets = (*env)->GetLongArrayElements(env, lpockets, 0);
  for (i=0; i<nplayers; i++) {
    pockets[i] = parseLongStandard(jpockets[i]);
    StdDeck_CardMask_OR(dead, dead, pockets[i]);
  }
  (*env)->ReleaseLongArrayElements(env, lpockets, jpockets, JNI_ABORT);
  if ((*env)->ExceptionOccurred(env) != NULL)
    return;

  /* parse list of board cards, also add to dead cards */
  board = parseLongStandard(jboard);
  nboard = numCardsStandard(board);
  StdDeck_CardMask_OR(dead, dead, board);

  /* do the heavy lifting of enumeration */
  orderflag = (orderKeys != NULL);    /* do we need detailed ordering info? */
  if (nsamples == 0) {
    err = enumExhaustive(gameType, pockets, board, dead, nplayers, nboard,
                         orderflag, &result);
  } else {
    err = enumSample(gameType, pockets, board, dead, nplayers, nboard, nsamples,
                     orderflag, &result);
  }
  if (err != 0) {
    jniThrow(env, "internal error in C library");
    goto cleanup;
  }
#ifdef DEBUG
  enumResultPrint(&result, pockets, board);
  fflush(stdout);
#endif

  /* scale EV result and send back to Java side */
  for (i=0; i<nplayers; i++)
    result.ev[i] /= result.nsamples;
  (*env)->SetDoubleArrayRegion(env, ev, 0, nplayers, (jdouble *)result.ev);
  if ((*env)->ExceptionOccurred(env) != NULL)
    goto cleanup;

  /* copy ordering information to Java side, if requested */
  if (orderflag) {
    copyOrdering(env, &result, orderKeys, orderVals);
    if ((*env)->ExceptionOccurred(env) != NULL)
      goto cleanup;
  }

 cleanup:
  enumResultFree(&result);
  return;
}

JNIEXPORT void JNICALL 
Java_org_pokersource_enum_Enumerate_PotEquity__II_3JJJ_3D
  (JNIEnv *env, jclass class, jint gameType, jint nsamples,
   jlongArray lpockets, jlong jboard, jlong jdead, jdoubleArray ev)
{
  Enumerate_PotEquity(env, gameType, nsamples, lpockets, jboard,
                      jdead, ev, 0, 0);
}

/*
 * Class:     org_pokersource_enum_Enumerate
 * Method:    PotEquity
 * Signature: (II[JJJ[D[[[I[[I)V
 */
JNIEXPORT void JNICALL Java_org_pokersource_enum_Enumerate_PotEquity__II_3JJJ_3D_3_3_3I_3_3I
(JNIEnv *env, jclass class, jint gameType, jint nsamples,
 jlongArray lpockets, jlong jboard, jlong jdead,
 jdoubleArray ev, jobjectArray orderKeys, jobjectArray orderVals)
{
  Enumerate_PotEquity(env, gameType, nsamples, lpockets, jboard,
                      jdead, ev, orderKeys, orderVals);
}
