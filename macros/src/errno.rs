//! # Errno
//! This macro generates a errno enum with all the possible error codes
//! It also generates a static table with the error codes and their descriptions
//! 
//! ## Source
//! The table is generated from the Linux kernel source code:
//!  - [`errno-base.h`](https://github.com/torvalds/linux/blob/master/include/uapi/asm-generic/errno-base.h)
//!  - [`errno.h`](https://github.com/torvalds/linux/blob/master/include/uapi/asm-generic/errno.h)

const ERRNO_BASE_H: &str = include_str!("../resources/errno-base.h");
const ERRNO_H: &str = include_str!("../resources/errno.h");

struct errno {
    name: String,
    value: i32,
    description: String,
}