#include <pltxt2htm/heap_guard.hh>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/list.h>

class Base {};

class Derived : public Base {};

int main() {
    {
        ::pltxt2htm::HeapGuard<int> a{};
        ::pltxt2htm::HeapGuard<int> b{};
        ::pltxt2htm::HeapGuard<int> c{1};

        ::exception::assert_true<false>(a == b);
        ::exception::assert_true<false>(a != c);
    }
    {
        ::fast_io::vector<::pltxt2htm::HeapGuard<int>> a{};
        ::fast_io::vector<::pltxt2htm::HeapGuard<int>> b{};
        ::exception::assert_true<false>(a == b);
    }
    {
        ::fast_io::list<::pltxt2htm::HeapGuard<int>> a{};
        ::fast_io::list<::pltxt2htm::HeapGuard<int>> b{};
        ::exception::assert_true<false>(a == b);
    }
    {
        ::pltxt2htm::HeapGuard<Derived> derived{};
        ::pltxt2htm::HeapGuard<Base> base(::std::move(derived));
    }
}
