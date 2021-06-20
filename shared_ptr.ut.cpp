#include <gtest/gtest.h>

#include "shared_ptr.hpp"

constexpr int value = 42;
struct custom_obj {
    int first = 1;
    int last = 99;

    int getFirst() { return first; }
    int getLast() { return last; }
};
struct shared_ptr_tests_fixture : public ::testing::Test {
    // Given
    custom_obj obj;
    shared_ptr<int> ptr_test{new int{value}};
    shared_ptr<custom_obj> ptr_test_obj{new custom_obj{}};
};

TEST_F(shared_ptr_tests_fixture, shouldDerefrencePtr) {
    ASSERT_EQ(value, *ptr_test);
}

TEST_F(shared_ptr_tests_fixture, shouldArrowOperatorWorks) {
    ASSERT_EQ(obj.getFirst(), ptr_test_obj->getFirst());
}
