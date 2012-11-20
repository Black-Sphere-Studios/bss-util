// Copyright �2012 Black Sphere Studios
// For conditions of distribution and use, see copyright notice in "bss_util.h"

#ifndef __C_BITFIELD_H__BSS__ //These are used in case this header file is used by two different projects dependent on each other, resulting in duplicates which cannot be differentiated by #pragma once
#define __C_BITFIELD_H__BSS__

#include "bss_defines.h"

namespace bss_util
{
  // Generic implementation of using an integral type's component bits to store flags.
  template<class T=unsigned int>
  class BSS_COMPILER_DLLEXPORT cBitField
  {
  public:
    // Initializes the bitfield with the given flag values, if any
    inline cBitField(T init=0) : _bitfield(init) {}
    // Destructor generated by compiler
    // Sets the entire bitfield to the given value
    inline BSS_FORCEINLINE void BSS_FASTCALL SetBits(T bitfield) { _bitfield=bitfield; }
    // Gets the entire bitfield
    inline BSS_FORCEINLINE T GetBits() const { return _bitfield; }
    // Adds one or more bit flags to the bitfield (use the OR operator to combine flags, e.g. flag1|flag2)
    inline BSS_FORCEINLINE void BSS_FASTCALL AddBit(T bits) { _bitfield |= bits; }
    // Removes one or more bit flags from the bitfield
    inline BSS_FORCEINLINE void BSS_FASTCALL RemoveBit(T bits) { _bitfield &= ~bits; }
    // Gets one or more bit flags from the bitfield (WARNING: This will return true if ANY of the supplied flags are true)
    inline BSS_FORCEINLINE bool BSS_FASTCALL GetBit(T bits) const { return (_bitfield & bits)!=0; }

    inline bool operator==(const cBitField& right) const { return _bitfield==right.GetBits(); }
    inline bool operator!=(const cBitField& right) const { return _bitfield!=right.GetBits(); }

  protected:
    T _bitfield;
  };

  // Extension of the bit field to use operator[] for read-only access. Not implemented in cBitField because it causes ambiguity problems
  template<class T=unsigned int>
  class BSS_COMPILER_DLLEXPORT cBitWrap : public cBitField<T>
  {
  public:
    inline cBitWrap(T init=0) : cBitField(init) {}
    
    inline BSS_FORCEINLINE bool operator[](T bit) const { return GetBit(bit); } 
    inline bool operator==(const cBitField& right) const { return _bitfield==right.GetBits(); }
    inline bool operator!=(const cBitField& right) const { return _bitfield!=right.GetBits(); }
    inline T operator ~() const { return ~_bitfield; }
    inline T operator &(T r) const { return _bitfield&r; }
    inline T operator |(T r) const { return _bitfield|r; }
    inline T operator ^(T r) const { return _bitfield^r; }
    inline T operator <<(T r) const { return _bitfield<<r; }
    inline T operator >>(T r) const { return _bitfield>>r; }
  };
}

#endif