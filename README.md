# Interactive Terminal

Interactive Terminal is a custom Unix shell developed in C, designed to provide a simplified interface for command-line navigation and operations. It's aimed at those seeking an intuitive and streamlined experience in managing directories, files, and processes directly from the terminal.

## Features

- Simplified command-line interface for ease of use.
- Execution of essential built-in commands like `cd` for directory changes.
- Capability to run external system commands, extending its utility.
- Input and output redirection (`<`, `>`, `>>`) to control command data flow.
- Support for command chaining through piping (`|`).
- Error handling mechanisms to assist with command corrections and understanding.

## Getting Started

These instructions will help you get a copy of Interactive Terminal running on your system for development and usage.

### Prerequisites

- A Linux environment or similar Unix-based operating system.
- The GCC compiler for building the project from source.

### Installation

1. Clone the Interactive Terminal project:
   ```bash
   git clone https://github.com/yourusername/InteractiveTerminal.git
   ```
2. Navigate into the project directory:
   ```bash
   cd InteractiveTerminal
   ```
3. Compile the shell:
   ```bash
   make
   ```
4. Start the shell:
   ```bash
   ./InteractiveTerminal
   ```

## Usage

Upon starting Interactive Terminal, you will be greeted with a clean command prompt. From here, you can type commands and interact with the system:

- Change directories:
  ```
  [InteractiveTerminal current_directory]$ cd /desired/path
  ```
- List directory contents:
  ```
  [InteractiveTerminal current_directory]$ ls
  ```
- Redirect output to a file:
  ```
  [InteractiveTerminal current_directory]$ ls > filelist.txt
  ```
- Utilize pipes for combined commands:
  ```
  [InteractiveTerminal current_directory]$ cat largefile.txt | less
  ```

To exit the terminal, type:
```
[InteractiveTerminal current_directory]$ exit
```

## Contributing

Your contributions are welcome! Please refer to the contributing guidelines for more information.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgments

- Special thanks to everyone who contributed to the development and testing of Interactive Terminal.
- Credit to all the open-source resources and documentation that made this project possible.

---

Ensure to replace `yourusername` with your GitHub username and customize the installation and usage instructions according to your project's actual setup and requirements.
