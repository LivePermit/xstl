#include "xstl_allocator.h"
#include <type_traits>

namespace xstl {

template <class T>
class vector {
public:
    using value_type = T;
    using allocator_type = xstl::allocator<T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T &;
    using const_reference = const T &;
    using pointer = T *;
    using const_pointer = const t *;
    using iterator = T *;
    using const_iterator = const T *;
    using reverse_iterator = std::reverse_iterator<T *>;
    using const_reverse_iterator = std::reverse_iterator<const T *>;

private:
public:
    vector() noexcept = default;

    vector(size_type count) { construct_elements(count, value_type()); }

    vector(size_type count, const value_type &value) { construct_elements(count, value); }

    template <class InputIt>
    vector(InputIt first, InputIt last)
    {
        if constexpr (std::is_integral<InputIt>::value) {
            construct_elements(static_cast<size_type>(first), static_cast<value_type>(last));
        } else {
            construct_from_others(first, last);
        }
    }

    vector(const vector &other) { construct_from_others(other.cbegin(), other.cend()); }

    vector(vector &&other) {}

    vector(std::initializer_list<T> ilist) { assign(ilist.cbegin(), ilist.cend()); }

    template <class InputIt>
    void assign(InputIt first, InputIt last)
    {
        clear_elements();
        construct_from_others(first, last);
    }

    void assign(std::initializer_list<T> ilist) { assign(ilist.cbegin(), ilist.cend()); }

    void assign(size_type count, const value_type &value)
    {
        clear_elements();
        construct_elements(count, value);
    }

    ~vector() { clear_elements(); }

    allocator_type get_allocator() const noexcept { return alloc_; }

    iterator begin() const noexcept { return begin_; }
    const_iterator begin() const noexcept { return begin_; };
    const_iterator cbegin() const noexcept { return begin_; }

    iterator end() const noexcept { return end_; }
    const_iterator end() const noexcept { return end_; }
    const_iterator cend() const noexcept { return end_; }

    reverse_iterator rbegin() const noexcept { return reverse_iterator(end_); }
    const_reverse_iterator rbegin() const noexcept { return reverse_iterator(end_); }
    const_reverse_iterator crbegin() const noexcept { return reverse_iterator(end_); }

    reverse_iterator rend() const noexcept { return reverse_iterator(begin_); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin_); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin_); }

    bool empty() const noexcept { return begin_ == nullptr; }
    size_type size() const noexcept { return end_ - begin_; }
    size_type capacity() const noexcept { return capacity_ - begin_; }
    void reserve(size_type new_cap) {}
    void shrink_to_fit() {}

    reference at(size_type pos)
    {
        if (pos > size()) {
            throw std::out_of_range("vector::at() - the specify index is out of bound");
        }
        return *(begin_ + pos);
    }
    const_reference at(size_type pos) const { return const_cast<vector *>(this)->at(); }

private:
    void construct_elements(size_type count, const value_type &value)
    {
        begin_ = alloc_.allocate(count);
        std::uninitialized_fill_n(begin_, count, value);
        end_ = begin_ + count;
        capacity_ = end_;
    }

    void construct_from_others(const_iterator first, const_iterator last)
    {
        auto count = last - first;
        begin_ = alloc_.allocate(count);
        std::uninitialized_copy(first, last, begin_);
        end_ = begin_ + count;
        capacity_ = end_;
    }

    void clear_elements()
    {
        if (!empty()) {
            destroy_elements(begin(), end());
            begin_ = nullptr;
            end_ = nullptr;
            capacity_ = nullptr;
        }
    }

    void destroy_elements(iterator begin, iterator end) noexcept
    {
        for (auto it = begin; it != end; it++) {
            alloc_.destroy(it);
        }
    }

private:
    pointer begin_ = nullptr;
    pointer end_ = nullptr;
    pointer capacity_ = nullptr;

    allocator_type alloc_;
};
} // namespace xstl
