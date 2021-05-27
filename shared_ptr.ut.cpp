#include <gtest/gtest.h>

#include "shared_ptr.hpp"

struct shared_ptr_test : public ::testing::Test {
    // shared_ptr<int> ptr = new int{10};
};

TEST_F(shared_ptr_test, shouldGetPtr) {
    ASSERT_EQ(1, 2);
}
