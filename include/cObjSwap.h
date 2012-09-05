// Copyright �2012 Black Sphere Studios
// For conditions of distribution and use, see copyright notice in "bss_util.h"

#ifndef __C_OBJSWAP_H__BSS__
#define __C_OBJSWAP_H__BSS__

#include "bss_deprecated.h"
#include "bss_dlldef.h"
#include "bss_compare.h"
#include <stdarg.h>
#include <string.h>

namespace bss_util {
  template<class T> // Specializes to case-sensitive string matching
  struct OBJSWAPFUNC_ALL { BSS_FORCEINLINE static char Comp(const T& left, const T& right) { return CompT_EQ<T>(left,right); } };
  template<> struct OBJSWAPFUNC_ALL<const char*> { BSS_FORCEINLINE static char Comp(const char* l, const char* r) { return !strcmp(l,r); } };
  template<> struct OBJSWAPFUNC_ALL<const wchar_t*> { BSS_FORCEINLINE static char Comp(const wchar_t* l, const wchar_t* r) { return !wcscmp(l,r); } };
  
  template<class T> // Specializes to case-insensitive string matching
  struct OBJSWAPFUNC_INS { BSS_FORCEINLINE static char Comp(const T& left, const T& right) { return CompT_EQ<T>(left,right); } };
  template<> struct OBJSWAPFUNC_INS<const char*> { BSS_FORCEINLINE static char Comp(const char* l, const char* r) { return !STRICMP(l,r); } };
  template<> struct OBJSWAPFUNC_INS<const wchar_t*> { BSS_FORCEINLINE static char Comp(const wchar_t* l, const wchar_t* r) { return !WCSICMP(l,r); } };
  
  // Generalized solution to allow dynamic switch statements
  template<class T, class SWAP=OBJSWAPFUNC_ALL<T>>
  class BSS_COMPILER_DLLEXPORT cObjSwap : protected SWAP
  {
  public:
	  inline explicit cObjSwap(const T& src, int num, ...)
	  {
		  va_list vl;
		  va_start(vl, num);
		  _result = _compobj(src, num, vl);
		  va_end(vl);
	  }
    inline explicit cObjSwap(const T& src, T* tarray, int num) : _result(CompareObjectsArray(src,num,tarray)) {}
    template<int N>
    inline explicit cObjSwap(const T& src, const T (&tarray)[N]) : _result(CompareObjectsArray<N>(src,tarray)) {}

    inline BSS_FORCEINLINE operator int() const { return _result; }

	  inline static int CompareObjects(const T& src, int num, ...)
	  {
		  va_list vl;
		  va_start(vl, num);
		  int retval = _compobj(src, num, vl);
		  va_end(vl);
		  return retval;
	  }

    template<int N>
    inline static BSS_FORCEINLINE int CompareObjectsArray(const T& src, const T (&tarray)[N])
    {
      return CompareObjectsArray(src,N,tarray);
    }

    inline static BSS_FORCEINLINE int CompareObjectsArray(const T& src, int num, const T* tarray)
    {
		  for(int i = 0; i < num; ++i)
			  if(SWAP::Comp(tarray[i], src))
				  return i;
		  return -1;
    }

  private:
	  inline static int _compobj(const T& src, int num, va_list args)
	  {
		  for(int i = 0; i < num; ++i)
			  if(SWAP::Comp((va_arg(args, const T)), src))
				  return i;
		  return -1;
	  }

	  int _result;
  };

  typedef cObjSwap<const char*> STRSWAP;
  typedef cObjSwap<const wchar_t*> WCSSWAP;
  typedef cObjSwap<const void*> PSWAP;
}

#endif