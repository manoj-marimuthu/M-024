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