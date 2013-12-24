/* $Id: AsianStudEvalImp.c 353 2002-06-28 05:56:19Z mjmaurer $ */

#include <stdio.h>
#include <jni.h>
#include "pokerjni.h"	/* javah output for us to implement */
#include "jniutil.h"	/* JNI help like exception throwing */
#include "poker_defs.h"	/* poker-eval basics */
#include "pokutil.h"	/* poker-eval help like card parsing */

#include "deck_astud.h"
#include "rules_astud.h"
#include "inlines/eval_astud.h"	/* must come after above!? */

JNIEXPORT jlong JNICALL Java_org_pokersource_eval_AsianStudEval_EvalHigh
   (JNIEnv *env, jclass class, jintArray ranks, jintArray suits)
{
  int ncards;
  AStudDeck_CardMask mcards;
  HandVal hival;
  
  if (parseAsianStudRanksSuits(env, ranks, suits, &mcards, &ncards)) {
    jniThrow(env, "unable to parse input cards");
    return (jlong)0;
  }
  hival = AStudDeck_AStudRules_EVAL_N(mcards, ncards);
#ifdef DEBUG
  printf("In C: Hand [%s] => Eval: %d ",
         DmaskString(AStudDeck, mcards), hival);
  AStudRules_HandVal_print(hival);
  printf("\n");
#endif
  return (jlong)hival;
}
