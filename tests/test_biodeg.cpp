#include "gtest/gtest.h"

namespace
{
  TEST(SampleTest, Check)
  {
    EXPECT_EQ(1, 1);
    EXPECT_GT(2, 1);
    EXPECT_TRUE(true);
  }
}
