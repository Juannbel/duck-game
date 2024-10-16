#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "common/foo.h"

using ::testing::HasSubstr;
using ::testing::ThrowsMessage;
using ::testing::AllOf;


namespace {
    TEST(FooTest, Check) {
        EXPECT_EQ(foo(4), (int)(4+5));
    }
}
