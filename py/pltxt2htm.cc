#include <cstdlib>
#include <memory>
#include <Python.h>
#include <pltxt2htm/pltxt2htm.h>

static PyObject* common_parser(PyObject* self, PyObject* args, PyObject* kwargs) noexcept {
    static char const* kwlist[] = {"text", "host", nullptr};
#ifndef NDEBUG
    char8_t const* text = nullptr;
    char8_t const* host = nullptr;
#else
    #if __has_attribute(indeterminate)
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
                                       const_cast<char**>(kwlist),
#else
                                       kwlist,
#endif
                                       ::std::addressof(text), ::std::addressof(host))) [[unlikely]] {
        return nullptr;
    }
    char8_t const* html =
        ::pltxt2htm::common_parser(reinterpret_cast<char8_t const*>(text), reinterpret_cast<char8_t const*>(host));
    PyObject* result = ::PyUnicode_FromString(reinterpret_cast<char const*>(html));
    ::free(static_cast<void*>(const_cast<char8_t*>(html)));
    return result;
}

static PyObject* advanced_parser(PyObject* self, PyObject* args, PyObject* kwargs) noexcept {
    static char const* kwlist[] = {"text", "host", nullptr};
#ifndef NDEBUG
    char8_t const* text = nullptr;
    char8_t const* host = nullptr;
#else
    #if __has_attribute(indeterminate)
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
                                       const_cast<char**>(kwlist),
#else
                                       kwlist,
#endif
                                       ::std::addressof(text), ::std::addressof(host))) [[unlikely]] {
        return nullptr;
    }
    char8_t const* html =
        ::pltxt2htm::advanced_parser(reinterpret_cast<char8_t const*>(text), reinterpret_cast<char8_t const*>(host));
    PyObject* result = ::PyUnicode_FromString(reinterpret_cast<char const*>(html));
    ::free(static_cast<void*>(const_cast<char8_t*>(html)));
    return result;
}

static PyMethodDef methods_[] = {
    // It was a little weird that PyCFunction mismatch with PyCFunctionWithKeywords, which will cause compiler warning
    {"common_parser", reinterpret_cast<PyCFunction>(common_parser), METH_VARARGS | METH_KEYWORDS, nullptr},
    {"advanced_parser", reinterpret_cast<PyCFunction>(advanced_parser), METH_VARARGS | METH_KEYWORDS, nullptr},
    {nullptr, nullptr, 0, nullptr}};

static struct PyModuleDef pltxt2htm_py_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "pltxt2htm",
    .m_doc = "Parse Quantam-Physics's text to html",
    .m_size =
        -1, /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    .m_methods = methods_,
    .m_slots = nullptr,
};

PyMODINIT_FUNC PyInit_pltxt2htm() noexcept {
    return ::PyModule_Create(::std::addressof(pltxt2htm_py_module));
}
