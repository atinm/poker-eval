/* $Id: jniutil.c 353 2002-06-28 05:56:19Z mjmaurer $ */

#include <jni.h>

/* Try to throw an exception back to java.  This may fail, so anyone calling
   this function should assume that it may return. */
void
jniThrow(JNIEnv *env, char *msg)
{
  jclass exClass;

  exClass = (*env)->FindClass(env, "java/lang/IllegalArgumentException");
  if (exClass == 0) { /* Unable to find the new exception class, give up. */
    return;
  }
  (*env)->ThrowNew(env, exClass, msg);
}

