## Comments

- Comments in M-024 can be made using the # symbol
- It supports comments on both a newline and a line with some code
- Multi-line comments are currently under work and will soon be available
- Example:
```python
# This is a comment
x = 20 # This is a comment
```
- Comments are handled during the lexer stage, the tokenizer skips them 
automatically. Refer the ```scripts/lexer.c``` file to know more about their
c implementation.
## Variables and Constants

These are value storing entities and are handled by a hashmap in ```scripts/variableRegistry.c```
for O(1) access at best case (could be more than that since the hashmap using chaining to
handle collisions).

### Variables

- Mutable value storing entities that are defined using an equal (=) operator where the lhs 
represents the name of the variable and the rhs represents the value (could be an atomic value
or an expression).
- Example-
```python
x = 20
name = "Manoj"
isInterpreterAwesome = True
```

### Constants
- Immutable value storing entities that are defined in the same except that the ```const``` keyword
is used to differentiate between them.
- Example-
```python
const ageToVote = 18
```
### Naming Convention
- The variables and constants cannot take any name and follow a convention that is defined as:
    - If you know python's naming convention, skip to the last point.
    - Variable names must be identifiers and not anything else
    ```python
    docs = "boring" # allowed
    "boring" = docs # not allowed
    ```
    - No special symbols except the underscore (_) is allowed.
    ```python
    three_zeroes = "000" # allowed
    three!zeroes = "000" # not allowed
    ```
    - Underscores cannot begin the variable name.
    ```python
    ex_ample = 10 # allowed
    _example = 10 # not allowed
    ```
## Datatypes

- M-024 currently supports 4 datatypes namely
    - NUMBER
    - STRING
    - BOOLEAN
    - NONE
- Any integer regardless of the sign or decimals come under the
```NUMBER``` type and a separate data type for floats is now
on going.
- Any String of any length (single or multiple characters) come under
the ```STRING``` type. M-024 does not have a dedicated type for 
characters like C or Java.
- ```True```, ```False``` values come under the ```BOOLEAN``` type and ```True``` is usually interpreted as 1 and ```False``` as 0. 
- ```None``` is a special datatype that denotes nothing, it is similar to Python's None datatype. 
- Datatypes are usually set during the evaluating stage and you can see them represented as
c enums in ```include/value.h```.

## Type conversion

- Since M-024 is dynamically typed, type conversion is
an default feature in its design.

<table>
    <tr>
        <td>function</td>
        <td>number</td>
        <td>string</td>
        <td>bool</td>
        <td>None</td>
        <td>list</td>
    </tr>
    <tr>
        <td>str()</td>
        <td>number to string conversion (1 -> '1')</td>
        <td>returns the same object</td>
        <td>"True" or "False"</td>
        <td>"None"</td>
        <td>under work</td>
    </tr>
    <tr>
        <td>int()</td>
        <td>returns number</td>
        <td>error</td>
        <td>1 for True, 0 for False</td>
        <td>error</td>
        <td>under work</td>
    </tr>
    <tr>
        <td>bool()</td>
        <td>True if n != 0, else False</td>
        <td>error</td>
        <td>returns the same object</td>
        <td>error</td>
        <td>under work</td>
    </tr>
</table>

## type()

- Used to identify an object's type
- receives only one argument and returns its type
as a string with the type name in uppercase.

<table>
    <tr>
        <td>object type</td>
        <td>what it returns ?</td>
    </tr>
    <tr>
        <td>string</td>
        <td>"STRING"</td>
    </tr>
    <tr>
        <td>number (integer / float)</td>
        <td>"NUMBER"</td>
    </tr>
    <tr>
        <td>boolean</td>
        <td>"BOOLEAN"</td>
    </tr>
    <tr>
        <td>None</td>
        <td>"NONE"</td>
    </tr>
    <tr>
        <td>list</td>
        <td>"LIST"</td>
    </tr>
</table>

## IO

- M-024 supports IO operations only to the terminal via the
```input()``` and ```print()``` functions.

### print()

- A built-in function (C-based, fast) which is used to print to
the terminal.
- Takes any number of arguments and any type of arguments
- prints a default space character in between each argument
provided
- example-
```python
print("Language Name:","M-024") # Language Name: M-024
```

### input()

- A built-in function (C-based, fast) which is used to get input from
the terminal. Only takes a single line per call.
- Takes a string argument which is displayed while asking input.
- example-
```python
input("Type Something:") # Type Something:<reads characters>
```

## Strings

- Strings are a cool feature in M-024. Since it is interpreted
and simple, string task are much easier.

### String indexing

- Strings can be indexed starting from index 0 to n-1 where n
represents the length of the string and n-1 is the final character's
index. example -

```python
s = "Hello"[0]
print(s) # H
```

- Negative indexes are also supported and is similar to python.
example - 

```python
s = "Hello"
print(s[-1]) # o
```

### String operations

- Strings can be concatenated and replicated.
example
```python
i = "Hello"
j = i + " World" # Hello World
k = i * 2 # HelloHello  
```

## List

