# A source-tree target uses build-only options, keeping the exported package free
# of flags selected by the build machine. An imported target gets the consumer's
# platform options directly.
function(pltxt2htm_apply_platform_options target)
    get_target_property(is_imported "${target}" IMPORTED)

    if(MSVC)
        # Universal character names inside u8 literals must keep their UTF-8
        # encoding. The exception model remains the consumer's choice.
        if(is_imported)
            target_compile_options("${target}" INTERFACE /utf-8 /Zc:u8EscapeEncoding)
        else()
            target_compile_options("${target}" INTERFACE
                "$<BUILD_INTERFACE:/utf-8>" "$<BUILD_INTERFACE:/Zc:u8EscapeEncoding>")
        endif()
    endif()

    if(WIN32 OR MINGW OR CMAKE_CXX_COMPILER_TARGET MATCHES "windows-msvc$")
        if(is_imported)
            target_link_libraries("${target}" INTERFACE ntdll)
        else()
            target_link_libraries("${target}" INTERFACE "$<BUILD_INTERFACE:ntdll>")
        endif()
    endif()
endfunction()
