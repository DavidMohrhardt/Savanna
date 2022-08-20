/**
 * Builds the Rust bindings for the library.
 */
fn main() -> miette::Result<()> {
    let path = std::path::PathBuf::from("src");
    let mut b = autocxx_build::Builder::new("src/lib.rs", &[&path]).build();
    b.flag_if_supported("-std=c++20").compile("autocxx-demo");

    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-changed=src/SavannaCore_BindingsWrapper.h");
    Ok(())
    // let path = std::path::PathBuf::from("src"); // include path
    // let mut b = autocxx_build::Builder::new("src/lib.rs", &[&path]).build();
    //     // This assumes all your C++ bindings are in main.rs
    // b?.flag_if_supported("-std=c++14")
    //  .compile("autocxx-demo"); // arbitrary library name, pick anything
    // println!("cargo:rerun-if-changed=src/main.rs");
    // // Add instructions to link to any C++ libraries you need.
    // Ok(())
}
