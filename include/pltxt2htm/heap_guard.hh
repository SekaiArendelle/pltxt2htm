#pragma once

#include <cstdlib>
#include <memory>
#include <utility>
#include <concepts>
#include <exception/exception.hh>

namespace pltxt2htm::details {

template<typename T>
class HeapGuard {
    T* ptr_;

public:
    using value_type = T;

    template<typename... Args>
    constexpr HeapGuard(Args&&... args) noexcept {
        T* ptr = reinterpret_cast<T*>(::std::malloc(sizeof(T)));
        if (ptr == nullptr) [[unlikely]] {
            // bad alloc should never be an exception or err_code
            ::exception::terminate();
        }
        ptr_ = ::std::construct_at(ptr, ::std::forward<Args>(args)...);
    }

    constexpr HeapGuard(HeapGuard const& other) = delete;

    template<typename U>
        requires (::std::derived_from<U, T>)
    constexpr HeapGuard(HeapGuard<U>&& other) noexcept {
        ptr_ = other.release();
    }

    constexpr HeapGuard& operator=(this HeapGuard<T>& self, HeapGuard const& other) = delete;

    constexpr HeapGuard& operator=(this HeapGuard<T>& self, HeapGuard<T>&& other) noexcept {
        ::std::destroy_at(self.ptr_);
        ::std::free(self.ptr_);
        self.ptr_ = other.release();
        return self;
    }

    constexpr ~HeapGuard() noexcept {
        ::std::destroy_at(ptr_);
        ::std::free(ptr_);
    }

    constexpr T* operator->(this HeapGuard<T> const& self) noexcept {
        return self.ptr_;
    }

    [[nodiscard]]
    constexpr T* release(this auto&& self) noexcept {
        T* ptr = self.ptr_;
        self.ptr_ = nullptr;
        return ptr;
    }
};

} // namespace pltxt2htm::details
