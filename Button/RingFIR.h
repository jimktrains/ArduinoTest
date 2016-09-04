#ifndef __RINGFIR_H__
#define __RINGFIR_H__

template<int READINGS, typename T>
class RingFIR
{
  T lastReadings[READINGS];
  T lastReadingsPos;
  unsigned int oneCount;

  public:
    T add(T val);
    T value();
};

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
