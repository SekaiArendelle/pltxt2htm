#pragma once

#include <algorithm>
#include <concepts>
#include <type_traits>
#include <utility>

#include <fast_io/fast_io_core.h>
#include <fast_io/fast_io_dsal/string.h>

#include <pltxt2htm/container/string.hh>

#include "doctest_config.hh"
#include "container/details/concepts.hh"

using U8String = ::pltxt2htm::container::U8String;
using U8StringView = ::pltxt2htm::container::U8StringView;

static_assert(!can_call_empty<U8String>);

[[nodiscard]]
constexpr auto test_assign_growth_and_range() noexcept -> bool {
    U8String string{};
    string.assign(U8StringView{u8"abcdef"});
    if (string != u8"abcdef" || string.c_str()[string.size()] != u8'\0') {
        return false;
    }
    string.assign(string.data() + 2, string.data() + 5);
    if (string != u8"cde" || string.c_str()[string.size()] != u8'\0') {
        return false;
    }
    constexpr char8_t source[]{u8"longer replacement"};
    string.assign(source, source + sizeof(source) / sizeof(char8_t) - 1);
    if (string != u8"longer replacement" || string.c_str()[string.size()] != u8'\0') {
        return false;
    }
    string.assign(source, source);
    return string.is_empty() && string.c_str()[0] == u8'\0';
}

static_assert(test_assign_growth_and_range());

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

static_assert(::std::same_as<U8String::value_type, char8_t>);
static_assert(!::std::is_constructible_v<U8StringView, U8String&&>);
static_assert(!::std::is_convertible_v<U8String&&, U8StringView>);
static_assert(can_subscript<U8String const>);
static_assert(requires(U8String& string, ::std::size_t position) {
    string[position];
    string[position] = u8'a';
});
static_assert(::fast_io::alias_scannable<U8String>);
static_assert(::fast_io::context_scannable<
              char8_t, ::std::remove_cvref_t<decltype(::fast_io::io_scan_alias(::std::declval<U8String&>()))>>);
static_assert(::fast_io::context_scannable<char8_t, decltype(::fast_io::mnp::line_get(::std::declval<U8String&>()))>);
static_assert(::fast_io::context_scannable<char8_t, decltype(::fast_io::mnp::whole_get(::std::declval<U8String&>()))>);

