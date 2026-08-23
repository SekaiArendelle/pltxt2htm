#include <algorithm>
#include <concepts>
#include <type_traits>

#include <fast_io/fast_io.h>

#include <pltxt2htm/container/string.hh>

#include "precompile.hh"

using U8String = ::pltxt2htm::container::U8String;
using U8StringView = ::pltxt2htm::container::U8StringView;
using Contracts = ::pltxt2htm::Contracts;

class DirtyAllocator {
public:
    static constexpr auto allocate(::std::size_t size) noexcept -> void* {
        auto* const pointer{static_cast<unsigned char*>(::fast_io::c_malloc_allocator::allocate(size))};
        ::std::fill_n(pointer, size, static_cast<unsigned char>(0xA5));
        return pointer;
    }

    static constexpr auto allocate_zero(::std::size_t size) noexcept -> void* {
        return ::fast_io::c_malloc_allocator::allocate_zero(size);
    }

    static constexpr void deallocate(void* pointer) noexcept {
        ::fast_io::c_malloc_allocator::deallocate(pointer);
    }
};

template<typename String>
concept has_subscript_operator = requires(String& string) { string[0]; };

static_assert(::std::same_as<U8String::value_type, char8_t>);
static_assert(::std::is_constructible_v<U8String, ::fast_io::u8string const&>);
static_assert(::std::is_constructible_v<U8StringView, U8String const&>);
static_assert(!::std::is_constructible_v<U8StringView, U8String&&>);
static_assert(::std::is_convertible_v<U8String const&, U8StringView>);
static_assert(!::std::is_convertible_v<U8String&&, U8StringView>);
static_assert(!has_subscript_operator<U8String>);
static_assert(!has_subscript_operator<U8String const>);

consteval auto test_constexpr_string() noexcept -> bool {
    U8String empty_string;
    empty_string.clear();
    empty_string.assign(U8StringView{});
    empty_string.assign_characters(0);
    U8String moved_empty_string{::std::move(empty_string)};
    if (!empty_string.empty() || empty_string.c_str()[0] != u8'\0' || !moved_empty_string.empty() ||
        moved_empty_string.c_str()[0] != u8'\0') {
        return false;
    }
    U8String assigned_empty_string{u8"not empty"};
    assigned_empty_string = ::std::move(moved_empty_string);
    if (!moved_empty_string.empty() || moved_empty_string.c_str()[0] != u8'\0' || !assigned_empty_string.empty() ||
        assigned_empty_string.c_str()[0] != u8'\0') {
        return false;
    }

    constexpr auto literal = ::pltxt2htm::details::U8LiteralString{u8"literal"};
    U8String const literal_string{literal};
    if (literal_string != u8"literal") {
        return false;
    }

    U8String const zero_string{16};
    if (!::std::all_of(zero_string.begin(), zero_string.end(), [](char8_t character) { return character == u8'\0'; }) ||
        zero_string.c_str()[zero_string.size()] != u8'\0') {
        return false;
    }

    U8String string{u8"abc"};
    string.push_back(u8'd');
    string.append(u8"ef");
    string.insert<Contracts::quick_enforce>(string.begin(), U8StringView{u8"0"});
    if (string != u8"0abcdef" || string.size() != 7 || string.c_str()[string.size()] != u8'\0') {
        return false;
    }

    U8String copy{string};
    U8String moved{::std::move(copy)};
    if (!copy.empty() || moved != string) {
        return false;
    }

    moved.pop_back<Contracts::quick_enforce>();
    moved.assign(U8StringView{u8"xy"});
    if (moved != u8"xy" || moved.front<Contracts::quick_enforce>() != u8'x' ||
        moved.back<Contracts::quick_enforce>() != u8'y') {
        return false;
    }

    U8String self_append{u8"abc"};
    self_append.reserve<Contracts::quick_enforce>(16);
    self_append.append(U8StringView{self_append.data() + 1, 2});
    if (self_append != u8"abcbc") {
        return false;
    }

    U8String self_assign{u8"abcdef"};
    self_assign.assign(U8StringView{self_assign.data() + 2, 3});
    if (self_assign != u8"cde") {
        return false;
    }

    U8String self_insert{u8"abcdef"};
    self_insert.reserve<Contracts::quick_enforce>(16);
    self_insert.insert<Contracts::quick_enforce>(self_insert.begin() + 3, U8StringView{self_insert.data() + 1, 4});
    return self_insert == u8"abcbcdedef";
}

static_assert(test_constexpr_string());

