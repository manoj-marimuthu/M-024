## REPL

- M-024 supports a REPL (read-eval-print-loop) environment for language exploration
and testing code quickly when you have an idea.

- The build instructions for the REPL are given in the readme.md in the main section.

- The repl files are inside the ```repl``` directory and you do no need to build the
interpreter for the repl since both are independent.

## repl directory and its structure

- inside the ```repl``` directory, there are 3 import objects excluding the Makefile
    1) ```build``` directory
    2) ```__repl__.mscf``` file
    3) ```repl.c``` file

- The ```build``` directory as you already know contains the object files and must be there before 
running the Makefile or else you get a build error.
- The ```repl.c``` file contains the main function that changes the logic that the interpreter uses
by using an infinite loop that accepts strings, which are then processed.

- ```__repl__.mscf``` is the file that the repl uses to write the strings from the input taken by
```repl.c```, so that it simulates the same process which we do in the interpreter with our own file.

- ```repl.c``` uses ```fgets()``` to receive user input, the writes it to the ```__repl__.mscf``` file and
then calls the ```interpret()``` function with the parameter as ```__repl__.mscf```. This successfully leads to the execution of the given code. But we also need to reset the global context variables (the ones listed in ```include/globals.h```) after every execution to prevent undefined behaviour.

- As you can say, this process has a high time complexity because we have to open the file, use the file pointer to write a line into the file and then close the file, call ```interpret()``` function, reset globals for each line of code that you execute.

- Hence this a main todo item for the next patch or version.

## Differences from Python's REPL

- use ```print()``` for things you wanna see, as it does not support auto printing like python's repl.
For example in python's REPL you can run

```python
>>> 1  + 2
3
```

- But for M-024, this will result in an invalid syntax and hence an error. So do this,

```python
>>> print(1 + 2)
3
```

- Errors lead to REPL termination (I know, a bummer)
- Only single line statements for now, such as print statements,
variable and constant declarations etc... (Again, I know, a bummer).

Due to these annoying inconsistencies this a top priority for upcoming
patch. 