# Expression Evaluator - RPN Calculator

A comprehensive integer calculator that parses and evaluates mathematical expressions using Reverse Polish Notation (RPN). This project was developed as an Algorithms and Data Structures assignment to demonstrate proficiency in stack data structures, memory management, and expression parsing.



## Technologies and Constraints

1. **Language:** C++
2. **Data Structures:** Custom, dynamically allocated linked-list-based stack (no arrays used for the core stack to meet maximum requirements).
3. **Architecture constraints:** The project strictly avoids the C++ Standard Template Library (STL), including `<string>` and `<stack>`, relying solely on custom implementations.

## Implemented Features

The calculator processes multiple formulas provided in standard infix notation and executes the following core functionalities:

1. **Infix to Postfix Conversion:** Transforms expressions from standard mathematical notation (infix) to Reverse Polish Notation (postfix) on the fly, without storing the entire expression in memory.
2. **Custom Stack Implementation:** Utilizes a custom, pointer-based linked list to handle the stack operations (`push`, `pop`, `peek`) dynamically.
3. **Advanced Operator Handling:** Supports standard arithmetic operations (`+`, `-`, `*`, `/`) with precedence rules.
4. **Function Evaluation:** * Implements unary functions (e.g., `N` for negation).
    * Implements conditional logic (`IF(a, b, c)`).
    * Supports variadic functions (`MIN(...)`, `MAX(...)`) by dynamically counting and processing arguments during the conversion phase.
5. **State Visualization:** Prints the mathematical formula in postfix notation and displays the state (content) of the stack before every calculation step.
6. **Error Handling & Memory Management:** Detects and properly handles division by zero (outputting `ERROR`). Ensures zero memory leaks through rigorous deallocation of dynamically created nodes.

## Supported Operations (in ascending order of priority)

* **Arithmetic:** `a + b`, `a - b`
* **Multiplication/Division:** `a * b`, `a / b` (Integer division, rounded down).
* **Conditional:** `IF(a, b, c)` (If `a > 0` return `b`, else `c`).
* **Negation:** `N a` (Unary `-a`).
* **Variadic Functions:** `MIN(a1, a2, ...)`, `MAX(a1, a2, ...)`
* **Parentheses:** `(...)`

## Setup and Compilation

To compile and run this calculator, you need a standard C++ compiler (like GCC or Clang).

1. Clone the repository to your local machine.
2. Navigate to the project directory in your terminal.
3. Compile the source code using the command: 
   `g++ main.cpp -o calculator -Wall -Wextra`
4. Run the application and provide the input via standard input (or pipe a file): 
   `./calculator < input.txt`
