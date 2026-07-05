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