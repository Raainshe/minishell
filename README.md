# Minishell

A robust shell implementation for 42 school that replicates many of the features found in bash.

## Features

- Command execution with path search (`ls`, `/bin/ls`)
- Command line history navigation
- Pipes (`|`) to connect commands
- Input/output redirections:
  - Input redirection (`<`)
  - Output redirection (`>`)
  - Append output redirection (`>>`)
  - Heredoc (`<<`)
- Environment variable expansion (`$USER`, `$PWD`, `$?`)
- Signal handling (Ctrl+C, Ctrl+D, Ctrl+\)
- Quote handling (single quotes `'` and double quotes `"`)
- Built-in commands:
  - `echo` - Display a message
  - `cd` - Change directory
  - `pwd` - Print working directory
  - `export` - Set environment variables
  - `unset` - Remove environment variables
  - `env` - Display environment variables
  - `exit` - Exit the shell

## Building

```
make
```

## Usage

After building the project:

```
./minishell
```

You'll be presented with a prompt where you can enter commands.

## Examples

```
minishell> ls -la
# Lists files in long format including hidden files

minishell> echo "Hello $USER"
# Prints Hello and your username

minishell> cat file.txt | grep "pattern" | wc -l
# Counts lines containing "pattern" in file.txt

minishell> cat << EOF > output.txt
# Start a heredoc to write multi-line content to output.txt

minishell> export NAME=value
# Set an environment variable

minishell> cd ~/Documents
# Change to Documents directory
```

## Authors

- ksinn - [GitHub](https://github.com/Fearcon14)
- rmakoni - [GitHub](https://github.com/raainshe)
