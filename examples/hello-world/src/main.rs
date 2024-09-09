#![no_std]
#![no_main]
use macros::__syscall;
__syscall!();

const SYS_WRITE: isize = 1;
const SYS_EXIT: isize = 60;

const STDOUT: isize = 1;

#[panic_handler]
fn idiot_panic(_info: &core::panic::PanicInfo) -> ! {
    loop {}
}
// Entry point
#[no_mangle]
pub extern "C" fn _start() -> ! {
    let msg = "Hello, World!\n";
    unsafe {
        syscall!(SYS_WRITE, STDOUT, msg.as_ptr() as isize, msg.len() as isize);
        syscall!(SYS_EXIT, 0);
    }
    loop {}
}