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

  TEST(BioDeg, Write)
  {
    EXPECT_TRUE(check_biodeg_write(2));
    EXPECT_TRUE(check_biodeg_write(4));
  }

  TEST(BioDeg, SimulateDiffusion)
  {
    EXPECT_TRUE(check_biodeg_diffusion(2));
    EXPECT_TRUE(check_biodeg_diffusion(4));
  }
}
