#include "gtest/gtest.h"
#include "tests.h"

namespace
{
  TEST(FreeFEM, NormalFunction)
  {
    EXPECT_TRUE(check_ff_exists());
  }

  TEST(FreeFEM, ParallelFunction)
  {
    EXPECT_TRUE(check_ff_parallel_exists(1));
    EXPECT_TRUE(check_ff_parallel_exists(3));
  }
}
