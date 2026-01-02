#include <cstdlib>
#include <memory>
#include <Python.h>
#include <fast_io/fast_io_dsal/array.h>
#include <pltxt2htm/pltxt2htm.h>

static ::PyObject* common_parser([[maybe_unused]] ::PyObject* self, ::PyObject* args, ::PyObject* kwargs) noexcept {
    constexpr auto kwlist = ::fast_io::array{"text", nullptr};
#ifndef NDEBUG
    char8_t const* text = nullptr;
#else
    #if __has_cpp_attribute(indeterminate)
    char8_t const* text [[indeterminate]];
    #else
    char8_t const* text;
    #endif
#endif
    // Before python3.13, argument `keywords` does not marked as const
    if (!::PyArg_ParseTupleAndKeywords(args, kwargs, "s",
#if PY_MINOR_VERSION < 13
                                       const_cast<char**>(kwlist.data()),
#else
                                       kwlist.data(),
#endif
                                       ::std::addressof(text))) [[unlikely]] {
        return nullptr;
    }
    char8_t const* html = ::pltxt2htm::common_parser<
#ifdef NDEBUG
        true
#else
        false
#endif
        >(text);
    ::PyObject* result = ::PyUnicode_FromString(reinterpret_cast<char const*>(html));
    ::free(reinterpret_cast<void*>(const_cast<char8_t*>(html)));
    return result;
}

static ::PyObject* advanced_parser([[maybe_unused]] ::PyObject* self, ::PyObject* args, ::PyObject* kwargs) noexcept {
    constexpr auto kwlist = ::fast_io::array{"text", nullptr};
#ifndef NDEBUG
    char8_t const* text = nullptr;
#else
    #if __has_cpp_attribute(indeterminate)
    char8_t const* text [[indeterminate]];
    #else
    char8_t const* text;
    #endif
#endif
    // Before python3.13, argument `keywords` does not marked as const
    if (!::PyArg_ParseTupleAndKeywords(args, kwargs, "s",
#if PY_MINOR_VERSION < 13
                                       const_cast<char**>(kwlist.data()),
#else
                                       kwlist.data(),
#endif
                                       ::std::addressof(text))) [[unlikely]] {
        return nullptr;
    }
    char8_t const* html = ::pltxt2htm::advanced_parser<
#ifdef NDEBUG
        true
#else
        false
#endif
        >(text);
    ::PyObject* result = ::PyUnicode_FromString(reinterpret_cast<char const*>(html));
    ::free(static_cast<void*>(const_cast<char8_t*>(html)));
    return result;
}

static ::PyObject* fixedadv_parser([[maybe_unused]] ::PyObject* self, ::PyObject* args, ::PyObject* kwargs) noexcept {
    constexpr auto kwlist = ::fast_io::array{"text", "host", nullptr};
#ifndef NDEBUG
    char8_t const* text = nullptr;
    char8_t const* host = nullptr;
#else
    #if __has_cpp_attribute(indeterminate)
    char8_t const* text [[indeterminate]];
    char8_t const* host [[indeterminate]];
    #else
    char8_t const* text;
    char8_t const* host;
    #endif
#endif
    // Before python3.13, argument `keywords` does not marked as const
    if (!::PyArg_ParseTupleAndKeywords(args, kwargs, "ss",
#if PY_MINOR_VERSION < 13
                                       const_cast<char**>(kwlist.data()),
#else
                                       kwlist.data(),
#endif
                                       ::std::addressof(text), ::std::addressof(host))) [[unlikely]] {
        return nullptr;
    }
    char8_t const* html = ::pltxt2htm::fixedadv_parser<
#ifdef NDEBUG
        true
#else
        false
#endif
        >(text, host);
    ::PyObject* result = ::PyUnicode_FromString(reinterpret_cast<char const*>(html));
    ::free(static_cast<void*>(const_cast<char8_t*>(html)));
    return result;
}

static auto methods_ = ::fast_io::array{
    // It was a little weird that PyCFunction mismatch with PyCFunctionWithKeywords, which will cause compiler warning
    ::PyMethodDef{"common_parser", reinterpret_cast<::PyCFunction>(::common_parser), METH_VARARGS | METH_KEYWORDS,
                  nullptr},
    ::PyMethodDef{"advanced_parser", reinterpret_cast<::PyCFunction>(::advanced_parser), METH_VARARGS | METH_KEYWORDS,
                  nullptr},
    ::PyMethodDef{"fixedadv_parser", reinterpret_cast<::PyCFunction>(::fixedadv_parser), METH_VARARGS | METH_KEYWORDS,
                  nullptr},
    ::PyMethodDef{nullptr, nullptr, 0, nullptr}};

static ::PyModuleDef pltxt2htm_py_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "pltxt2htm",
    .m_doc = "Parse Quantam-Physics's text to html",
    /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    .m_size = -1,
    .m_methods = ::methods_.data(),
    .m_slots = nullptr,
    .m_traverse = nullptr,
    .m_clear = nullptr,
    .m_free = nullptr};

PyMODINIT_FUNC PyInit_pltxt2htm() noexcept {
    ::PyObject* m = ::PyModule_Create(::std::addressof(::pltxt2htm_py_module));
    if (m == nullptr) [[unlikely]] {
        return nullptr;
    }

    ::PyModule_AddObject(
        m, "__version__",
        ::PyTuple_Pack(3, ::PyLong_FromLong(::pltxt2htm::version::major),
                       ::PyLong_FromLong(::pltxt2htm::version::minor), ::PyLong_FromLong(::pltxt2htm::version::patch)));

    return m;
}
