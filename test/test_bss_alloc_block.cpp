// Copyright �2018 Black Sphere Studios
// For conditions of distribution and use, see copyright notice in "bss_util.h"

#include "test.h"
#include "test_alloc.h"
#include "bss-util/BlockAlloc.h"

using namespace bss;

TESTDEF::RETPAIR test_bss_ALLOC_BLOCK()
{
  BEGINTEST;
  TEST_ALLOC_FUZZER<BlockPolicy, size_t, 1, 10000>(__testret);
  ENDTEST;
}