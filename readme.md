# Custom Interpreter Version : v0.3.0
## Table of Contents
- [What ?](#what-)
- [Why ?](#why-)
- [How ?](#how-)
- [Setup](#setup)
- [Documentation](#documentation)
  - [Comments](#comments)
  - [Data Types](#datatypes)
  - [Variables And Constants](#variables-and-constants)
  - [Arithmetic Operations](#arithmetic-operations)
    - [String Operations](#string-operations)
  - [Logical Operations](#logical-Operations)
  - [Comparators](#comparators)
  - [IO Statements](#io-statements)
    - [print()](#print)
    - [input()](#input)
  - [Built-in and Type-Conversion Functions](#built-in-and-type-conversion-functions)
  - [Conditional Statements](#conditional-statements)
  - [Loops](#loops)
  - [Functions](#functions)
  - [Imports and stdlib](#imports-and-stdlib)
  - [Errors](#errors)
- [Final Thoughts](#final-thoughts)
- [Author](#author)
# What ?
This repository holds the codebase of a custom interpreter written from scratch in C.

# Why ? 
This interpreter acts as a great educational tool for programming languages and runtime designers.
It can also act as a language that is fast to setup, great for testing your programming ideas quickly since it
has a neat and modern syntax. However it does have bugs and provides lesser features than modern production interpreters
that limit the ability of the user to ideate and work quickly. Iam actively working on solving bugs,expanding features
and to provide libraries.

# How ?
The project uses the classic interpreter architecture with a lexer and a recursive parser that produces an AST (abstract syntax tree).
The AST is then recursively evaluated (tree walker) and executed. Memory is handled by a Pool that holds all allocations from LexerNodes,
ASTnodes,String allocations,Variables,Functions etc. It automatically frees memory on program end ensuring no memory leak occurs. Variables and Functions
are stored in a Hashmap (chains on collisions) for faster lookups during the evaluation of the AST. It also supports Indentation for readability and 
uses a stack for tracking the indentation.

# Setup
- Clone the Repo Using Git or Download Manually
- The repo consists of a Makefile which can be run using the command
```fish
  make
```
- An executable is now produced , which can be used to run ".app" files. Example - To run a file called "main.app" , run
```fish
./interpret "main.app"
```
- A test set is provided for development purposes (to identify features from breaking) after making code changes
- to run the test set in windows (cmd script here, use powershell commands manually if needed)
```bash
run_tests
```
- The test files are provided under the test/ folder and can be utilissed manually too for a different OS (UNIX-based) or purpose (testing only certain features)

# Documentation
This section provides the documentation for all the features supported by this interpreter version. It is advised to go through
the documentation before programming since it has similar syntax to other languages like python that can make it confusing.

## Comments
```python
# Comments Can be Made using a '#'
```
## Datatypes
1. Numbers (integers,floats)
2. Strings
3. Booleans
4. None
## Variables And Constants
- To create a variable , use its name followed by '=' and its value.
```python
name = "Interpreter"
version = 0.2
isItAwesome = True
```
- To create a constant , use the 'const' keyword followed by the name,'=' and its value.
```python
const pi = 3.1428
```
## Arithmetic Operations
- +,-,*,/,% and ^ for powers.Example - 
```python
a = 30 + 10*5 - (100/5) # 60 
b = 10 % 3 # 1
c = 2 ^ 3 # 8
```
### String Operations
- Strings can be concatenated, replicated and indexed.
```python
print("Interpreter" + " Docs") # "Interpreter Docs"
print("Docs" * 3) # "DocsDocsDocs"
print("Docs"[0]) # "D"
```
## Logical Operations
- supports and,or and not.
```python
a = True and False # False
b = not False # True
c = not a # True
```
## comparators
- The interpreter supports various comparators such as ==,!=,>,<,>=,<= .
```python
print(1 != True) # True (since True = 1)
print(1 < 0) # False
print("Interpreter" == "is not Fun") # False
```
## IO statements
### print()
- To print contents on the terminal, use the built-in print() function. The print() function takes any number of parameters and prints them 
in the same line with spaces in between with the newline escape sequence.

```python
print("Hello World") # Hello World
print(1 + 2) # 3
print("Pi is",3.1428) # Pi is 3.1428
```

### input()
- to get user input from the terminal, use the built-in input() function.
```python
a = input("Enter Your Name:")
```
## Built-in and Type-Conversion Functions
- The interpreter provides 2 major built-in utility functions len() and type()
```python
a = len("I Love Interpreters !") # 21
b = type(a) # 'NUMBER'
```
- The interpreter returns uppercase strings for each type (NUMBER,STRING,NONE,BOOLEAN)
- The len() function only accepts strings, else raises a runtime error. 
- The interpreter also provides type conversion functions such as int(),str(),bool()
```python
a = int("20") # 20
b = str(True) # "True"
c = bool(0) # False
d = str(2) # "2"
```
- feel free to experiment with different combinations and functions
## Conditional Statements
- Supports if,elif,else,nested ifs.Every Statement under these must be indented appropriately.
```python
a = 10
b = 20
if a > b:
  print(a,"is larger than",b)
elif a < b:
  print(a,"is smaller than",b)
else:
  print(a,"is equal to",b) 
```
## Loops
- The interpreter supports while loop, for loop and nested loops.Every Statement under these must be indented appropriately. The for loop feature is currently under construction for further development 
due to poor code clarity and to prevent bugs.
```python
i = 1
while i < 10:
  print(i * "*")
  i = i + 1
```
- Example for loop:
```python
for i in <1,10,2>: # this print numbers 1,3,5,7,9
  print(i)
```
- The <start,end,skip> syntax provides a neat way to set loop ranges.
- for loops also support string traversal by using the 'in' operator
```python
for char in 'hello world':
  print(char)
```
## Functions
- The latest Feature in this version are functions that can accept parameters and can return values. Recursion is now supported.
```python
fn sum(a,b):
  return a+b
```
- Function call can be made like:
```python
print(sum(10,20)) # 30
c = sum(0,10) # 10
```
- Functions in this language also support complex recursive patterns and problems. Example -
```python
# Tower of Hanoi using Recursion

fn hanoi(n,src,aux,dest):
    if n == 1:
        print("Move Disk 1 from",src,"to",dest)
        return None
    hanoi(n-1,src,dest,aux)
    print("Move Disk",n,"from",src,"to",dest)
    hanoi(n-1,aux,src,dest)

hanoi(3,'A','B','C')
```
- Check out the test folder to try functions and recursion.
## Imports and stdlib
- The interpreter as of v0.3.0 now supports imports.
- imports can be done using the "load" keyword followed by the filename/path.
- The filepath is relative to the interpreter's exe file and not that of the loader.
```python
load "lib/math.py"
```
- The current standard library is limited but the upcoming versions and patches will improve them as side quests
- Standard libraries
  - math.app
    - mathematics utility library
    - Constants
      - pi
      - e
      - tau
    - functions
      - isEven(n)
      - isOdd(n)
      - floor(n)
      - ceil(n)
      - factorial(n)
      - dist(x1,y1,x2,y2)
      - abs(n)
      - gcd(a,b)
      - lcm(a,b)
      - copysign(a,b)
      - rad(deg)
      - deg(rad)
  - tester.app 
    - testing library for basic unit testing
    - functions
      - testEqual(a,b,test_name)
      - testNotEqual(a,b,test_name)
      - testTrue(a,test_name)
      - testFalse(a,test_name)
  
## Errors
- The interpreter does not support error/exception handling features...yet but it does have a neat error system that provides error type,line at
   which the error happens ,hints and Even Error types. The following are the error types in the interpreter.
  
| Error Type | Why ? | What to do ? |
|------------|-------|--------------|
| SYNTAX_ERROR | Incorrect Syntax Usage | Refer Docs |
| DIVISION_BY_ZERO_ERROR | Division by zero | add checks / remove manual /0 |
| COMPILE_TIME_ERROR | Possibly Memory Allocation Error | Rebuild the interpreter or DM me the program |
| FILE_NOT_FOUND_ERROR | Occurs when you pass a filename that does not exist for interpretation | check filename or rename existing file |
| FILE_ARG_ERROR | Occurs when you pass multiple arguments in the terminal instead of just filename or --version | remove unnecessary arguments |
| FILE_EXTENSION_ERROR | Occurs when you pass a filename that is not of '.app' extension | change file extension to '.app' |
| INDENTATION_ERROR | Occurs when there are wrong or unnecessary indentations in blocks or main program | check indentations, specially inside blocks. Make sure all main program statements are not indented |
| RUN_TIME_ERROR | Occurs when there is a runtime error like passing a string to int() etc | check program logic, add checks using the line number given by the interpreter |
| FUNCTION_ERROR | Occurs when there are function related errors like reusing function names, empty functions or wrong parameters | Use the line number to identify the exact error and debug |
| IMPORT_ERROR | Occurs if deadlocks exists between imports | Prevent deadlocks , do not repeat imports |
# Future Plans
- Adding call stacks and supporting recursion for functions
- Building an import system and a standard library
- Providing data structures such as lists and hashmaps
- Possibly adding Garbage Collection (reference counting/mark and sweep GCs)
- VM bytecode extension 

# Final Thoughts
This interpreter started as an educational project , to teach myself how interpreters work but over time , my goals have changed. The initial question was "How do interpreters work ?" and then it became "Can I write one ?" and currently it is "Can I make it powerful enough to write real world projects ?". My vision is to make this powerful enough to be the backbone of real world projects such as college assignments and projects,hackathon apps or even prototypes since I believe building an interpreter is just one task but using it is another. Contributions and Bug reporting are welcome.

# Author
Manoj K M   
Written With C and Curiosity

