/**
 * @file object_id.cc
 * @brief Unit tests for ::pltxt2htm::PlObjectId.
 */

#include "precompile.hh"

#include <cstddef>
#include <type_traits>
#include <pltxt2htm/ast/ast.hh>

namespace {

constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;
using ObjectId = ::pltxt2htm::PlObjectId<ndebug>;
using ::pltxt2htm::container::U8String;
using ::pltxt2htm::container::U8StringView;

constexpr auto canonical = U8StringView{u8"642cf37a494746375aae306a"};
constexpr auto other = U8StringView{u8"642cf37a494746375aae306b"};

} // namespace

int main() {
    static_assert(ObjectId::hex_digits == 24);
    static_assert(::std::is_nothrow_move_constructible_v<ObjectId>);
    static_assert(::std::is_nothrow_copy_constructible_v<ObjectId>);

    {
        auto opt_id = ObjectId::try_make(canonical);
        pltxt2htm_test_assert_true(opt_id.has_value());
        pltxt2htm_test_assert_true(opt_id.template value<ndebug>().as_string() == canonical);
    }
    {
        // BSON spells ObjectId in lowercase; uppercase source digits are folded.
        auto opt_id = ObjectId::try_make(U8StringView{u8"642CF37A494746375AAE306A"});
        pltxt2htm_test_assert_true(opt_id.has_value());
        pltxt2htm_test_assert_true(opt_id.template value<ndebug>().as_string() == canonical);
    }
    {
        // The 12-byte value carries no check digits, so the whole grammar is the length plus the
        // digit set: 24 digits and nothing else.
        pltxt2htm_test_assert_true(ObjectId::try_make(U8StringView{u8"expid"}).has_value() == false);
        pltxt2htm_test_assert_true(ObjectId::try_make(U8StringView{u8"642cf37a494746375aae306"}).has_value() == false);
        pltxt2htm_test_assert_true(ObjectId::try_make(U8StringView{u8"642cf37a494746375aae306a0"}).has_value() ==
                                   false);
        pltxt2htm_test_assert_true(ObjectId::try_make(U8StringView{u8"642cf37a494746375aae306g"}).has_value() == false);
        pltxt2htm_test_assert_true(ObjectId::try_make(U8StringView{u8""}).has_value() == false);
    }
    {
        auto const left = ObjectId::try_make(canonical).template value<ndebug>();
        auto const right = ObjectId::try_make(canonical).template value<ndebug>();
        auto const other_id = ObjectId::try_make(other).template value<ndebug>();
        pltxt2htm_test_assert_true(left == right);
        pltxt2htm_test_assert_false(left == other_id);
    }
    {
        // An all-digit payload is a valid ObjectId.
        pltxt2htm_test_assert_true(ObjectId::try_make(U8StringView{u8"000000000000000000000000"}).has_value());
    }
    {
        // The constructor is private, so try_make is the only way to produce a value and the
        // identifier grammar cannot be bypassed.
        static_assert(::std::is_constructible_v<ObjectId, U8String&&> == false);
        static_assert(::std::is_convertible_v<U8String, ObjectId> == false);
    }

    return 0;
}
