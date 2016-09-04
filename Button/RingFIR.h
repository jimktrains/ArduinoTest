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

#endif
