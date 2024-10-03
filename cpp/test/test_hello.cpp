#include <gtest/gtest.h>

class HelloTest : public ::testing::Test {
};

TEST_F(HelloTest, test_hello) {
    EXPECT_EQ(1, 1);
}
