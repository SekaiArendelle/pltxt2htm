add_rules("mode.debug", "mode.release")
set_allowedmodes("debug", "release")

set_languages("c++23")
set_encodings("utf-8")

-- only uses emcc to build wasm
target("pltxt2htm", function()
    set_kind("binary")
    add_files("pltxt2htm.cc")
    set_extension(".js")
    add_cxxflags("-fvisibility=hidden", {tools = {"clangxx", "gcc"}})
    set_exceptions("no-cxx")
    add_cxxflags("-fno-rtti")
    add_cxxflags("-fno-unwind-tables")
    add_cxxflags("-fno-asynchronous-unwind-tables")
    if is_mode("release") then
        add_cxflags("-fno-ident")
    end
    add_includedirs("../include")
    add_ldflags("-fuse-ld=lld", {force = true})
    add_ldflags("-flto=thin", {force = true})
    add_ldflags("-s EXPORTED_FUNCTIONS=['_common_parser','_advanced_parser']", {force = true})
    add_ldflags("-s EXPORTED_RUNTIME_METHODS=['ccall','cwrap']", {force = true})
    add_ldflags("-s MODULARIZE=1", {force = true})
    add_ldflags("-s EXPORT_ES6=1", {force = true})
    add_ldflags("-s EXPORT_NAME=\"pltxt2htm\"", {force = true})

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
        if infer_out_dir then
            import("utils.archive")
            archive.archive(install_dir .. ".zip", install_dir)
            print("archive to \"" .. install_dir .. ".zip\"")
        end
    end)
end)
