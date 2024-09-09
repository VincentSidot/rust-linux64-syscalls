/// This macro generates a syscall function for each number of arguments
/// It also generates a macro to call the syscall functions with variadic arguments
use proc_macro::TokenStream;
use std::str::FromStr;

const REGISTERS: [&str; 6] = ["rdi", "rsi", "rdx", "r10", "r8", "r9"];
pub fn syscall(_n: TokenStream) -> TokenStream {
    let mut payload = String::new();
    for n in 0..=6 {
        let mut op_regs = String::new();
        let mut op_args = String::new();

        for i in 0..n {
            op_regs.push_str(&format!("in(\"{}\") arg{},", REGISTERS[i], i));
            op_args.push_str(&format!("arg{}: isize,", i));
        }

        let fun_name = format!("syscall{}", n);

        payload = format!(
            r#"
            {payload}
            #[inline(always)]
            unsafe fn {fun_name}(syscall_number: isize, {op_args}) -> isize {{
                let ret: isize;
                core::arch::asm! {{
                    "syscall",
                    {op_regs}
                    inout("rax") syscall_number => ret,
                }};
                ret
            }}    
        "#,
            payload = payload,
            fun_name = fun_name,
            op_args = op_args,
            op_regs = op_regs
        );
    }

    let mut macro_rule = String::new();

    for n in 0..=6 {
        let mut args = String::new();
        let mut values = String::new();
        for i in 0..=n {
            args.push_str(&format!("$arg{}: expr,", i));
            values.push_str(&format!("$arg{} as isize,", i));
        }
        args.pop();
        values.pop();
        macro_rule = format!(
            r#"
                {macro_rule}
                ({args}) => {{
                    syscall{n}({values})
                }};
            "#,
            macro_rule = macro_rule,
            args = args,
            values = values,
            n = n
        );
    }

    // Lets build the syscall! macro
    payload = format!(
        r#"
        {payload}
        macro_rules! syscall {{
            {macro_rule}
        }}
    "#,
        payload = payload,
        macro_rule = macro_rule
    );

    TokenStream::from_str(&payload).unwrap()
}
