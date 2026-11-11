# Point Clang at the resource directory that is actually installed next to the compiler.
#
# conda-forge Clang bakes the build-time package prefix into `-print-resource-dir`. The
# compiler-rt runtimes (libclang_rt.*.a, required by -fsanitize=fuzzer/address/undefined) are
# shipped by a separate package and installed under the active prefix instead, so the reported
# directory has no `lib/` and every sanitizer link fails with "cannot find .../libclang_rt.*.a".
# Override the resource directory only when the reported one is unusable, leaving stock
# toolchains untouched.
function(pltxt2htm_use_installed_clang_resource_dir target visibility)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        return()
    endif()
    # clang-cl spells options differently; the affected environments (conda-forge on POSIX) are
    # never MSVC-like, so leave that combination alone.
    if(CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        return()
    endif()

    execute_process(
        COMMAND "${CMAKE_CXX_COMPILER}" -print-resource-dir
        OUTPUT_VARIABLE _pltxt2htm_reported_resource_dir
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
        RESULT_VARIABLE _pltxt2htm_probe_result)
    if(NOT _pltxt2htm_probe_result EQUAL 0 OR NOT _pltxt2htm_reported_resource_dir)
        return()
    endif()
    # The compiler-rt runtimes live in <resource-dir>/lib; without it sanitizer links fail.
    if(EXISTS "${_pltxt2htm_reported_resource_dir}/lib")
        return()
    endif()

    file(REAL_PATH "${CMAKE_CXX_COMPILER}" _pltxt2htm_compiler)
    get_filename_component(_pltxt2htm_bindir "${_pltxt2htm_compiler}" DIRECTORY)
    get_filename_component(_pltxt2htm_prefix "${_pltxt2htm_bindir}" DIRECTORY)
    file(GLOB _pltxt2htm_candidates LIST_DIRECTORIES TRUE "${_pltxt2htm_prefix}/lib/clang/*")
    set(_pltxt2htm_resource_dirs)
    foreach(_pltxt2htm_candidate IN LISTS _pltxt2htm_candidates)
        if(EXISTS "${_pltxt2htm_candidate}/lib")
            list(APPEND _pltxt2htm_resource_dirs "${_pltxt2htm_candidate}")
        endif()
    endforeach()
    if(NOT _pltxt2htm_resource_dirs)
        return()
    endif()
    list(SORT _pltxt2htm_resource_dirs COMPARE NATURAL)
    list(GET _pltxt2htm_resource_dirs -1 _pltxt2htm_resource_dir)
    message(STATUS
        "Clang resource dir '${_pltxt2htm_reported_resource_dir}' has no compiler-rt; "
        "using '${_pltxt2htm_resource_dir}'")

    target_compile_options(${target} ${visibility} "-resource-dir=${_pltxt2htm_resource_dir}")
    target_link_options(${target} ${visibility} "-resource-dir=${_pltxt2htm_resource_dir}")
endfunction()
