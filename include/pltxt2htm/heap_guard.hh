#pragma once

#include <cstdlib>
#include <cstring>
#include <memory>
#include <utility>
#include <concepts>
#include <exception/exception.hh>

namespace pltxt2htm::details {

template<typename T>
class HeapGuard;

/**
 * @brief This internal var is a part of `concept is_heap_gurad`
 */
template<typename T>
constexpr bool is_heap_guard_ = false;

/**
 * @brief This internal var is a part of `concept is_heap_gurad`
 */
template<typename T>
constexpr bool is_heap_guard_<HeapGuard<T>> = true;

/**
 * @brief Whether the type is a heap guard
 * @tparam type to check
 */
template<typename T>
concept is_heap_guard = is_heap_guard_<::std::remove_cvref_t<T>>;

template<typename T>
class HeapGuard {
    T* ptr_;

public:
    using value_type = T;

    template<typename... Args>
        requires ((!::pltxt2htm::details::is_heap_guard<Args>) && ...)
    constexpr HeapGuard(Args&&... args) noexcept {
        T* ptr = reinterpret_cast<T*>(::std::malloc(sizeof(T)));
        if (ptr == nullptr) [[unlikely]] {
            // bad alloc should never be an exception or err_code
            ::exception::terminate();
        }
        ptr_ = ::std::construct_at(ptr, ::std::forward<Args>(args)...);
    }

#if 1
    constexpr HeapGuard(HeapGuard<T> const& other) noexcept {
        T* ptr = reinterpret_cast<T*>(::std::malloc(sizeof(T)));
        if (ptr == nullptr) [[unlikely]] {
            // bad alloc should never be an exception or err_code
            ::exception::terminate();
        }
        ::std::memcpy(ptr, other.ptr_, sizeof(T));
        ptr_ = ptr;
        // Cause libc++, libstdc++ and STL do not support ::std::start_lifetime_as
        // and ::std::malloc will implicitly start lifetime of ptr, I temporarily
        // comment out the next line of code.
        // ::std::start_lifetime_as<T>(ptr);
    }
#else
    constexpr HeapGuard(HeapGuard<T> const&) noexcept = delete
    #if __cpp_deleted_function >= 202403L
        ("Despite copy a HeapGuard is safe, but the HeapGuard's behavior is more similar to ::std::unique_ptr")
    #endif
        ;
#endif

    template<typename U>
        requires (::std::derived_from<U, T>)
    constexpr HeapGuard(HeapGuard<U>&& other) noexcept {
        ptr_ = other.release();
    }

#if 1
    constexpr HeapGuard& operator=(this HeapGuard<T>& self, HeapGuard<T> const& other) noexcept {
        ::std::destroy_at(self.ptr_);
        ::std::free(self.ptr_);
        ::std::memcpy(self.ptr_, other.ptr_, sizeof(T));
        return self;
    }
#else
    constexpr HeapGuard& operator=(this HeapGuard<T>& self, HeapGuard const& other) noexcept = delete
    #if __cpp_deleted_function >= 202403L
        ("Despite copy a HeapGuard is safe, but the HeapGuard's behavior is more similar to ::std::unique_ptr")
    #endif
        ;
#endif

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
