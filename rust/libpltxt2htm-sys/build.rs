use std::path::{Path, PathBuf};
use std::process::Command;

fn profile_mode() -> &'static str {
    if std::env::var("PROFILE").map(|v| v == "release").unwrap_or(false) {
        "release"
    } else {
        "debug"
    }
}

fn static_lib_name(target: &str) -> &'static str {
    if target.contains("windows") && profile_mode() == "debug" {
        "pltxt2htmd"
    } else {
        "pltxt2htm"
    }
}

fn try_run_xmake(c_root: &Path, out_dir: &Path) -> Result<(), String> {
    let install_dir = out_dir.join("pltxt2htm-install");

    let config_status = Command::new("xmake")
        .arg("config")
        .arg("-P")
        .arg(c_root)
        .arg("-k")
        .arg("static")
        .arg("-m")
        .arg(profile_mode())
        .status()
        .map_err(|e| format!("failed to invoke xmake config: {e}"))?;
    if !config_status.success() {
        return Err("xmake config failed".to_owned());
    }

    let build_status = Command::new("xmake")
        .arg("build")
        .arg("-P")
        .arg(c_root)
        .status()
        .map_err(|e| format!("failed to invoke xmake build: {e}"))?;
    if !build_status.success() {
        return Err("xmake build failed".to_owned());
    }

    let install_status = Command::new("xmake")
        .arg("install")
        .arg("-P")
        .arg(c_root)
        .arg("-o")
        .arg(&install_dir)
        .status()
        .map_err(|e| format!("failed to invoke xmake install: {e}"))?;
    if !install_status.success() {
        return Err("xmake install failed".to_owned());
    }

    println!("cargo:rustc-link-search=native={}", install_dir.join("lib").display());
    Ok(())
}

fn fallback_build_by_cc(c_root: &Path) {
    let mut build = cc::Build::new();
    build
        .cpp(true)
        .std("c++23")
        .file(c_root.join("src/pltxt2htm.cc"))
        .include(c_root.join("include"))
        .include("../../include")
        .warnings(false);

    if Command::new("clang++-20").arg("--version").status().is_ok() {
        build.compiler("clang++-20");
    }

    if profile_mode() == "release" {
        build.define("NDEBUG", None);
    }

    build.compile("pltxt2htm");
}

fn main() {
    let c_root = PathBuf::from("..").join("..").join("c");
    println!("cargo:rerun-if-changed={}", c_root.join("src/pltxt2htm.cc").display());
    println!("cargo:rerun-if-changed={}", c_root.join("include/pltxt2htm.h").display());
    println!("cargo:rerun-if-changed={}", c_root.join("include/push_macros.h").display());
    println!("cargo:rerun-if-changed=../../include/pltxt2htm");

    let out_dir = PathBuf::from(std::env::var("OUT_DIR").expect("OUT_DIR is not set"));

    match try_run_xmake(&c_root, &out_dir) {
        Ok(()) => {}
        Err(reason) => {
            println!("cargo:warning=Using cc-rs fallback because {reason}");
            fallback_build_by_cc(&c_root);
        }
    }

    let target = std::env::var("TARGET").unwrap_or_default();
    println!("cargo:rustc-link-lib=static={}", static_lib_name(&target));

    if target.contains("msvc") {
        return;
    }
    let stdlib = if target.contains("apple") || target.contains("freebsd") || target.contains("openbsd") {
        "c++"
    } else {
        "stdc++"
    };
    println!("cargo:rustc-link-lib={stdlib}");
}
