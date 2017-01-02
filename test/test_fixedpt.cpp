// Copyright �2017 Black Sphere Studios
// For conditions of distribution and use, see copyright notice in "bss_util.h"

#include "test.h"
#include "FixedPt.h"

using namespace bss_util;

TESTDEF::RETPAIR test_bss_FIXEDPT()
{
  BEGINTEST;

  TEST(((double)Fixed<13, int64_t>(0.001).Reciprocal() == 1024.0));
  TEST(((double)Fixed<13>(2.0).Reciprocal() == 0.5));
  {
    Fixed<13> fp(23563.2739);
    float res = fp;
    fp += 27.9;
    res += 27.9f;
    TEST(fcompare(res, fp));
    res = fp;
    fp -= 8327.9398437;
    res -= 8327.9398437f;
    TEST(fcompare(res, fp));
    fp *= 6.847399;
    res *= 6.847399f;
    TEST(fcompare(res, fp, 215)); // We start approaching the edge of our fixed point range here so things predictably get out of whack
    res = fp;
    fp /= 748.9272;
    res /= 748.9272f;
    TEST(fcompare(res, fp, 6));
  }

  {
    Fixed<23, int64_t> fp(23563.2739);
    float res = fp;
    fp += 27.9;
    res += 27.9f;
    TEST(fcompare(res, fp));
    res = fp;
    fp -= 8327.9398437;
    res -= 8327.9398437f;
    TEST(fcompare(res, fp));
    fp *= 6.847399;
    res *= 6.847399f;
    TEST(fcompare(res, fp, 215)); // We start approaching the edge of our fixed point range here so things predictably get out of whack
    res = fp;
    fp /= 748.9272;
    res /= 748.9272f;
    TEST(fcompare(res, fp, 6));
  }
  const double d1 = 1234.01004;
  const double d2 = -140.005;
  Fixed<43, int64_t> fp23(d1);
  Fixed<31, int64_t> fp13(d2);
  TEST(fcompare(-fp23, -d1, 40LL));
  TEST(fcompare(-fp13, -d2, 4000LL));
  double test = (fp23 + fp13) / (fp23 - (fp13 * fp23) - fp13) * fp13 - fp23;
  TEST(fcompare(test, (d1 + d2) / (d1 - (d2 * d1) - d2) * d2 - d1, 20000000000LL));

  ENDTEST;
}