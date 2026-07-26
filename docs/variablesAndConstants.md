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