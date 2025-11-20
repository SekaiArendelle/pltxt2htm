#pragma once

#include <cstdlib>
#include <memory>
#include <utility>
#include <concepts>
#include <type_traits>
#include <exception/exception.hh>

namespace pltxt2htm {

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
 * @brief RAII a heap allocated pointer, similar to std::unique_ptr
 */
template<typename T>
class
#if __has_cpp_attribute(clang::trivial_abi)
    [[clang::trivial_abi]]
#endif
    HeapGuard {
    T* ptr_;

public:
    void (*deleter_)(T*) noexcept;
    using value_type = T;

    template<typename... Args>
        requires (((!::pltxt2htm::is_heap_guard<Args>) && ...) && ::std::constructible_from<T, Args...>)
    constexpr HeapGuard(Args&&... args) noexcept {
        this->deleter_ = [](T* self) static constexpr noexcept { self->~T(); };
        this->ptr_ = static_cast<T*>(::std::malloc(sizeof(T)));
        if (this->ptr_ == nullptr) [[unlikely]] {
            // bad alloc should never be an exception or err_code
            ::exception::terminate();
        }
        ::new (this->ptr_) T(::std::forward<Args>(args)...);
    }

    constexpr HeapGuard(::pltxt2htm::HeapGuard<T> const& other) noexcept
        requires (::std::is_copy_constructible_v<T>)
    {
        this->deleter_ = other.deleter_;
        // ::std::malloc will implicitly start lifetime of ptr
        // Therefore, should not call ::std::start_lifetime_as
        this->ptr_ = static_cast<T*>(::std::malloc(sizeof(T)));
        if (this->ptr_ == nullptr) [[unlikely]] {
            // bad alloc should never be an exception or err_code
            ::exception::terminate();
        }
        ::new (this->ptr_) T(*other.release_imul());
    }

    template<typename U>
        requires (::std::derived_from<U, T>)
    constexpr HeapGuard(::pltxt2htm::HeapGuard<U>&& other) noexcept
        : deleter_{reinterpret_cast<void (*)(T*) noexcept>(other.deleter_)} {
        this->ptr_ = other.release();
        other.deleter_ = nullptr;
    }

    constexpr auto operator=(::pltxt2htm::HeapGuard<T> const&) noexcept -> ::pltxt2htm::HeapGuard<T>& = delete
#if __cpp_deleted_function >= 202403L
        ("Despite copy a HeapGuard is safe, but the HeapGuard's behavior is more similar to std::unique_ptr")
#endif
        ;

    constexpr auto operator=(this ::pltxt2htm::HeapGuard<T>& self, ::pltxt2htm::HeapGuard<T>&& other) noexcept
        -> ::pltxt2htm::HeapGuard<T>& {
#if 0
        if (self.ptr_ == other.release_imul()) [[unlikely]] {
            ::exception::terminate();
        }
#endif
        self.swap(other);
        return self;
    }

    constexpr ~HeapGuard() noexcept {
        if (ptr_ != nullptr) {
            this->deleter_(this->ptr_);
            ::std::free(this->ptr_);
        }
    }

    constexpr T* operator->(this ::pltxt2htm::HeapGuard<T>& self) noexcept {
        return self.ptr_;
    }

    constexpr T const* operator->(this ::pltxt2htm::HeapGuard<T> const& self) noexcept {
        return self.ptr_;
    }

    constexpr auto operator==(this ::pltxt2htm::HeapGuard<T> const& self,
                              ::pltxt2htm::HeapGuard<T> const& other) noexcept(noexcept(*self.ptr_ == *other.ptr_)) {
        return *self.ptr_ == *other.ptr_;
    }

    [[nodiscard]]
    constexpr T const* release_imul(this ::pltxt2htm::HeapGuard<T> const& self) noexcept {
        return self.ptr_;
    }

    /**
     * @note The result is a borrowed reference.
     */
    [[nodiscard]]
    constexpr T* get_unsafe(this auto&& self) noexcept {
        return self.ptr_;
    }

    [[nodiscard]]
    constexpr T* release(this auto&& self) noexcept {
        T* ptr = self.ptr_;
        self.ptr_ = nullptr;
        return ptr;
    }

    constexpr void swap(this HeapGuard<T>& self, HeapGuard<T>& other) noexcept {
        auto tmp_ptr = self.ptr_;
        auto tmp_deleter = self.deleter_;
        self.ptr_ = other.ptr_;
        self.deleter_ = other.deleter_;
        other.ptr_ = tmp_ptr;
        other.deleter_ = tmp_deleter;
    }
};

} // namespace pltxt2htm
