#include <gtest/gtest.h>

#include "shared_ptr.hpp"

constexpr int value = 42;

struct example_obj {
    int val_;
    char sign_;
    bool flag_;

    example_obj(int val, char sign, bool flag)
        : val_(val), sign_(sign), flag_(flag) {}
};

struct shared_ptr_tests_fixture : public ::testing::Test {
    shared_ptr<int> ptr_test{new int{value}};
};

TEST(shared_ptr_tests, shouldCreateDefaultNullptr) {
    // Given
    int expected_count = 0;

    // When
    shared_ptr<int> ptr{};

    // Then
    ASSERT_FALSE(ptr);
    ASSERT_EQ(ptr.get(), nullptr);
    ASSERT_EQ(ptr.operator->(), nullptr);
    ASSERT_EQ(ptr.use_count(), expected_count);
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

TEST_F(shared_ptr_tests_fixture, shouldModifyFromCopyOperator) {
    // Given
    int expected_count = 1;
    shared_ptr<int> ptr(new int(value * value));
    ASSERT_EQ(ptr.use_count(), expected_count);

    // When
    ptr_test = ptr;
    auto ptr_other = ptr_test;
    expected_count += 2;

    // Then
    ASSERT_TRUE(ptr);
    ASSERT_TRUE(ptr_test);
    ASSERT_TRUE(ptr_other);
    ASSERT_EQ(*ptr_other, value * value);
    ASSERT_EQ(ptr_other.use_count(), expected_count);
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

TEST_F(shared_ptr_tests_fixture, shouldResetToGivenValue) {
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

TEST_F(shared_ptr_tests_fixture, shouldResetToNullptr) {
    // Given
    int expected_count = 1;
    ASSERT_EQ(ptr_test.use_count(), expected_count);

    // When
    ptr_test.reset();
    expected_count = 0;

    // Then
    ASSERT_FALSE(ptr_test);
    ASSERT_EQ(ptr_test.get(), nullptr);
    ASSERT_EQ(ptr_test.use_count(), expected_count);
}

TEST(make_shared_tests, shouldCreatePtrFromTrivialType) {
    // Given
    int expected_count = 1;

    // When
    auto ptr = make_shared<int>(value);

    // Then
    ASSERT_TRUE(ptr);
    ASSERT_EQ(*ptr, value);
    ASSERT_EQ(ptr.use_count(), expected_count);
}

TEST(make_shared_tests, shouldCreatePtrFromCustomType) {
    // Given
    int expected_count = 1;
    char example_char = 'z';
    bool example_bool = true;

    // When
    auto ptr = make_shared<example_obj>(value, example_char, example_bool);

    // Then
    ASSERT_TRUE(ptr);
    ASSERT_EQ(ptr.use_count(), expected_count);
    ASSERT_EQ(ptr->val_, value);
    ASSERT_EQ(ptr->sign_, example_char);
    ASSERT_EQ(ptr->flag_, example_bool);
}
