/**
 * @file heap_guard.hh
 * @brief RAII smart pointer for heap-allocated objects
 * @details Provides a unique_ptr-like smart pointer that manages heap-allocated
 *          objects with automatic cleanup. Unlike std::unique_ptr, this uses
 *          custom deleters and is optimized for the pltxt2htm use case.
 */

#pragma once

#include <cstdlib>
#include <memory>
#include <utility>
#include <concepts>
#include <type_traits>
#include <exception/exception.hh>

namespace pltxt2htm {

/**
 * @brief Forward declaration of HeapGuard template
 * @tparam T The type of object being managed
 */
template<typename T>
class HeapGuard;

namespace details {

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

} // namespace details

/**
 * @brief Concept to check if a type is a HeapGuard
 * @tparam T The type to check
 * @note This concept is used for template specialization and type checking
 */
template<typename T>
concept is_heap_guard = ::pltxt2htm::details::is_heap_guard_<::std::remove_cvref_t<T>>;

/**
 * @brief RAII smart pointer for heap-allocated objects
 * @details This class provides unique ownership of heap-allocated objects with
 *          automatic cleanup. It is similar to std::unique_ptr but optimized
 *          for the pltxt2htm library's specific needs.
 * @tparam T The type of object being managed
 * @note The class is marked with [[clang::trivial_abi]] when available for better performance
 */
template<typename T>
class
#if __has_cpp_attribute(clang::trivial_abi)
    [[clang::trivial_abi]]
#endif
    HeapGuard {
    T* ptr_;

public:
    void (*deleter_)(void*) noexcept;
    using value_type = T;

    /**
     * @brief Construct a HeapGuard by constructing the managed object in-place
     * @tparam Args Types of arguments to forward to the constructor
     * @param args Arguments to forward to the constructor of T
     * @note This function allocates memory on the heap and constructs the object in-place
     * @warning If allocation fails, the program terminates via exception::terminate()
     */
    template<typename... Args>
        requires (((!::pltxt2htm::is_heap_guard<Args>) && ...) && ::std::constructible_from<T, Args...>)
    constexpr HeapGuard(Args&&... args) noexcept {
        this->deleter_ = [](void* self) static constexpr noexcept { static_cast<T*>(self)->~T(); };
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
        : deleter_{other.deleter_} {
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
     * @brief Get a raw pointer to the managed object (unsafe)
     * @tparam Self The type of this object (deduced)
     * @param self This object
     * @return Raw pointer to the managed object
     * @note This is unsafe because it doesn't transfer ownership.
     *       The caller must ensure the HeapGuard outlives the pointer usage.
     * @warning Do not delete this pointer manually!
     */
    [[nodiscard]]
    constexpr T* get_unsafe(this auto&& self) noexcept {
        return self.ptr_;
    }

    /**
     * @brief Release ownership of the managed object
     * @tparam Self The type of this object (deduced)
     * @param self This object
     * @return Raw pointer to the managed object, ownership is transferred to caller
     * @note After calling this, the HeapGuard no longer manages the object.
     *       The caller is responsible for properly destroying and freeing the object.
     * @warning The returned pointer must be properly deleted using the original deleter
     */
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
