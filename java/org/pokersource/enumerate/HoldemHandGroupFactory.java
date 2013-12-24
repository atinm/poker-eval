// $Id: HoldemHandGroupFactory.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate;

import java.util.HashMap;

/** Creates instances of HoldemHandGroup-implementing objects from their
 string representations.  Use of this factory is preferred to directly
 calling the constructors of subclasses, as the factory maintains a cache
 of immutable instances.

 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class HoldemHandGroupFactory {
  private HoldemHandGroupFactory() {    // don't let anybody instantiate us
  }

  /** Set of registered classes that we know how to instantiate. */
  private static Class[] groupClasses = {
    HoldemAtomicGroup.class,
    HoldemCanonGroup.class,
    HoldemSMGroup.class,
    HoldemAbdulGroup.class,
    HoldemUniversalGroup.class,
    HoldemThresholdGroup.class,
  };

  /** Cache of hand groups already instantiated. */
  private static HashMap cache = new HashMap();

  /** Try to parse groupSpec using class gclass.  If parsing succeeds,
   returns new group object; if parsing fails, returns null; if something
   unexpected happens, throws a runtime exception. */
  private static HoldemHandGroup tryGetInstance(Class gclass,
                                                String groupSpec) {
    HoldemHandGroup group = null;
    try {
      java.lang.reflect.Constructor ctor =
              gclass.getConstructor(new Class[]{String.class});
      group = (HoldemHandGroup) ctor.newInstance(new Object[]{groupSpec});
    } catch (java.lang.reflect.InvocationTargetException e) {
      Throwable te = e.getTargetException();
      if (te instanceof IllegalArgumentException) {
        // This is the exception we throw when unable to parse the
        // group specification.  So ignore it and try another class.
      } else {
        // Something unexpected
        throw new RuntimeException("BUG1: " + te);
      }
    } catch (Exception e) {
      // Something else unexpected
      throw new RuntimeException("BUG2: " + e);
    }
    return group;
  }

  /** Tries to parse groupSpec into a hand group using the gclass constructor.
   If gclass fails to parse groupSpec, throws a runtime exception. */
  public static HoldemHandGroup getInstance(String groupSpec, Class gclass) {
    HoldemHandGroup group;
    // First try cache
    group = (HoldemHandGroup) cache.get(groupSpec);
    if (group != null)
      return group;
    group = tryGetInstance(gclass, groupSpec);
    if (group == null)
      throw new IllegalArgumentException("cannot parse spec: " + groupSpec);
    // Save in cache
    cache.put(groupSpec, group);
    return group;
  }

  /** Tries to parse groupSpec into a hand group using the constructors of the
   list of known classes.  If all known classes fail to parse groupSpec,
   throws a runtime exception. */
  public static HoldemHandGroup getInstance(String groupSpec) {
    HoldemHandGroup group;
    // First try cache
    group = (HoldemHandGroup) cache.get(groupSpec);
    if (group != null)
      return group;
    // Loop through the known classes that can parse holdem hand groups and
    // try each in turn.  If one fails to parse groupSpec, try another until we
    // run out.
    for (int i = 0; i < groupClasses.length && group == null; i++) {
      group = tryGetInstance(groupClasses[i], groupSpec);
    }
    if (group == null)
      throw new IllegalArgumentException("cannot parse spec: " + groupSpec);
    // Save in cache
    cache.put(groupSpec, group);
    return group;
  }
}
