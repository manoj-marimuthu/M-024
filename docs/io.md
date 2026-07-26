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