set_policy("check.auto_ignore_flags", false)

add_rules("mode.debug", "mode.release")
set_allowedmodes("debug", "release")

set_languages("c++23")
set_encodings("utf-8")

-- only uses emcc to build wasm
target("pltxt2htm", function()
    set_kind("binary")
    add_files("pltxt2htm.cc")
    set_extension(".js")
    add_cxxflags("-fvisibility=hidden")
    set_exceptions("no-cxx")
    add_cxxflags("-fno-rtti")
    add_cxxflags("-fno-unwind-tables")
    add_cxxflags("-fno-asynchronous-unwind-tables")
    if is_mode("release") then
        add_cxxflags("-fno-ident")
        add_ldflags("-flto")
    end
    add_includedirs("$(projectdir)/../include")
    add_ldflags("-fuse-ld=lld")
    add_ldflags("-s EXPORTED_FUNCTIONS=['_common_parser','_advanced_parser','_fixedadv_parser','_ver_major','_ver_minor','_ver_patch']")
    add_ldflags("-s EXPORTED_RUNTIME_METHODS=['ccall','cwrap']")
    add_ldflags("-s MODULARIZE=1")
    add_ldflags("-s EXPORT_ES6=1")
    add_ldflags("-s EXPORT_NAME=\"pltxt2htm\"")

    on_install(function (target)
        local install_dir = target:installdir()
        local infer_out_dir = false
        if not install_dir or install_dir == "auto" then
            infer_out_dir = true
        end
        if infer_out_dir then
            install_dir = "wasm32-unknown-emscripten-pltxt2htm-wasm-" .. get_config("mode")
        end
        if not os.exists(install_dir) or not os.isdir(install_dir) then
            os.mkdir(install_dir)
        end
        for _, a_file in ipairs(target:targetdir() .. "/pltxt2htm*") do
            os.cp(a_file, install_dir)
        end
        print("install to \"" .. path.join(target:scriptdir(), install_dir) .. "\"")
    end)
end)
