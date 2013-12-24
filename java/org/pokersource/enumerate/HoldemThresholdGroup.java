// $Id: HoldemThresholdGroup.java 384 2004-05-11 06:27:47Z mjmaurer $

package org.pokersource.enumerate;

/**
 A holdem-specific implementation of ThresholdHandGroup.
 @see ThresholdHandGroup
 @author Michael Maurer &lt;<a href="mailto:mjmaurer@yahoo.com">mjmaurer@yahoo.com</a>&gt;
 */

public class HoldemThresholdGroup extends ThresholdHandGroup
        implements HoldemHandGroup {
  public HoldemThresholdGroup(String groupSpec) {
    super(groupSpec);
  }
}
