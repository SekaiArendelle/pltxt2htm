add_rules("mode.debug")
set_allowedmodes("debug")

set_languages("c++23")
set_encodings("utf-8")

for _, file in ipairs(os.files("*.cc")) do
    local name = path.basename(file)
    target(name) do
        set_symbols("debug")
        set_toolset("cxx", "clang++")
        set_toolset("ld", "clang++")
        set_kind("binary")
        set_default(false)
        add_files(name .. ".cc")
        add_tests("default")
        add_includedirs("../include")
        add_cxxflags("-fno-exceptions")
        add_cxxflags("-fno-rtti")
        add_cxxflags("-fno-unwind-tables")
        add_cxxflags("-fno-asynchronous-unwind-tables")
        add_cxxflags("-fsanitize=address")
        add_syslinks("ntdll")
        add_syslinks("clang_rt.asan_dynamic-x86_64")
    end
end
