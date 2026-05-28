use std::path::{Path, PathBuf};
use std::process::Command;

fn profile_mode() -> &'static str {
    if std::env::var("PROFILE")
        .map(|v| v == "release")
        .unwrap_or(false)
    {
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

fn try_run_cmake(c_root: &Path, out_dir: &Path) -> Result<(), String> {
    let install_dir = out_dir.join("pltxt2htm-install");
    let build_dir = c_root.join("build-cargo");

    // Clean any existing build dir
    let _ = std::fs::remove_dir_all(&build_dir);

    let mut cmake_cmd = Command::new("cmake");
    cmake_cmd
        .arg("-S")
        .arg(c_root)
        .arg("-B")
        .arg(&build_dir)
        .arg("-G")
        .arg("Ninja");

    let mode = profile_mode();
    cmake_cmd.arg("-DCMAKE_BUILD_TYPE=".to_owned() + mode);

    // Pass toolchain if CXX is set (e.g., cross-compilation via cargo)
    if let Ok(cxx) = std::env::var("CXX") {
        cmake_cmd.arg("-DCMAKE_CXX_COMPILER=".to_owned() + &cxx);
    }

    let config_status = cmake_cmd
        .status()
        .map_err(|e| format!("failed to invoke cmake: {e}"))?;
    if !config_status.success() {
        return Err("cmake configure failed".to_owned());
    }

    let build_status = Command::new("cmake")
        .arg("--build")
        .arg(&build_dir)
        .arg("-v")
        .status()
        .map_err(|e| format!("failed to invoke cmake --build: {e}"))?;
    if !build_status.success() {
        return Err("cmake build failed".to_owned());
    }

    let install_status = Command::new("cmake")
        .arg("--install")
        .arg(&build_dir)
        .arg("--prefix")
        .arg(&install_dir)
        .status()
        .map_err(|e| format!("failed to invoke cmake --install: {e}"))?;
    if !install_status.success() {
        return Err("cmake install failed".to_owned());
    }

    // Clean up build dir
    let _ = std::fs::remove_dir_all(&build_dir);

    println!(
        "cargo:rustc-link-search=native={}",
        install_dir.join("lib").display()
    );
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

    if std::env::var_os("CXX").is_none() {
        for cxx in [
            "clang++",
            "g++",
            "c++",
            "clang++-22",
            "clang++-21",
            "clang++-20",
            "g++-16",
            "g++-15",
            "g++-14",
        ] {
            if Command::new(cxx).arg("--version").status().is_ok() {
                build.compiler(cxx);
                println!("cargo:warning=Using C++ compiler fallback: {cxx}");
                break;
            }
        }
    }

    if profile_mode() == "release" {
        build.define("NDEBUG", None);
    }

    build.compile("pltxt2htm");
}

fn main() {
    let c_root = PathBuf::from("..").join("..").join("c");
    println!(
        "cargo:rerun-if-changed={}",
        c_root.join("src/pltxt2htm.cc").display()
    );
    println!(
        "cargo:rerun-if-changed={}",
        c_root.join("include/pltxt2htm.h").display()
    );
    println!(
        "cargo:rerun-if-changed={}",
        c_root.join("include/push_macros.h").display()
    );
    println!("cargo:rerun-if-changed=../../include/pltxt2htm");

    let out_dir = PathBuf::from(std::env::var("OUT_DIR").expect("OUT_DIR is not set"));

    match try_run_cmake(&c_root, &out_dir) {
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
    let stdlib =
        if target.contains("apple") || target.contains("freebsd") || target.contains("openbsd") {
            "c++"
        } else {
            "stdc++"
        };
    println!("cargo:rustc-link-lib={stdlib}");
}
