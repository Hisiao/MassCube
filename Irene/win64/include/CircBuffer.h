/******************************************************************************
$HeadURL$

 File: CircBuffer.h

 Description: Template declarations for a 'circular buffer' data container,
   that is a cross between a queue and a vector.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2015 Atmospheric and Environmental Research, Inc. (AER)

   DISTRIBUTION A. Approved for public release; distribution is unlimited.

   The AE9/AP9/SPM software license is contained in the 'documents/Licenses' 
   folder of this distribution file collection.

 Author:

   This software was developed by AER staff

 Contact:

   Atmospheric and Environmental Research, Inc.
   131 Hartwell Avenue
   Lexington, MA 02421-3126 USA
   Phone: 781.761.2288
   email: spwx@aer.com

 References:
 
   None

 Revision history:

  Version      Date        Notes
  1.0          09/18/2015  Created 
  
SVNTag: $Id$
******************************************************************************/

#ifndef CIRCBUFFER_H
#define CIRCBUFFER_H

#include <vector>
#include <stdexcept>
#include <iterator>
#include <iostream>

// class CircBuffer is a cross between a queue and a vector
//    * data is pushed on and popped off like a queue
//      (these operations are just copying the data values to an internal class 
//       vector and/or changing some buffer indices - no memory resizing needed)
//    * when amount of data exceeds capacity, oldest entries are overwritten
//    * random access to data is permitted like a vector, using indices that
//       are in reference to the 'front'
//    * minimal internal memory operations: allocation only occurs during
//       buffer initialization or for explicit buffer resizing requests.

template <typename Type>
class CircBuffer
{
  public:
    CircBuffer (int iSize=0);
    ~CircBuffer ();
    void clear ();
    void resize (const int& iSize);
    void assign (std::vector<int>::iterator itStart, 
                std::vector<int>::iterator itBeyond);
    Type front ();
    Type back ();
    Type at (const int& iRef=0);
    Type operator[] (const int& iRef);
    Type pop ();
    Type pop_front () { return pop(); }
    void push_back (const Type& tValue);
    void push_back (const std::vector<Type>& vtValues);
    void saveState ();
    void restoreState ();
    size_t capacity() { return size_t(m_iBufferSize); }
    size_t size() { return size_t(m_iBufferCount); }
    bool isFull() { return (m_iBufferSize>0 && m_iBufferSize==m_iBufferCount); }
    bool isEmpty() { return (m_iBufferCount==0); }

  private:
    std::vector<Type> m_vtBuffer;
    int m_iBufferSize;   // buffer capacity
    int m_iBufferCount;  // number of data values in buffer
    int m_iBeginIndex;   // index of 'front' data value
    int m_iEndIndex;     // index beyond 'end' data value
    int m_iDefaultSize;

    // copy of values to permit 'restore' to prior buffer state,
    //  provided that no new data has been added, or buffer resized.
    // --this is possible since the 'pop' operation is non-destructive
    int m_iBufferSizeSnap;  // doubles as flag for unrestorable changes
    int m_iBufferCountSnap;
    int m_iBeginIndexSnap;
    int m_iEndIndexSnap;
};

// ----------------------------------------

template <typename Type>
CircBuffer<Type>::CircBuffer(int iSize)
  : m_iBufferCount(0)
  , m_iBeginIndex(0)
  , m_iEndIndex(0)
  , m_iDefaultSize(32)  // in case no initial sizing before first push_back
  , m_iBufferSizeSnap(-1)
{

  m_iBufferSize = iSize;
  // check for invalid size request
  if (iSize < 0) {
    std::cerr << "Warning: sizing circular buffer to " << m_iDefaultSize
              << "; requested sizing " << iSize << " ignored" << std::endl;
    m_iBufferSize = m_iDefaultSize;
  }

  m_vtBuffer.resize(m_iBufferSize);
}

template <typename Type>
CircBuffer<Type>::~CircBuffer()
{
  m_vtBuffer.clear();
}

// clear() : resets buffer to have no stored entry values 
//  -- changed to also remove memory allocation of underlying vector
template <typename Type>
void CircBuffer<Type>::clear()
{
  // reset counter and indices
  m_iBufferCount = 0;
  m_iBeginIndex = 0;
  m_iEndIndex = 0;

  m_vtBuffer.clear();
  m_iBufferSize = 0;
}