consteval auto test_constexpr_string() noexcept -> bool {
    U8String empty_string;
    empty_string.clear();
    empty_string.assign(U8StringView{});
    empty_string.assign_characters(0);
    U8String moved_empty_string{::std::move(empty_string)};
    if (!empty_string.is_empty() || empty_string.c_str()[0] != u8'\0' || !moved_empty_string.is_empty() ||
        moved_empty_string.c_str()[0] != u8'\0') {
        return false;
    }
    U8String assigned_empty_string{u8"not empty"};
    assigned_empty_string = ::std::move(moved_empty_string);
    if (!moved_empty_string.is_empty() || moved_empty_string.c_str()[0] != u8'\0' ||
        !assigned_empty_string.is_empty() || assigned_empty_string.c_str()[0] != u8'\0') {
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
    string.insert(string.begin(), U8StringView{u8"0"});
    if (string != u8"0abcdef" || string.size() != 7 || string.c_str()[string.size()] != u8'\0') {
        return false;
    }

    U8String unchecked_string{};
    unchecked_string.push_back(u8'a');
    unchecked_string.append(u8"bc");
    if (unchecked_string != u8"abc") {
        return false;
    }

    U8String copy{string};
    U8String moved{::std::move(copy)};
    if (!copy.is_empty() || moved != string) {
        return false;
    }

    moved.pop_back();
    moved.assign(U8StringView{u8"xy"});
    if (moved != u8"xy" || moved.front() != u8'x' || moved.back() != u8'y') {
        return false;
    }

    U8String self_append{u8"abc"};
    self_append.reserve(16);
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
    self_insert.reserve(16);
    self_insert.insert(self_insert.begin() + 3, U8StringView{self_insert.data() + 1, 4});
    return self_insert == u8"abcbcdedef";
}

static_assert(test_constexpr_string());

TEST_SUITE("container_string") {
    TEST_CASE("assign-growth-and-range") {
        CHECK(test_assign_growth_and_range());
    }

    TEST_CASE("empty-string-state") {
        U8String empty_string;
        CHECK(empty_string.data() == ::fast_io::null_terminated_c_str_v<char8_t>);
        CHECK(empty_string.capacity() == 0);
        empty_string.clear();
        empty_string.assign(U8StringView{});
        empty_string.assign_characters(0);
        U8String moved_empty_string{::std::move(empty_string)};
        U8String assigned_empty_string{u8"not empty"};
        assigned_empty_string = ::std::move(moved_empty_string);
        CHECK(empty_string.data() == ::fast_io::null_terminated_c_str_v<char8_t>);
        CHECK(moved_empty_string.data() == ::fast_io::null_terminated_c_str_v<char8_t>);
        CHECK(assigned_empty_string.data() == ::fast_io::null_terminated_c_str_v<char8_t>);
    }

    TEST_CASE("fast-io-output") {
        U8String const string{u8"abc"};
        auto const printed = ::fast_io::u8concat_fast_io(string, 123);
        CHECK(printed == u8"abc123");
    }

    TEST_CASE("fast-io-scanning") {
        char8_t const token_input[]{u8"  first second"};
        ::fast_io::u8ibuffer_view token_buffer{token_input, token_input + sizeof(token_input) / sizeof(char8_t) - 1};
        U8String scanned_token{u8"old value"};
        ::fast_io::io::scan(token_buffer, scanned_token);
        CHECK(scanned_token == u8"first");
        ::fast_io::io::scan(token_buffer, scanned_token);
        CHECK(scanned_token == u8"second");

        char8_t const line_input[]{u8"first line\nsecond line"};
        ::fast_io::u8ibuffer_view line_buffer{line_input, line_input + sizeof(line_input) / sizeof(char8_t) - 1};
        U8String scanned_line{u8"old value"};
        ::fast_io::io::scan(line_buffer, ::fast_io::mnp::line_get(scanned_line));
        CHECK(scanned_line == u8"first line");
        ::fast_io::io::scan(line_buffer, ::fast_io::mnp::line_get(scanned_line));
        CHECK(scanned_line == u8"second line");

        char8_t const whole_input[]{u8"whole input\nwith spaces"};
        ::fast_io::u8ibuffer_view whole_buffer{whole_input, whole_input + sizeof(whole_input) / sizeof(char8_t) - 1};
        U8String scanned_whole{u8"old value"};
        ::fast_io::io::scan(whole_buffer, ::fast_io::mnp::whole_get(scanned_whole));
        CHECK(scanned_whole == u8"whole input\nwith spaces");

        char8_t const empty_input[]{u8""};
        ::fast_io::u8ibuffer_view empty_buffer{empty_input, empty_input};
        U8String scanned_empty{u8"old value"};
        ::fast_io::io::scan(empty_buffer, ::fast_io::mnp::whole_get(scanned_empty));
        CHECK(scanned_empty.is_empty());
    }

    TEST_CASE("zero-initialization") {
        U8String zero_string{1024};
        for (char8_t const character : zero_string) {
            REQUIRE(character == u8'\0');
        }
        CHECK(zero_string.c_str()[zero_string.size()] == u8'\0');

        ::pltxt2htm::container::BasicString<char8_t, DirtyAllocator> dirty_allocator_zero_string{1024};
        for (char8_t const character : dirty_allocator_zero_string) {
            REQUIRE(character == u8'\0');
        }
    }

    TEST_CASE("self-referential-append") {
        U8String self_append{u8"abc"};
        while (self_append.size() < self_append.capacity()) {
            self_append.push_back(u8'x');
        }
        ::std::size_t const append_old_size{self_append.size()};
        self_append.append(U8StringView{self_append.data(), 3});
        REQUIRE(self_append.size() == append_old_size + 3);
        CHECK(self_append[append_old_size] == u8'a');
        CHECK(self_append[append_old_size + 1] == u8'b');
        CHECK(self_append[append_old_size + 2] == u8'c');
    }

    TEST_CASE("self-referential-insert") {
        U8String self_insert_after_source{u8"abcdef"};
        self_insert_after_source.reserve(16);
        self_insert_after_source.insert(self_insert_after_source.begin() + 4,
                                        U8StringView{self_insert_after_source.data() + 1, 2});
        CHECK(self_insert_after_source == u8"abcdbcef");

        U8String self_insert_before_source{u8"abcdef"};
        self_insert_before_source.reserve(16);
        self_insert_before_source.insert(self_insert_before_source.begin() + 1,
                                         U8StringView{self_insert_before_source.data() + 3, 2});
        CHECK(self_insert_before_source == u8"adebcdef");

        U8String self_insert_overlapping{u8"abcdef"};
        self_insert_overlapping.reserve(16);
        self_insert_overlapping.insert(self_insert_overlapping.begin() + 3,
                                       U8StringView{self_insert_overlapping.data() + 1, 4});
        CHECK(self_insert_overlapping == u8"abcbcdedef");
    }

    TEST_CASE("self-referential-assign") {
        U8String self_assign{u8"abcdef"};
        self_assign.assign(U8StringView{self_assign.data() + 2, 3});
        CHECK(self_assign == u8"cde");
    }

    TEST_CASE("self-insert-reallocate") {
        U8String self_insert_reallocate{u8"abc"};
        while (self_insert_reallocate.size() < self_insert_reallocate.capacity()) {
            self_insert_reallocate.push_back(u8'x');
        }
        ::std::size_t const insert_old_size{self_insert_reallocate.size()};
        self_insert_reallocate.insert(self_insert_reallocate.begin() + 1,
                                      U8StringView{self_insert_reallocate.data(), 3});
        REQUIRE(self_insert_reallocate.size() == insert_old_size + 3);
        CHECK(self_insert_reallocate[0] == u8'a');
        CHECK(self_insert_reallocate[1] == u8'a');
        CHECK(self_insert_reallocate[2] == u8'b');
        CHECK(self_insert_reallocate[3] == u8'c');
        CHECK(self_insert_reallocate[4] == u8'b');
        CHECK(self_insert_reallocate[5] == u8'c');
    }

    TEST_CASE("reserve-preserves-contents") {
        U8String reserved{u8"reserve keeps contents"};
        reserved.reserve(reserved.capacity() + 1024);
        CHECK(reserved == u8"reserve keeps contents");
        CHECK(reserved.c_str()[reserved.size()] == u8'\0');
    }
}
