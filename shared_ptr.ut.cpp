#include <gtest/gtest.h>

#include "shared_ptr.hpp"

constexpr int value = 42;

struct shared_ptr_tests_fixture : public ::testing::Test {
    shared_ptr<int> ptr_test{new int{value}};
};

TEST(shared_ptr_tests, shouldCreateDefaultNullptr) {
    // Given
    int expected_cout = 0;
    
    // When
    shared_ptr<int> ptr{};

    // Then
    ASSERT_FALSE(ptr);
    ASSERT_EQ(ptr.get(), nullptr);
    ASSERT_EQ(ptr.operator->(), nullptr);
    ASSERT_EQ(ptr.use_count(), expected_cout);
}

TEST(shared_ptr_tests, shouldCreatePtrWithInt) {
    // Given
    int expected_count = 1;

    // When
    shared_ptr<int> ptr(new int(value));

    // Then
    ASSERT_TRUE(ptr);
    ASSERT_EQ(*ptr, value);
    ASSERT_EQ(ptr.use_count(), expected_count);
}

TEST_F(shared_ptr_tests_fixture, shouldCreatePtrFromCopyCtor) {
    // Given
    int expected_count = 1;
    ASSERT_EQ(ptr_test.use_count(), expected_count);

    // When
    auto ptr(ptr_test);
    expected_count++;

    // Then
    ASSERT_TRUE(ptr);
    ASSERT_EQ(*ptr, *ptr_test);
    ASSERT_EQ(ptr.get(), ptr_test.get());
    ASSERT_EQ(ptr.use_count(), expected_count);
}

TEST_F(shared_ptr_tests_fixture, shouldCreatePtrFromCopyOperator) {
    // Given
    int expected_count = 1;
    ASSERT_EQ(ptr_test.use_count(), expected_count);

    // When
    auto ptr = ptr_test;
    expected_count++;

    // Then
    ASSERT_TRUE(ptr);
    ASSERT_EQ(*ptr, *ptr_test);
    ASSERT_EQ(ptr.get(), ptr_test.get());
    ASSERT_EQ(ptr_test.use_count(), expected_count);
}

TEST_F(shared_ptr_tests_fixture, shouldCreatePtrFromMoveCtor) {
    // Given
    int expected_count = 1;
    ASSERT_EQ(ptr_test.use_count(), expected_count);

    // When
    auto ptr(std::move(ptr_test));

    // Then
    ASSERT_TRUE(ptr);
    ASSERT_EQ(*ptr, value);
    ASSERT_FALSE(ptr_test);
    ASSERT_EQ(ptr_test.get(), nullptr);
    ASSERT_EQ(ptr.use_count(), expected_count);
}

TEST_F(shared_ptr_tests_fixture, shouldCreatePtrFromMoveOperator) {
    // Given
    int expected_count = 1;
    ASSERT_EQ(ptr_test.use_count(), expected_count);

    // When
    auto ptr = std::move(ptr_test);

    // Then
    ASSERT_TRUE(ptr);
    ASSERT_EQ(*ptr, value);
    ASSERT_FALSE(ptr_test);
    ASSERT_EQ(ptr_test.get(), nullptr);
    ASSERT_EQ(ptr.use_count(), expected_count);
}

TEST_F(shared_ptr_tests_fixture, shouldSwap) {
    // Given
    shared_ptr<int> ptr(new int(-value));

    // When
    ptr.swap(ptr_test);

    // Then
    ASSERT_EQ(*ptr_test, -value);
    ASSERT_EQ(*ptr, value);
}

TEST_F(shared_ptr_tests_fixture, shouldReset) {
    // Given
    int expected_count = 2;
    auto ptr(ptr_test);
    ASSERT_EQ(ptr_test.use_count(), expected_count);

    // When
    ptr_test.reset(new int(-value));
    expected_count = 1;

    // Then
    ASSERT_EQ(*ptr_test, -value);
    ASSERT_EQ(ptr_test.use_count(), expected_count);
}
