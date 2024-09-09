pub enum Error {
    FromSyscall(isize),
}

impl Error {
    pub fn from_syscall(value: isize) -> Self {
        Error::FromSyscall(value)
    }
}
