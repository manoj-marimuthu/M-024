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