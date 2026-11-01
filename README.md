# Library Management System — C++ OOP

A beginner-friendly console mini-project built with object-oriented programming in C++.

## Features
- Add and list books
- Search books by title or author
- Register and list library members
- Issue books to registered members
- Return issued books
- Save book/member records in local text files (`books.txt`, `members.txt`)

## OOP concepts demonstrated
- **Classes and objects:** `Book`, `Member`, and `Library`
- **Encapsulation:** class data is private and accessed through public methods
- **Constructors:** initialize book and member objects
- **Abstraction:** `Library` provides operations without exposing storage details
- **Composition:** `Library` manages collections of `Book` and `Member` objects

## Compile and run
Requires a C++17 compiler.

### Windows (MinGW g++)
```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o library.exe
library.exe
```

### Linux / macOS
```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o library
./library
```

## GitHub note
GitHub is a code-hosting platform and can compile this project automatically using the included GitHub Actions workflow. GitHub Pages cannot run native C++ console applications in a web browser. The workflow verifies that the source compiles; to interact with the program, download/clone the repository and run it with a C++ compiler, or use a C++ development environment.

The Actions workflow builds the program on each push and stores the compiled executable as a downloadable artifact for a limited time.
