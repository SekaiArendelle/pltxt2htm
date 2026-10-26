/**
 * @file object_id.hh
 * @brief Unit tests for ::pltxt2htm::PlObjectId.
 */

#pragma once

#include "doctest_config.hh"

#include <cstddef>
#include <type_traits>
#include <pltxt2htm/ast/ast.hh>

TEST_SUITE("object_id") {
    using ObjectId = ::pltxt2htm::PlObjectId;
    using ::pltxt2htm::container::U8String;
    using ::pltxt2htm::container::U8StringView;

    constexpr auto canonical = U8StringView{u8"642cf37a494746375aae306a"};
    constexpr auto other = U8StringView{u8"642cf37a494746375aae306b"};

    static_assert(ObjectId::hex_digits == 24);
    static_assert(::std::is_nothrow_move_constructible_v<ObjectId>);
    static_assert(::std::is_nothrow_copy_constructible_v<ObjectId>);

    TEST_CASE("canonical identifier") {
        auto opt_id = ObjectId::try_make(canonical);
        REQUIRE(opt_id.has_value());
        CHECK(opt_id.value().as_string() == canonical);
    }
    TEST_CASE("uppercase digits are folded") {
        // BSON spells ObjectId in lowercase; uppercase source digits are folded.
        auto opt_id = ObjectId::try_make(U8StringView{u8"642CF37A494746375AAE306A"});
        REQUIRE(opt_id.has_value());
        CHECK(opt_id.value().as_string() == canonical);
    }
    TEST_CASE("invalid identifiers") {
        // The 12-byte value carries no check digits, so the whole grammar is the length plus the
        // digit set: 24 digits and nothing else.
        CHECK_FALSE(ObjectId::try_make(U8StringView{u8"expid"}).has_value());
        CHECK_FALSE(ObjectId::try_make(U8StringView{u8"642cf37a494746375aae306"}).has_value());
        CHECK_FALSE(ObjectId::try_make(U8StringView{u8"642cf37a494746375aae306a0"}).has_value());
        CHECK_FALSE(ObjectId::try_make(U8StringView{u8"642cf37a494746375aae306g"}).has_value());
        CHECK_FALSE(ObjectId::try_make(U8StringView{u8""}).has_value());
    }
    TEST_CASE("identifier equality") {
        auto const left = ObjectId::try_make(canonical).value();
        auto const right = ObjectId::try_make(canonical).value();
        auto const other_id = ObjectId::try_make(other).value();
        CHECK(left == right);
        CHECK_FALSE(left == other_id);
    }
    TEST_CASE("all-digit identifier") {
        // An all-digit payload is a valid ObjectId.
        CHECK(ObjectId::try_make(U8StringView{u8"000000000000000000000000"}).has_value());
    }
    // The constructor is private, so try_make is the only way to produce a value and the
    // identifier grammar cannot be bypassed.
    static_assert(::std::is_constructible_v<ObjectId, U8String&&> == false);
    static_assert(::std::is_convertible_v<U8String, ObjectId> == false);
}
