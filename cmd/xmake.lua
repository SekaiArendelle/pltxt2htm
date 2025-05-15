add_rules("mode.debug", "mode.release")

set_languages("c++23")
set_encodings("utf-8")

target("pltxt2htm") do
    set_kind("binary")
    add_files("main.cc")
    add_includedirs("../include")
    if is_mode("release") then
        add_cxxflags("-fno-exceptions")
        add_cxxflags("-fno-rtti")
        add_cxxflags("-fno-unwind-tables")
        add_cxxflags("-fno-asynchronous-unwind-tables")
        add_cxflags("-fno-ident")
    end
    add_syslinks("ntdll")
end
