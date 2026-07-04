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