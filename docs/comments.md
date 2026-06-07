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