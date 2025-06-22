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

/**
 * @brief RAII a heap allocated pointer
 * @tparam type to allocate
 */
template<typename T>
class HeapGuard {
    T* ptr_;

public:
    using value_type = T;

    template<typename... Args>
        requires (((!::pltxt2htm::details::is_heap_guard<Args>) && ...) && ::std::constructible_from<T, Args...>)
    constexpr HeapGuard(Args&&... args) noexcept {
        if consteval {
            this->ptr_ = new T(::std::forward<Args>(args)...);
        } else {
            this->ptr_ = reinterpret_cast<T*>(::std::malloc(sizeof(T)));
            if (this->ptr_ == nullptr) [[unlikely]] {
                // bad alloc should never be an exception or err_code
                ::exception::terminate();
            }
            ::std::construct_at(this->ptr_, ::std::forward<Args>(args)...);
        }
    }

#if 1
    constexpr HeapGuard(HeapGuard<T> const& other) noexcept {
        // ::std::malloc will implicitly start lifetime of ptr
        // Therefore, should not call ::std::start_lifetime_as
        this->ptr_ = reinterpret_cast<T*>(::std::malloc(sizeof(T)));
        if (this->ptr_ == nullptr) [[unlikely]] {
            // bad alloc should never be an exception or err_code
            ::exception::terminate();
        }
        ::std::memcpy(this->ptr_, other.ptr_, sizeof(T));
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
        this->ptr_ = other.release();
    }

    constexpr HeapGuard& operator=([[maybe_unused]] this HeapGuard<T>& self,
                                   [[maybe_unused]] HeapGuard<T> const& other) noexcept {
#if 1
        static_assert(
            false, "Despite copy a HeapGuard is safe, but the HeapGuard's behavior is more similar to std::unique_ptr");
#else
        ::std::destroy_at(self.ptr_);
        ::std::free(self.ptr_);
        ::std::memcpy(self.ptr_, other.ptr_, sizeof(T));
        return self;
#endif
    }

    constexpr HeapGuard& operator=(this HeapGuard<T>& self, HeapGuard<T>&& other) noexcept {
        self.swap(self, other);
        return self;
    }

    constexpr ~HeapGuard() noexcept {
        if (ptr_ != nullptr) {
            ::std::destroy_at(ptr_);
            ::std::free(ptr_);
        }
    }

    constexpr T* operator->(this HeapGuard<T> const& self) noexcept {
        return self.ptr_;
    }

    constexpr T const* release_imul(this HeapGuard<T> const& self) noexcept {
        return self.ptr_;
    }

    [[nodiscard]]
    constexpr T* release(this auto&& self) noexcept {
        T* ptr = self.ptr_;
        self.ptr_ = nullptr;
        return ptr;
    }

    constexpr void swap(this HeapGuard<T>& self, HeapGuard<T>& other) noexcept {
        T* tmp = self.ptr_;
        self.ptr_ = other.ptr_;
        other.ptr_ = tmp;
    }
};

} // namespace pltxt2htm::details
