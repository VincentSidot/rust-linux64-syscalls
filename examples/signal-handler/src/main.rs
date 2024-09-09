use std::arch::global_asm;

use macros::__syscall;
__syscall!();

const SYS_GETPID: isize = 0x27;
const SYS_SIGACTION: isize = 0x0d;
// const SYS_SIGRETURN: isize = 0x0f;

const SA_RESTART: c_int = 0x10000000;
const SA_RESTORER: c_int = 0x04000000;

const SIGINT: i32 = 0x2;

#[allow(non_camel_case_types)]
type size_t = usize;
#[allow(non_camel_case_types)]
type sighandler_t = size_t;
#[allow(non_camel_case_types)]
type c_int = i32;

#[repr(C)]
#[allow(non_camel_case_types)]
#[derive(Debug)]
struct sigset_t {
    #[cfg(target_pointer_width = "32")]
    __val: [u32; 32],
    #[cfg(target_pointer_width = "64")]
    __val: [u64; 16],
}

#[repr(C)]
#[allow(non_camel_case_types)]
#[derive(Debug)]
struct kernel_sigaction {
    // pub sa_sigaction: sighandler_t,
    // pub sa_mask: sigset_t,
    // #[cfg(target_arch = "sparc64")]
    // __reserved0: c_int,
    // pub sa_flags: c_int,
    // pub sa_restorer: Option<extern "C" fn()>,
    sa_handler: sighandler_t,
    sa_flags: c_int,
    sa_restorer: unsafe extern "C" fn(),
    sa_mask: sigset_t,
}

macro_rules! syscall_ret {
    ($code:ident, $($args:expr),* => $transformer:block) => {
        {
            let $code = unsafe { syscall!($($args),*) };
            if $code < 0 {
                Err(-$code)?
            } else {
                $transformer
            }
        }
    };
    (@noret $($args:expr),*) => {
        syscall_ret!(code, $($args),* => { Ok(()) })
    };
    (@cast $($args:expr),*) => {
        syscall_ret!(code, $($args),* => { Ok(code as _)})
    };

}

#[derive(Debug)]
struct SyscallError {
    code: isize,
}

impl std::fmt::Display for SyscallError {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(f, "SyscallError({})", self.code)
    }
}

type SysResult<T> = Result<T, SyscallError>;

impl SyscallError {
    fn new(code: isize) -> Self {
        Self { code }
    }
}

impl From<isize> for SyscallError {
    fn from(code: isize) -> Self {
        Self::new(code)
    }
}

impl std::error::Error for SyscallError {}

fn getpid() -> SysResult<u32> {
    syscall_ret!(@cast SYS_GETPID)
}

fn rt_sigaction(
    signum: c_int,
    act: &kernel_sigaction,
    oldact: Option<&mut kernel_sigaction>,
) -> SysResult<()> {
    let size = std::mem::size_of::<&kernel_sigaction>();
    syscall_ret!(@noret
        SYS_SIGACTION,
        signum,
        act as *const kernel_sigaction as usize,
        oldact.map_or(std::ptr::null_mut(), |oldact| oldact as *mut kernel_sigaction) as usize,
        size
    )
}

extern "C" fn handler(signum: c_int) {
    println!("Signal: {}", signum);
}

extern "C" {
    fn restorer();
}

global_asm!(
    r#"
    restorer:
        mov eax, 0x0f
        syscall
    "#
);

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("PID: {}", getpid()?);

    // Set up signal handler
    let act = kernel_sigaction {
        sa_handler: handler as sighandler_t,
        sa_mask: unsafe { std::mem::zeroed() },
        sa_flags: SA_RESTART | SA_RESTORER,
        sa_restorer: restorer,
    };

    println!("act size: {}", std::mem::size_of::<kernel_sigaction>());

    // let mut oldact = unsafe { std::mem::zeroed() };

    rt_sigaction(SIGINT, &act, None)?;

    loop {
        println!("Hello!");
        std::thread::sleep(std::time::Duration::from_secs(1));
    }
}