```list``` is a new feature as of v0.4.0 and are very similar
to lists in python. It currently supports list and nested list creation,
indexing and nested indexing, list modification at an index, and push and
pop operations.

### creating lists

Use square brackets to initialize a list as written below.

```python
const x = [1,2,"hi"]
```

### indexing a list

- A list can be indexed with an identifier or a list directly. It behaves
like an object but internally it is not and rather handled by the interpreter
for specific cases.

```python
x = [1,2,3]
print(x[2]) # 3
print([1,2][0]) # 0 , behaves like an object
```

### Modifying list

Lists can be modified by either changing value at
an index or by pushing / popping.

```
x = [1,2,3]
x[0] = 100
print(x) # [100,2,3]
x = x + 20 
print(x) # [100,2,3,20]
y = pop x
print(x) # [100,2,3]
print(y) = 20
```
## Operations

1) Arithemtic operations
2) Logical operations
3) relational operations

### Arithemtic operations

<table>
    <tr>
        <td>Operator</td>
        <td>What it does?</td>
    </tr>
    <tr>
        <td>+</td>
        <td>Binary addition / unary sign (like +1)</td>
    </tr>
    <tr>
        <td>-</td>
        <td>Binary subtraction / unary sign (like -1)</td>
    </tr>
    <tr>
        <td>*</td>
        <td>Binary multiplication</td>
    </tr>
    <tr>
        <td>/</td>
        <td>Binary division</td>
    </tr>
    <tr>
        <td>^</td>
        <td>Binary exponentiation</td>
    </tr>
    <tr>
        <td>%</td>
        <td>Binary modulus (returns remainder)</td>
    </tr>
</table>

### Logical operations

<table>
    <tr>
        <td>Operator</td>
        <td>what it does ?</td>
    </tr>
    <tr>
        <td>and</td>
        <td>logical ```and``` operation (like && in C)</td>
    </tr>
    <tr>
        <td>or</td>
        <td>logical ```or``` operation (like || in C)</td>
    </tr>
    <tr>
        <td>not</td>
        <td>Logical ```not``` operation (like ! in C)</td>
    </tr>
    <tr>
        <td>in</td>
        <td>Checks presence of character in string (list-implementation upcoming in v0.4.1 patch)</td>
    </tr>
</table>

### Relational operations

<table>
    <tr>
        <td>Operator</td>
        <td>what it does ?</td>
    </tr>
    <tr>
        <td>></td>
        <td>greater than</td>
    </tr>
    <tr>
        <td><</td>
        <td>less than</td>
    </tr>
    <tr>
        <td>>=</td>
        <td>greater than or equal to</td>
    </tr>
    <tr>
        <td><=</td>
        <td>lesser than or equal to</td>
    </tr>
    <tr>
        <td>==</td>
        <td>equality check</td>
    </tr>
    <tr>
        <td>!=</td>
        <td>non-equality check</td>
    </tr>
</table>
  
## Conditional Statements

- M-024 currently supports ```if```, ```elif``` and ```else``` to
write conditional statments. The ```switch``` statement is also
under work but is not in the latest version.

- Example snippet -

```python
if False:
    print("Unexpected")
elif True:
    print("Expected")
elif 10 > 1:
    print("Unreachable")
else:
    print("Unexpected pro max")

# the above snippet prints "Expected", if not
# we are having serious problem with this feature
```

## Loops

- Looping statements in M-024 are written using ```while```
and ```for``` keywords.
- Infinite loops are possible and an idea of controlling maximum
loop capacity and recursion for a program is under consideration.

### while

- ```while``` loops run statements until the given condition is 
False and it is very similar to while loops in python. Example -

```python
while True:
    print("Boom!")
# Hope you don't do the above
```
### for

```for``` loops in M-024 are a little different from other
programming languages.  There are 2 types of for loops in M-024.

1) range loops

- These loops run under a given range. Can count backwords too.
Use the angular-bracket syntax for writing a ranged for loop as
written below.

- ```<start,end,skip>``` is the choosen syntax. (similar to Python's range())

```python
# loop pattern = 1 -> 3 -> 5 -> 7 -> 9
for i in <1,10,2>: 
    s = s + i

# loop pattern = 10 -> 8 -> 6 -> 4 -> 2
for i in <10,1,-2>:
    s = s + i
```

2) Looping a string/ identifier containing a string

- As the title suggests, looping a string is possible.
Example -

```python
# traversing characters of a string
for char in "Hello":
    print(char)

const feature = "cool"

# traversing characters of an identifier holding a string

for letter in feature:
    print(letter)
```
- The ```for``` loop feature for the ```list``` data type is
actively under development and will be released along with the
upcoming M-024 patch.

- Note that a while loop can still traverse a list in case you need
it.
## Functions

a M-024 function is created using the ```fn``` keyword.
example -

```python
fn add(a,b):
    return a+b
```

```return``` is supported and the interpreter maintains scope
by using callstacks internally. So variables created inside a 
function die instantly after function ends. The order of access
for variables and constants is to first check the local scope and
then the outer scopes and finally the global scope (similar to Python).

