#include <pltxt2htm/heap_guard.hh>

int main() {
    ::pltxt2htm::HeapGuard<int> a{};
    ::pltxt2htm::HeapGuard<int> b{};
    ::pltxt2htm::HeapGuard<int> c{1};

    ::exception::assert_true<false>(a == b);
    ::exception::assert_true<false>(a != c);
}
