#include <gtest/gtest.h>

#include "shared_ptr.hpp"

constexpr int value = 42;
struct custom_obj {
    int first = 1;

    int getFirst() { return first; }
};
struct shared_ptr_tests_fixture : public ::testing::Test {
    // Given
    custom_obj obj;
    shared_ptr<int> ptr_test{new int{value}};
    shared_ptr<custom_obj> ptr_test_obj{new custom_obj{}};
};

TEST(shared_ptr_test, shouldCreateDeafultNullptr) {
    shared_ptr<int> ptr;
    ASSERT_EQ(ptr.get(), nullptr);
    ASSERT_EQ(ptr.use_count(), 1);
}

TEST(shared_ptr_test, shouldAllocateFromRawPtr) {
    auto ptr = shared_ptr<int>(new int(value));
    ASSERT_EQ(*ptr, value);
}
    

TEST_F(shared_ptr_tests_fixture, shouldDerefrencePtr) {
    ASSERT_EQ(value, *ptr_test);
}

TEST_F(shared_ptr_tests_fixture, shouldArrowOperatorWorks) {
    ASSERT_EQ(obj.getFirst(), ptr_test_obj->getFirst());
}


TEST_F(shared_ptr_tests_fixture, shouldAllocateFromCopyCtor) {
    auto ptr(ptr_test);
    ASSERT_EQ(*ptr, *ptr_test);
}

TEST_F(shared_ptr_tests_fixture, shouldAllocateFromAssignOperator) {
    auto ptr = ptr_test;
    ASSERT_EQ(*ptr, *ptr_test);
}

TEST(shared_ptr_test, shouldAllocateFromMoveCtor) {
    auto ptr = shared_ptr<int>(new int(value));
    auto ptr2(std::move(ptr));
    ASSERT_EQ(ptr.get(), nullptr);
    ASSERT_EQ(*ptr2, value);
}

TEST(shared_ptr_test, shouldAllocateFromMoveOperator) {
    auto ptr = shared_ptr<int>(new int(value));
    auto ptr2 = shared_ptr<int>(new int(-value));
    ptr2 = std::move(ptr);
    ASSERT_EQ(ptr.get(), nullptr);
    ASSERT_EQ(*ptr2, value);
}
