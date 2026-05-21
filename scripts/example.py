
#disable VS code extension /editor extensions for seeing this without error
#constants and const keyword are undefined in actual python
# bottom left corner -> click {} python -> change to Plain text

#using import (make sure to keep import files (math.py and author.py) in the same directory as MiniPython.exe)
import math
import author

# arithemtic expressions
const radius = 2
area  =  radius * radius * pi 

printc("Area of the Circle: ")
print(area)
printc("Program Written By: ")
print(author)

print()

# logical expressions with booleans
a = True or False
b = not a and (True and False)
printc("Boolean Expresion Solution: ")
print(b)

# string expression with concatenation (* not supported,yet)
s1=  "This is"
s2 = " a simple python"
s3 = " interpreter !"

stringExpression = s1 + (s2 + s3)
print(stringExpression)