add_rules("mode.debug", "mode.release")

target("pltxt2htm") do
    set_languages("c++23")
    set_plat("wasm")
    set_kind("binary")
    add_files("pltxt2htm.cc")
    add_cxxflags("-fno-exceptions")
    add_cxxflags("-fno-rtti")
    add_cxxflags("-fno-unwind-tables")
    add_cxxflags("-fno-asynchronous-unwind-tables")
    add_cxxflags("-fno-ident")
    add_includedirs("../include")
    add_ldflags("-s EXPORTED_FUNCTIONS=['_pltxt2html']", {force = true})
    add_ldflags("-s EXPORTED_RUNTIME_METHODS=['ccall']", {force = true})
    set_toolchains("emcc@emscripten")
end
