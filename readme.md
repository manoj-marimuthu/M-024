# Custom Programming Language 
It is simple interpreter written from scratch in C. It is the first version of the project and has only limited features.

# Features
1) Interprets .py files and runs them
2) Supports Integers,Strings,Booleans,Floats
3) Supports Variable Declaration
4) Constants are supported (with const keyword before variable name)
5) Basic Arithmetic Operations - +,-,*,/
6) Basic Logical Operations and,not,or
7) printing of variables and strings (Does not support , like python)
8) printc() prints to the terminal without newline character at the end
9) print() prints to the terminal with addition of newline character at the end
10) both printc() and print() take argument of a single expression type (Integers,Floats,Boolean,Strings)
11) import other .py files using import <filename> keyword , searches files in the same project directory
12) Memory Tracker to manage memory

# Installation and Usage
1) download interpret.exe from bin folder of this repository
2) Paste it in your project folder
3) run the command interpret <filepath> in your terminal (make sure the file is in the same directory as interpret.exe)
4) For your guide, Some Basic Libraries and Files are shared in src folder of this repository
5) All imports must stay in the same directory as interpreter.exe

# Common Errors and Tips
1) print("Name:" + name) is fine but print("Age:" + 18) causes error as the expression consists both string and int. Use separate prints.
2) Imports never work until you place them in the same directory as interpret.exe ,because the interpreter searches imports in its own directory even though the main file is in some other directory.
3) suggested file structure: keep all the files from interpret.exe,imports and the main python file in the same directory without sub folders. This will be corrected in version 0.0.1.

# Future Plans for Version 0.0.1 
1) Addition of more features such as conditionals,loops and even functions with recursion.
2) Better Memory Management,Import Rules

   
