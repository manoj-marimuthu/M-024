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