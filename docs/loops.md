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