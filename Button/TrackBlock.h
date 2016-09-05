#ifndef __TRACKBLOCK_H__
#define __TRACKBLOCK_H__

enum TrackOcc {
  None,
  East,
  West
}

class TrackBlock {
  BinarySensor eastOccSensor;
  BinarySensor westOccSensor;
}

#endif
