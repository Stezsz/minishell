# Minishell – 42 School Project

The **minishell** project is a simple Unix shell implementation, designed to handle commands and execute them with basic functionality. It aims to develop a deeper understanding of how shells work and how they interact with the operating system. The challenge involves building a shell from scratch, supporting key features such as piping, redirection, and environment variable management.

---

## Main Objectives

- Implement a basic Unix shell using system calls.
- Handle command execution, environment variables, and redirection.
- Implement piping and process control.
- Ensure proper parsing of commands and their arguments.
- Implement built-in commands like `cd`, `echo`, `exit`, and `pwd`.
- Handle signals and error management.

---

## Key Features

1. **Command Parsing:**
   The shell reads the input, parses it, and identifies the command, its arguments, and any redirection or pipes.

2. **Command Execution:**
   Executes the parsed commands using `execve`, with support for built-in functions like `cd` and `echo`.

3. **Pipes and Redirection:**
   Implements piping (`|`) and input/output redirection (`>`, `<`).

4. **Environment Variables:**
   Manages and updates environment variables with `getenv`, `setenv`, and `unsetenv`.

5. **Signals Handling:**
   Properly handles `SIGINT` and `SIGQUIT` signals to ensure smooth operation.

6. **Error Handling:**
   Provides informative error messages and ensures robustness in handling invalid commands and arguments.

---

## Conclusion

The **minishell** project is an essential challenge that helps understand the inner workings of shells, the process management system, and how to handle user input and execute commands efficiently. It provides hands-on experience with core system calls and reinforces concepts like parsing, environment management, and signals.

---

> ✅ **Final Grade: 100/100**
> Project made at [42 Lisboa](https://www.42lisboa.com/pt/)
> 👤 Author: Stephan Rodrigues Lassaponari ([@Stezsz](https://github.com/Stezsz))