Recursion is also supported. To try it, Checkout the examples directory.

```python
# example recursive function

fn factorial(n):
    if n == 0 or n == 1:
        return 1
    return n * factorial(n-1)
```

Remember that indentation is crucial and must be maintained for both syntax
correctness and readability.
## len()

- Returns the length of object based on datatype.
- Raises an error for any datatype other than string
and list.
- Example - 
```python
x = len("Hi") # 2
x = len([1,2,3]) # 3
```
## Imports

- Imports just like any other programming language, lets developers
to incldue contents of another file.

- In the case of M-024, imports are done using the ```mount``` keyword.
For standard libraries, use the direct name but for other files, use a string
representation of the file name
Example -

1) For stdlib files

```python
mount math
```
2) For other normal files (typically outside ```/lib```)

```python
mount "MyFile.mscf"
```

- It currently does not support ```as``` or ```from``` like python and it
is something for the future versions.

- It has one important distinction. It pollutes the current file's function and
variable registry. For example, say we have two files ```A.mscf``` and ```B.mscf```.

- File contents of ```A.mscf``` :

```python
x = "Iam from A.mscf"
```

- File contents of ```B.mscf``` :

```python
x = "Iam from B.mscf"
mount "A.mscf"
print(x)
```

- The output of the snippet above will print "Iam from A.mscf"
onto the terminal. It does not have a object like syntax similar to
python and is more like an include from C language than the import
from the python language hence the name ```mount``` (a common ground).

## math.mscf

- The first standard library in M-024, that holds the following functions:

<table>
    <tr>
        <td>Function / Variable Name</td>
        <td>What it does ?</td>
    </tr>
    <tr>
        <td>pi</td>
        <td>constant containing pi value</td>
    </tr>
    <tr>
        <td>e</td>
        <td>constant containing e value</td>
    </tr>
    <tr>
        <td>tau</td>
        <td>contant containing tau value</td>
    </tr>
    <tr>
        <td>isOdd(n)</td>
        <td>returns True if n is odd, else returns False</td>
    </tr>
    <tr>
        <td>isEven(n)</td>
        <td>returns True if n is even, else returns False</td>
    </tr>
    <tr>
        <td>floor(n)</td>
        <td>returns the floor value of n (ex - floor(1.2) = 1)</td>
    </tr>
    <tr>
        <td>ceil(n)</td>
        <td>returns the ceil value of n (ex - ceil(1.2) = 2)</td>
    </tr>
    <tr>
        <td>factorial(n)</td>
        <td>returns the factorial of n (ex - factorial(5) = 120)</td>
    </tr>
     <tr>
        <td>dist(x1,y1,x2,y2)</td>
        <td>returns the euclidean distance between the points (x1,y1) and (x2,y2)</td>
    </tr>
    <tr>
        <td>abs(n)</td>
        <td>returns the absolute value of n (ex - abs(-5) = 5)</td>
    </tr>
    <tr>
        <td>exp(n)</td>
        <td>returns e^(n)</td>
    </tr>
    <tr>
        <td>gcd(a,b)</td>
        <td>returns the greatest common divisor of a and b</td>
    </tr>
    <tr>
        <td>lcm(a,b)</td>
        <td>returns the least common multiple of a and b</td>
    </tr>
    <tr>
        <td>copysign(a,b)</td>
        <td>returns a but with the sign of b</td>
    </tr>
    <tr>
        <td>rad(deg)</td>
        <td>converts degrees into radians and returns it</td>
    </tr>
    <tr>
        <td>deg(rad)</td>
        <td>converts radians into degrees and returns it</td>
    </tr>
</table>

## tester.mscf

- The standard testing library used primarily in the test suite
for this repository and is built for anyone who instantly needs a library
to write tests. It contains the following functions:

<table>
    <tr>
        <td>Function name</td>
        <td>What it does ?</td>
    </tr>
    <tr>
        <td>testEqual(a,b,test_name)</td>
        <td>prints success message if a == b else prints the failure message</td>
    </tr>
    <tr>
        <td>testAlmostEqual(a,b,test_name)</td>
        <td>prints the success message if a == b or if |a - b| < 0.01 else prints the failure message</td>
    </tr>
    <tr>
        <td>testNotEqual</td>
        <td>prints the success message if a != b else prints the failure message</td>
    </tr>
    <tr>
        <td>testTrue(a,test_name)</td>
        <td>prints the success message if a is True else prints the failure message</td>
    </tr>
    <tr>
        <td>testFalse(a,test_name)</td>
        <td>prints the success message if a is False else prints the failure message</td>
    </tr>
</table>

- All test functions use the test_name parameter to inform the user of the test if it has
failed.

- A success / pass message looks like:
```bash
[OK] MyCustomTest
```

- and a failed test looks like:

```bash
[FAILED] MyCustomTest | Expected: 1, Received: 2 
```

- the ```log_success()``` and ```log_failure()``` functions inside the module are
used by these test functions and handle the pretty printing.
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
