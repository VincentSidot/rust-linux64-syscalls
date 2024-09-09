extern crate proc_macro;
mod syscall;

#[proc_macro]
pub fn __syscall(input: proc_macro::TokenStream) -> proc_macro::TokenStream {
    syscall::syscall(input)
}
