#include "xstl/xstl_unique_ptr.h"
#include <gtest/gtest.h>
#include <stddef.h>

class TestA
{
public:
    static auto get_construct() noexcept -> uint32_t { return construct_; }
    static auto get_delete() noexcept -> uint32_t { return delete_; }

    TestA() { construct_++; }
    ~TestA() { delete_++; }

private:
    static uint32_t construct_;
    static uint32_t delete_;
};

uint32_t TestA::construct_ = 0U;
uint32_t TestA::delete_ = 0U;

TEST(xstl_unique_ptr, case_aa)
{
    auto a = xstl::make_unique<TestA>();
    EXPECT_EQ(1U, TestA::get_construct());
    EXPECT_EQ(0U, TestA::get_delete());
}
