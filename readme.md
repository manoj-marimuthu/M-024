## M-024 Programming Language
### Current Version: v0.4.0 • Under Active Development
A lightweight interpreted general purpose programming language, written in C, designed 
for rapid scripting, testing and simplicity.

Here is an example program for the bubble sort algorithm in M-024.

```python title="sort.mscf"

# bubble sort
arr = []
const n = int(input("Enter array size:")) # constants

for i in <0,n>:
    num = int(input("Element " + str(i) + ": "))
    arr = arr + num # list appending

for i in <0,n>:
    for j in <0,n-i-1>:
        if arr[j] > arr[j+1]:
            temp = arr[j]
            arr[j] = arr[j+1]
            arr[j+1] = temp

print("Sorted array:",arr)
```
### Why M-024 ?

- M-024 started as personal project for learning and soon evolved into a usable and
practical language for various tasks. 

- Lightweight (< 1MB)
- Can be used for scripting, learning interpreters and to try ideas.

### Features
- strings, numbers, booleans, list and ```None```.
- variables and constants
- IO functions such as ```print()``` and ```input()```
- Functions with recursion
- Arithmetic operations (+,-,*,/,^,%)
- Logical operations (```and```,```not```,```in```,```or```)
- Relational operations (==,!=,>,<,>=,<=)
- Built-in functions like ```len()```,```type()```
- loops (```for``` and ```while```)
- list and list operations
- import system (```mount```)
- stdlib
- And so much more

### Installation

- M-024 can be built using GCC or other c compilers in different Operating systems. Its development was primarily in the Windows operating system but the makefile is optimised to work on other operating systems like Linux.

- Clone the repository
```bash
git clone https://www.github.com/manoj-marimuthu/M-024.git
```
- run the makefile
```bash
make
```
- be aware that the command might change (for example - ```mingw32-make``` for mingw32)
- Check if build was successful. Try:
```bash
./M024 --version
```
- The above command is for Powershell. For CMD, run ```M024.exe <filename>```.

- To build the REPL, first change directory using
```bash
cd repl
```
- Then run the makefile for the repl
```bash
make
```
- be aware that the command might change (for example - ```mingw32-make``` for mingw32)
- Then run the executable using:
```bash
./repl
```
- for cmd, just run ```repl.exe```.
- *Note : You do not need to build the interpreter for the REPL. They are independant of each other.*

### Current Status

Iam actively working on it to add a set of unique features (inspired from ```UNIX```) and to optimize its
performance by incorporting Stack-Based Virtual Machine for executing bytecode instructions. However, These
might take some more versions and the ones that are gonna be released next will primarily be of new feature addition (one final version for feature addition..for the time being) and improving modularity.

### Contributing
 
Simply open a Pull request and I will merge it if it is necessary and correct.

### Testing
- For contributors testing is crucial.Use the batch script to run all tests if you are in windows. Example in cmd:
```bash
run_tests
```
- There is also a powershell script provided. Otherwise I suggest you to run tests for the specific feature you worked on or the features that are affected by it since the test folder contains tests for many features making it hard to infer what you need. Example - suppose you work on strings, you could just do
```bash
./M024 "test/test_strings.mscf"
```

- I have followed a general convention of test_component.mscf for writing tests for any component. All test files require the ```tester.mscf``` standard library.

### Documentation

Documentation is available in the docs directory and it is under work.

### Final Thoughts

To learn the language, I suggest you to go through the documentation inside the docs folder and all the documentations are in markdown, so it is better to stick with github to reading. Contributors, Bug reports and any random thoughts on M-024 are welcome. I built M-024 for fast coding (in terms of writing code and not in terms of actual performance speed, that would be ironic), reducing long syntax struggles and of course for learning about interpreters and their design.

*Happy Hacking !*
