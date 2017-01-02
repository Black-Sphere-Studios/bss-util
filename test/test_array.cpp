// Copyright �2017 Black Sphere Studios
// For conditions of distribution and use, see copyright notice in "bss_util.h"

#include "test.h"
#include "cArray.h"

using namespace bss_util;

TESTDEF::RETPAIR test_ARRAY()
{
  BEGINTEST;

  cArray<int> a(5);
  TEST(a.Capacity() == 5);
  a.Insert(5, 2);
  TEST(a.Capacity() == 6);
  TEST(a[2] == 5);
  a.Remove(1);
  TEST(a[1] == 5);
  a.SetCapacity(10);
  TEST(a[1] == 5);
  TEST(a.Capacity() == 10);

  {
    cArray<int> e(0);
    cArray<int> b(e);
    b = e;
    e.Insert(5, 0);
    e.Insert(4, 0);
    e.Insert(2, 0);
    e.Insert(3, 1);
    TEST(e.Capacity() == 4);
    int sol[] = { 2,3,4,5 };
    TESTARRAY(sol, return e[i] == sol[i];);
    cArray<int> c(0);
    c = e;
    TESTARRAY(sol, return c[i] == sol[i];);
    cArray<int> d(0);
    e = d;
    TEST(!e.Capacity());
    e += d;
    TEST(!e.Capacity());
    e = c;
    TESTARRAY(sol, return e[i] == sol[i];);
    e += d;
    TESTARRAY(sol, return e[i] == sol[i];);
    d += c;
    TESTARRAY(sol, return d[i] == sol[i];);
    e += c;
    int sol2[] = { 2,3,4,5,2,3,4,5 };
    TESTARRAY(sol, return e[i] == sol[i];);
  }

  auto f = [](cArray<DEBUG_CDT<true>, uint32_t, CARRAY_SAFE>& arr)->bool {
    for(uint32_t i = 0; i < arr.Capacity(); ++i)
      if(arr[i]._index != i)
        return false;
    return true;
  };
  auto f2 = [](cArray<DEBUG_CDT<true>, uint32_t, CARRAY_SAFE>& arr, uint32_t s) { for(uint32_t i = s; i < arr.Capacity(); ++i) arr[i]._index = i; };

  assert(!DEBUG_CDT_SAFE::Tracker.Length());
  {
    DEBUG_CDT<true>::count = 0;
    cArray<DEBUG_CDT<true>, uint32_t, CARRAY_SAFE> b(10);
    f2(b, 0);
    b.Remove(5);
    for(uint32_t i = 0; i < 5; ++i) TEST(b[i]._index == i);
    for(uint32_t i = 5; i < b.Capacity(); ++i) TEST(b[i]._index == (i + 1));
    TEST(b.Capacity() == 9);
    TEST(DEBUG_CDT<true>::count == 9);
    f2(b, 0);
    b.SetCapacity(19);
    f2(b, 9);
    TEST(f(b));
    TEST(DEBUG_CDT<true>::count == 19);
    TEST(b.Capacity() == 19);
    cArray<DEBUG_CDT<true>, uint32_t, CARRAY_SAFE> c(b);
    TEST(f(c));
    TEST(DEBUG_CDT<true>::count == 38);
    b += c;
    for(uint32_t i = 0; i < 19; ++i) TEST(b[i]._index == i);
    for(uint32_t i = 19; i < 38; ++i) TEST(b[i]._index == (i - 19));
    TEST(DEBUG_CDT<true>::count == 57);
    b + c;
    f2(b, 0);
    b.Insert(DEBUG_CDT<true>(), 5);
    for(uint32_t i = 0; i < 5; ++i) TEST(b[i]._index == i);
    for(uint32_t i = 6; i < b.Capacity(); ++i) TEST(b[i]._index == (i - 1));
    TEST(DEBUG_CDT<true>::count == 58);
    b.Insert(DEBUG_CDT<true>(), b.Capacity());
    TEST(DEBUG_CDT<true>::count == 59);
  }
  TEST(!DEBUG_CDT<true>::count);
  TEST(!DEBUG_CDT_SAFE::Tracker.Length());

  auto f3 = [](cArray<DEBUG_CDT<false>, uint32_t, CARRAY_CONSTRUCT>& arr)->bool {
    for(uint32_t i = 0; i < arr.Capacity(); ++i)
      if(arr[i]._index != i)
        return false;
    return true;
  };
  auto f4 = [](cArray<DEBUG_CDT<false>, uint32_t, CARRAY_CONSTRUCT>& arr, uint32_t s) { for(uint32_t i = s; i < arr.Capacity(); ++i) arr[i]._index = i; };
  {
    DEBUG_CDT<false>::count = 0;
    cArray<DEBUG_CDT<false>, uint32_t, CARRAY_CONSTRUCT> b(10);
    f4(b, 0);
    b.Remove(5);
    for(uint32_t i = 0; i < 5; ++i) TEST(b[i]._index == i);
    for(uint32_t i = 5; i < b.Capacity(); ++i) TEST(b[i]._index == (i + 1));
    TEST(b.Capacity() == 9);
    TEST(DEBUG_CDT<false>::count == 9);
    f4(b, 0);
    b.SetCapacity(19);
    f4(b, 9);
    TEST(f3(b));
    TEST(DEBUG_CDT<false>::count == 19);
    TEST(b.Capacity() == 19);
    cArray<DEBUG_CDT<false>, uint32_t, CARRAY_CONSTRUCT> c(b);
    TEST(f3(c));
    TEST(DEBUG_CDT<false>::count == 38);
    b += c;
    for(uint32_t i = 0; i < 19; ++i) TEST(b[i]._index == i);
    for(uint32_t i = 19; i < 38; ++i) TEST(b[i]._index == (i - 19));
    TEST(DEBUG_CDT<false>::count == 57);
    b + c;
    f4(b, 0);
    b.Insert(DEBUG_CDT<false>(), 5);
    for(uint32_t i = 0; i < 5; ++i) TEST(b[i]._index == i);
    for(uint32_t i = 6; i < b.Capacity(); ++i) TEST(b[i]._index == (i - 1));
    TEST(DEBUG_CDT<false>::count == 58);
    b.Insert(DEBUG_CDT<false>(), b.Capacity());
    TEST(DEBUG_CDT<false>::count == 59);
  }
  TEST(!DEBUG_CDT<false>::count);
  TEST(!DEBUG_CDT_SAFE::Tracker.Length());

  ENDTEST;
}
