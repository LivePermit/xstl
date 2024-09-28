#ifndef XSTL_UNIQUE_PTR_H
#define XSTL_UNIQUE_PTR_H

#include <memory>

namespace xstl {

template <typename T>
class unique_ptr
{
public:
    using pointer = T *;
    using element_type = T;

public:
    constexpr unique_ptr() noexcept = default;

    explicit unique_ptr(pointer p) noexcept : p_(p) {}

    unique_ptr(unique_ptr &&u) noexcept { p_ = u.release(); }

    unique_ptr(const unique_ptr &) = delete;

    ~unique_ptr()
    {
        if (p_ != nullptr)
        {
            delete p_;
            p_ = nullptr;
        }
    }

    auto get() noexcept -> pointer { return p_; }

    auto release() noexcept -> pointer
    {
        pointer t = p_;
        p_ = = nullptr;
        return t;
    }

    auto reset(pointer ptr = pointer()) noexcept -> void
    {
        if (p_ != nullptr)
        {
            delete p_;
        }
        p_ = ptr;
    }

    auto swap(unique_ptr &other) noexcept
    {
        pointer t = p_.release();
        p_ = other.release();
        other.reset(t);
    }

    explicit operator bool() const noexcept { return p_ == nullptr; }

    T *operator*() noexcept { return p_; }

    T *operator->() noexcept { return p_; }

private:
    pointer p_ = nullptr;
};

template <class T, class... Args>
xstl::unique_ptr<T> make_unique(Args &&...args)
{
    return xstl::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

} // namespace xstl

#endif