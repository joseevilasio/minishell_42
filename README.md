<p align="center">
	<img src="https://img.shields.io/github/last-commit/marianaobmorais/minishell?color=%2312bab9&style=flat-square"/>
</p>

# minishell

#### Finished in: 2024-12-24

This project was done in partnership with [José Júnior](https://github.com/joseevilasio)

## About

This project objective is to create a simplified version of Bourne-Again Shell (Bash). It provides hands-on experience with key concepts in systems programming, such as:
- accurately parse and interpret user input, handling commands with pipes and redirections;
- implementing process creation, executing commands, and managing parent-child process communication;
- supporting input and output redirection and piping between two or more commands;
- handling signals to manage process termination and keyboard interrupts properly;
- manage memory dynamically, ensuring proper allocation and deallocation, avoiding memory leaks, and handling errors.

## Features
The mandatory part consists of:

- Handling `’` (single quotes) which should prevent the shell from interpreting the metacharacters in the quoted sequence.
- Handling `"` (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for `$` (dollar sign).
- Implementing pipes `|` and redirections for input `<`, output `>`, output in append mode `>>` and heredoc `<<`.
- Handling environment variables (`$` followed by a sequence of characters) which expand to their values; and `$?` which expands to the exit status of the most recently executed foreground pipeline.
- Handling the behavior of `ctrl-C`, `ctrl-D` and `ctrl-\` like in bash. And in interactive mode: `ctrl-C` displays a new prompt on a new line; `ctrl-D` exits the shell; `ctrl-\` does nothing.
- Implementing the builtins: `echo` with option -n; `cd` with only a relative or absolute path; `pwd` with no options; `export` with no options; `unset` with no options; `env` with no options or arguments; and `exit`.

And the bonus part consist of the additional:

- Handling the logic operators `&&` and `||` with parentheses for priorities.
- Wildcards `*` for the current working directory.

The minishell program does not interpret unclosed quotes, unclosed parentheses or any special characters that are not required by the subject such as `\` (backslash), `;` (semicolon) or `~` (tilde).

## Build

Clone the repository: 
```shell
git clone https://github.com/marianaobmorais/minishell.git
```
Enter the cloned directory:
```shell
cd minishell
```
Run `make` to compile the program:
```shell
make bonus
```
## Usage
Start the program:
```shell
./minishell
```
Insert input, like you would in bash:
```shell
ls -la
```
```shell
export VAR=test && echo $VAR
```
```shell
cd .. && pwd
```
## The Norm

This project is written in accordance to the 42 School's "The Norm" (_Version 4_). To review the Norm, [click here](https://github.com/42School/norminette/blob/master/pdf/en.norm.pdf).

## Contributions

If you find any issues or have suggestions for improvements, please feel free to open an issue or submit a pull request.

### Message to students

If you are searching resources to learn more about your own minishell, I encourage you to turn to your peers and the function manuals. Do not implement any code you do not understand and cannot code from scratch.
