#ifndef __RINGFIR_H__
#define __RINGFIR_H__

/**
 * Finite Impulse Response filter with uniform weights
 * for binary values.
 * 
 * The template paramter represents the number of
 * readings to include in the filter. (Allows for 
 * stack-only allocation.)
 */
template<int READINGS>
class BinaryRingFIR
{
  boolean lastReadings[READINGS];
  boolean lastReadingsPos;
  unsigned int oneCount;

  public:
    /**
     * Adds a reading to the filter
     */
    boolean add(boolean val);

    /**
     * Returns the current filter value
     */
    boolean value();
};

template<int READINGS>
boolean BinaryRingFIR<READINGS>::add(boolean val)
{
  // Recompute the count of positive readings
  // without looping over the list
  oneCount += (val - lastReadings[lastReadingsPos]);

  // Updates the ring buffer and the ring position
  lastReadings[lastReadingsPos] = val;
  lastReadingsPos = (lastReadingsPos + 1) % READINGS;
  return value();
}

template<int READINGS>
boolean BinaryRingFIR<READINGS>::value()
{
  return oneCount >= ( (READINGS >> 2) + (READINGS & 1) );
}

#endif
