## M-024 Complete one stretch guide

- The following readme covers all the features of M-024 and is specially made
in case you want to download the documentation all in.

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

## Commands

A new feature with a lot more to come. Inspired from shells like ```bash```,
what if we could write commands inside source code and they behave similarly
to how UNIX shells return answers. Not all commands are unix inspired and this
version has limited commands. In the upcoming version, Iam planning to add
commands such as chmod or file handling commands like grep (to make them the
standard for file handling in M-024 and not built-in functions).

### list of available commands

<table>
    <tr>
        <td>Command</td>
        <td>what it does ?</td>
    </tr>
    <tr>
        <td>pop</td>
        <td>pops an element from the list and returns it</td>
    </tr>
    <tr>
        <td>kill</td>
        <td>
            Inspired from pkill, kill deletes variables
            and functions from memory.
        </td>
    </tr>
</table>