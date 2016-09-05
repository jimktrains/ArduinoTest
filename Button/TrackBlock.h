#ifndef __TRACKBLOCK_H__
#define __TRACKBLOCK_H__

enum TrackOcc
{
  None,
  EastBound,
  WestBound
}

/*
 * Yes, there are many more, but some
 * like Stop-and-Procceed or Stop-Grade
 * rely on signage and others aren't
 * applicable to me at this time
 */
enum SignalAspect
{
  /**
   * Proceed not exceding authorized 
   * speed
   */
  Clear,
  /**
   * Proced at medium speed until
   * entire train clears all
   * interlocking or spring 
   * switches, then proceed at 
   * normal speed
   */
  MediumClear,
  /**
   * Proceed at medium to next 
   * signal, being prepared to
   * stop
   */
  Approach,
  /**
   * Stop
   */
  Stop
}

class TrackBlock {
  BinarySensor eastOccSensor;
  BinarySensor westOccSensor;
  TrackOcc occupancy;
}

#endif