// resize(iSize) : changes the maximum number of entries (changes memory allocation)
template <typename Type>
void CircBuffer<Type>::resize(const int& iSize)
{
  // check for no change in size
  if (iSize == m_iBufferSize) return;

  // check for invalid size request
  if (iSize < 1) {
    std::cerr << "Warning request to resize circular buffer to "
              << iSize << " ignored" << std::endl;
    return;
  }

  // save current size, update to new size
  int iPrevSize = m_iBufferSize;
  m_iBufferSize = iSize;

  m_vtBuffer.resize(m_iBufferSize);
  
  // check if size being reduced
  if (m_iBufferSize < iPrevSize) {
    if (m_iBufferCount > 0)
      std::cerr << "Warning: resize to smaller capacity: all loaded data removed from circular buffer"
                << std::endl;
    clear(); // reset indices
    return;
  }

  // nothing further to do if no data in buffer
  if (m_iBufferCount == 0) return;

  // when iSize > m_iBufferSize : can maintain existing data
  //  but may require moving of data, depending where begin/end are

  // check if ready to wrap around end, but no data wrapped yet
  if (m_iEndIndex == 0) {
    // revise end index and return
    m_iEndIndex = iPrevSize;
    return;
  }

  // data is contiguous, just return 
  if ( m_iEndIndex > m_iBeginIndex ) return;

  // data is wrapped: move data from lower indices to past end of previous size
  int ii = 0;
  int iNewEnd = iPrevSize;
  while (ii < m_iEndIndex) {
//diag__ std::cerr << "  --moving data from "<<ii<<" to "<<iNewEnd<<"\n";
    m_vtBuffer[iNewEnd++] = m_vtBuffer[ii++];
    if ( iNewEnd>=iSize ) iNewEnd = 0;
  }
  m_iEndIndex = iNewEnd;

}

// assign(itStart,itBeyond) : clears buffer, then fills it with new entry values
template <typename Type>
void CircBuffer<Type>::assign(std::vector<int>::iterator itStart, 
                              std::vector<int>::iterator itBeyond)
{
  // determine number of entries being assigned
  int iCount = std::distance(itStart,itBeyond);
  // check that this does not exceed buffer capacity
  if ( iCount > m_iBufferSize ) {
    std::cerr << "Error: 'assign' exceeds capacity of circular buffer" << std::endl;
    return;
  }

  // clear out any current data entries
  clear();
  // assign new data entries
  m_vtBuffer.assign ( itStart, itBeyond );

  // update buffer indices
  m_iBufferCount = iCount;
  m_iEndIndex = iCount;

  // clear value for 'restoreState' capability
  m_iBufferSizeSnap = -1;
}

// cannot define 'begin()' and 'end()' methods
//  as returned iterators most likely non-contiguous
// --attempts to iterate on them would fail

// front() : returns reference to first entry value
template <typename Type>
Type CircBuffer<Type>::front()
{
  return at(0);
}

// back() : returns reference to last entry value
template <typename Type>
Type CircBuffer<Type>::back()
{
  return at(m_iBufferCount-1);
}

// at() : returns reference to entry value at index in reference to 'front'
template <typename Type>
Type CircBuffer<Type>::at(const int& iRef) // default=0 (ie front)
{
  if (m_iBufferCount == 0) {
    throw std::out_of_range("CircBuffer<>::at(): empty buffer"); 
  }
  // check for reference greater than number of entries loaded
  if ( iRef >= m_iBufferCount ) {
    throw std::out_of_range("CircBuffer<>::at(): reference beyond end of circular buffer");
  }
  // get actual buffer index of referenced entry
  int iIndex = (m_iBeginIndex + iRef ) % m_iBufferSize;
  const Type& entry = m_vtBuffer[iIndex];

  return entry;
}

// operator[] : returns reference to entry value at index in reference to 'front'
template <typename Type>
Type CircBuffer<Type>::operator[](const int& iRef)
{
  //--no bounds checking, but will point to an entry location (except when unallocated)
  // get actual buffer index of referenced entry
  int iIndex = (m_iBeginIndex + iRef ) % m_iBufferSize;
  const Type& entry = m_vtBuffer[iIndex];

  return entry;
}

