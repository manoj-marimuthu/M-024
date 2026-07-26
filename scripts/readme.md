# M-024 Programming Language

The following documentation is made to guide you through
the c files and their purpose for the interpreter core. 
The Project has the following files:

### callStack.c

- This file forms the base for functions , function calls 
  and recursion.
- It holds a global variable ```call_stack``` that stores
  all function frames during function call
- By default it holds a global frame, which is when no function
  is called and the execution is happening in the main program.
  It is pushed by ```interpret.c```.

### error.c

- This file provides the ```error()``` function which uses all the error
  types from error.h and provides a error raising function which 
  prints error type, line count and even hints.
- This file would eventually be used for advanced error handling.

### evaluate.c

- This file provides the tree-walker, which evaluates the AST nodes
  recursively and executes them.
- It uses the parser to obtain the AST, uses the evaluate() function
  for actual execution and the ```evaluate()``` function is called by the
  ```Execute()``` function.

### functionRegistry.c

- This file holds the logic for creating functional object (which store
  function names, statements under the function, its local scope).
- Uses ```setFunction()``` and ```getFunction()``` functions to handle functionArr[].
  Uses hashmaps for speed.

### globals.c

- This file helps in the manipulation of global variables (as defined in ```globals.h```),
  and handling of imports without global variables getting messed up.
- It provides a functions ```copy_state()```, ```clear_state()```, ```restore_state()```.

### imports.c

- This file provides the necessary logic for the import system.
- It uses two linked-lists loading and loaded to handle deadlocks in imports and
  other than that provides a list of functions to manipulate the two linked-lists.

### indentation.c

- This file provides the necessary logic for indendation tracking in the lexer.
- It uses a stack (implemented using a linked-list) and provides utility functions
  for pushing, popping, peeking and traversing (which I constantly used to debug).

### lexer.c

- This file provides the necessary logic for tokenizing the target file
- It handles the indentation logic, Lexer nodes and builds a linked list
  of tokens for parsing.

### memoryHandler.c

- This files acts as a basic dumb garbage collector.
- It tracks all allocations from strings, lexer nodes, ast nodes etc
  and stores them in a linkedlist
- When the program execution finishes or gets interrupted, all memory is
  freed to prevent memory leaks.
- Eventually this file would form the base for writing a GC such as a mark-sweep
  or reference counting GC.

### parser.c

- This file provides the parsing logic (recursive descent) and constructs the AST
  using the lexer output.
- It uses a set of functions defined in ```parser.h``` and runs them using the ```Parser()```
  function at the bottom of the file.

### variableRegistry.c

- This file, as the name suggests handles variables and constants.
- It uses a hashmap to store variables and their data (value, name, whether it is
  constant etc) for speed.
- uses the ```getVariable()``` and ```setVariable()``` functions on the variables created by
  ```createVariable()``` function.

The documentation for this section is limited and will be improved in upcoming versions
when I have time. If you are exploring the codebase, I suggest you to follow the order starting from   error.c, variableRegistry.c, functionRegistry.c followed by indentation.c, lexer.c, parser.c, callStack.c evaluate.c. Then you can go through imports.c and globals.c.