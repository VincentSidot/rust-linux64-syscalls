fn main() {
    // Tell cargo to tell rustc to link the system c runtime
    println!("cargo:rustc-link-arg=-nostartfiles");
    println!("cargo:rustc-link-lib=c");
    println!("cargo:rustc-link-arg=-nodefaultlibs");
    println!("cargo:rustc-link-arg=-nostdlib");
    
    // Also tell cargo to use panic=abort when compiling
    println!("cargo:panic=abort");
}