// pop() : returns reference to first entry value, and 'removes' it from buffer
template <typename Type>
Type CircBuffer<Type>::pop()
{
  if (m_iBufferCount == 0) {
    throw std::out_of_range("CircBuffer<>::pop(): empty buffer"); 
  }
  // reference to value will still be valid after adjusting buffer indices
  const Type& result = m_vtBuffer[m_iBeginIndex];
  m_iBeginIndex = (m_iBeginIndex + 1) % m_iBufferSize;
  --m_iBufferCount;

  if (m_iBufferCount == 0) {
    // reset start index to beginning when buffer is empty
    m_iBeginIndex = 0;
    m_iEndIndex = 0;
  }

  return result;
}

// push_back() : adds a new entry value to back of buffer; 
//               when buffer is full, this overwrites oldest entry
template <typename Type>
void CircBuffer<Type>::push_back (const Type& tValue)
{

  // check for unsized buffer
  if ( m_iBufferSize == 0 ) {
    m_iBufferSize = m_iDefaultSize;
    std::cerr << "Warning: push_back onto unsized circular buffer; " << std::endl
              << "  setting to default capacity = "<< m_iBufferSize << std::endl;
    m_vtBuffer.resize(m_iBufferSize);
  }

  // load value at 'end' index
  m_vtBuffer[m_iEndIndex] = tValue;  // when full, this overwrites at 'begin' index

  // update 'end' index, wrapping as needed
  m_iEndIndex = (m_iEndIndex + 1) % m_iBufferSize;

  // appropriately update 'begin' index or buffer count
  if ( m_iBufferCount == m_iBufferSize )
    m_iBeginIndex = m_iEndIndex;  // if full, update 'begin' index
  else
    ++m_iBufferCount;

  // clear value for 'restoreState' capability
  m_iBufferSizeSnap = -1;
}


// push_back() : adds a vector of new entry values to back of buffer; 
//               when buffer is full, this overwrites oldest entries
template <typename Type>
void CircBuffer<Type>::push_back (const std::vector<Type>& vtValues)
{

  // determine number of entries in vector being pushed
  int iCount = int(vtValues.size());

  // check for unsized buffer
  if ( m_iBufferSize == 0 ) {
    m_iBufferSize = m_iDefaultSize;
    std::cerr << "Warning: push_back onto unsized circular buffer; " << std::endl;
    if ( iCount > m_iBufferSize ) {
      m_iBufferSize = iCount;
      std::cerr << "  setting to pushed vector size = "<< m_iBufferSize << std::endl;
    }
    else
      std::cerr << "  setting to default capacity = "<< m_iBufferSize << std::endl;
    m_vtBuffer.resize(m_iBufferSize);
  }

  if ( iCount > m_iBufferSize ) {
    std::cerr << "Error: 'push_back' of vector exceeds capacity of circular buffer" << std::endl;
    return;
  }

  // loop over vector
  for (int ii=0; ii<iCount; ++ii) {
    // load each vector value at 'end' index
    m_vtBuffer[m_iEndIndex] = vtValues[ii];  // when full, this overwrites at 'begin' index
    // update 'end' index, wrapping as needed
    m_iEndIndex = (m_iEndIndex + 1) % m_iBufferSize;
    // update data counter as needed
    if ( m_iBufferCount < m_iBufferSize ) ++m_iBufferCount;
  }
  // when full, set the 'begin' index to the 'end' index
  if ( m_iBufferCount == m_iBufferSize )
    m_iBeginIndex = m_iEndIndex;

  // clear value for 'restoreState' capability
  m_iBufferSizeSnap = -1;
}

// Because the 'pop' operation here is non-destructive, it is possible
//  to return to a previous CircBuffer state, provided that no now
//  new data is added to the buffer, or the buffer resized.

// --save the current state of the CircBuffer
template <typename Type>
void CircBuffer<Type>::saveState ()
{
  // save current buffer sizes and indices
  m_iBufferSizeSnap  = m_iBufferSize; 
  m_iBufferCountSnap = m_iBufferCount;
  m_iBeginIndexSnap  = m_iBeginIndex; 
  m_iEndIndexSnap    = m_iEndIndex;   
}

// --restore a previous state of the CircBuffer, 
//   provided no data additions/buffer resizes have occurred
template <typename Type>
void CircBuffer<Type>::restoreState ()
{

  // check that restoration is possible
  if ( m_iBufferSize != m_iBufferSizeSnap ) {
    std::cerr << " CircBuffer modified - cannot restoreState " << std::endl;
    return;
  }

  // restore buffer sizes and indices
  m_iBufferCount = m_iBufferCountSnap;
  m_iBeginIndex  = m_iBeginIndexSnap;
  m_iEndIndex    = m_iEndIndexSnap;
}
#endif
