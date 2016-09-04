#ifndef __RINGFIR_CPP__
#define __RINGFIT_CPP__

#include "RingFIR.h"

template<int READINGS, typename T>
T RingFIR<READINGS, T>::add(T val)
{
  oneCount += (val - lastReadings[lastReadingsPos]);
  lastReadings[lastReadingsPos] = val;
  lastReadingsPos = (lastReadingsPos + 1) % READINGS;
  return value();
}

template<int READINGS, typename T>
T RingFIR<READINGS, T>::value()
{
  return oneCount >= ( (READINGS >> 2) + (READINGS & 1) );
}

#endif