int main() {
    U8String empty_string;
    pltxt2htm_test_assert_true(empty_string.data() == ::fast_io::null_terminated_c_str_v<char8_t>);
    pltxt2htm_test_assert_true(empty_string.capacity() == 0);
    empty_string.clear();
    empty_string.assign(U8StringView{});
    empty_string.assign_characters(0);
    U8String moved_empty_string{::std::move(empty_string)};
    U8String assigned_empty_string{u8"not empty"};
    assigned_empty_string = ::std::move(moved_empty_string);
    pltxt2htm_test_assert_true(empty_string.data() == ::fast_io::null_terminated_c_str_v<char8_t>);
    pltxt2htm_test_assert_true(moved_empty_string.data() == ::fast_io::null_terminated_c_str_v<char8_t>);
    pltxt2htm_test_assert_true(assigned_empty_string.data() == ::fast_io::null_terminated_c_str_v<char8_t>);

    U8String const string{u8"abc"};
    auto const printed = ::fast_io::u8concat_fast_io(string, 123);
    pltxt2htm_test_assert_true(printed == u8"abc123");

    ::fast_io::u8string const fast_io_string{u8"fast_io"};
    U8String const compatible{fast_io_string};
    pltxt2htm_test_assert_true(compatible == u8"fast_io");

    U8String zero_string{1024};
    for (char8_t const character : zero_string) {
        pltxt2htm_test_assert_true(character == u8'\0');
    }
    pltxt2htm_test_assert_true(zero_string.c_str()[zero_string.size()] == u8'\0');

    ::pltxt2htm::container::BasicString<char8_t, DirtyAllocator> dirty_allocator_zero_string{1024};
    for (char8_t const character : dirty_allocator_zero_string) {
        pltxt2htm_test_assert_true(character == u8'\0');
    }

    U8String self_append{u8"abc"};
    while (self_append.size() < self_append.capacity()) {
        self_append.push_back(u8'x');
    }
    ::std::size_t const append_old_size{self_append.size()};
    self_append.append(U8StringView{self_append.data(), 3});
    pltxt2htm_test_assert_true(self_append.size() == append_old_size + 3);
    pltxt2htm_test_assert_true(self_append.index<Contracts::quick_enforce>(append_old_size) == u8'a');
    pltxt2htm_test_assert_true(self_append.index<Contracts::quick_enforce>(append_old_size + 1) == u8'b');
    pltxt2htm_test_assert_true(self_append.index<Contracts::quick_enforce>(append_old_size + 2) == u8'c');

    U8String self_insert_after_source{u8"abcdef"};
    self_insert_after_source.reserve<Contracts::quick_enforce>(16);
    self_insert_after_source.insert<Contracts::quick_enforce>(self_insert_after_source.begin() + 4,
                                                              U8StringView{self_insert_after_source.data() + 1, 2});
    pltxt2htm_test_assert_true(self_insert_after_source == u8"abcdbcef");

    U8String self_insert_before_source{u8"abcdef"};
    self_insert_before_source.reserve<Contracts::quick_enforce>(16);
    self_insert_before_source.insert<Contracts::quick_enforce>(self_insert_before_source.begin() + 1,
                                                               U8StringView{self_insert_before_source.data() + 3, 2});
    pltxt2htm_test_assert_true(self_insert_before_source == u8"adebcdef");

    U8String self_insert_overlapping{u8"abcdef"};
    self_insert_overlapping.reserve<Contracts::quick_enforce>(16);
    self_insert_overlapping.insert<Contracts::quick_enforce>(self_insert_overlapping.begin() + 3,
                                                             U8StringView{self_insert_overlapping.data() + 1, 4});
    pltxt2htm_test_assert_true(self_insert_overlapping == u8"abcbcdedef");

    U8String self_assign{u8"abcdef"};
    self_assign.assign(U8StringView{self_assign.data() + 2, 3});
    pltxt2htm_test_assert_true(self_assign == u8"cde");

    U8String self_insert_reallocate{u8"abc"};
    while (self_insert_reallocate.size() < self_insert_reallocate.capacity()) {
        self_insert_reallocate.push_back(u8'x');
    }
    ::std::size_t const insert_old_size{self_insert_reallocate.size()};
    self_insert_reallocate.insert<Contracts::quick_enforce>(self_insert_reallocate.begin() + 1,
                                                            U8StringView{self_insert_reallocate.data(), 3});
    pltxt2htm_test_assert_true(self_insert_reallocate.size() == insert_old_size + 3);
    pltxt2htm_test_assert_true(self_insert_reallocate.index<Contracts::quick_enforce>(0) == u8'a');
    pltxt2htm_test_assert_true(self_insert_reallocate.index<Contracts::quick_enforce>(1) == u8'a');
    pltxt2htm_test_assert_true(self_insert_reallocate.index<Contracts::quick_enforce>(2) == u8'b');
    pltxt2htm_test_assert_true(self_insert_reallocate.index<Contracts::quick_enforce>(3) == u8'c');
    pltxt2htm_test_assert_true(self_insert_reallocate.index<Contracts::quick_enforce>(4) == u8'b');
    pltxt2htm_test_assert_true(self_insert_reallocate.index<Contracts::quick_enforce>(5) == u8'c');

    U8String reserved{u8"reserve keeps contents"};
    reserved.reserve<Contracts::quick_enforce>(reserved.capacity() + 1024);
    pltxt2htm_test_assert_true(reserved == u8"reserve keeps contents");
    pltxt2htm_test_assert_true(reserved.c_str()[reserved.size()] == u8'\0');

    return 0;
}
