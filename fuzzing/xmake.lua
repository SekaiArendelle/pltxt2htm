set_allowedmodes("debug")
add_rules("mode.debug")
set_defaultmode("debug")

includes("../xmake/*.lua")

set_languages("c++23")
set_encodings("utf-8")
set_symbols("debug")
set_kind("binary")
set_exceptions("no-cxx")
add_ldflags("-fuse-ld=lld")
add_cxxflags("-fsanitize=fuzzer", {force = true})
add_ldflags("-fsanitize=fuzzer", {force = true})
add_cxxflags("-fno-rtti")
add_cxxflags("-fno-omit-frame-pointer")
if is_plat("windows") or is_plat("mingw") then
    print("error: do not support fuzzing on windows")
    os.exit(1)
end
add_includedirs("$(projectdir)/../include")

on_config(function (target)
    local toolchains = target:tool("cxx")
    if path.basename(toolchains) ~= "clang++" and path.basename(toolchains) ~= "clang" then
        print("error: Only `--toolchain=clang` is supported")
        os.exit(1)
    end
end)

target("advanced_parser", function()
    add_files("$(projectdir)/advanced_parser.cc")
end)

target("common_parser", function()
    add_files("$(projectdir)/common_parser.cc")
end)
