load 'lib/tester.app'

# The following test file provides a range of test cases
# to check if arithemtic operations such as +,-,*,/,%,^
# work correctly for Integers and Floats.

# +
testEqual(10 + 1,11,"Basic Integer Addition Test")
testEqual(10.10 + 1.20,11.30,"Basic Float Addition Test")

# -

testEqual(20 - 5,15,"Basic Integer Subtraction Test")
testEqual(20.5 - 10.4,10.1,"Basic Float Subtraction Test")

# *

testEqual(20 * 4,80,"Basic Integer Multiplication Test")
testEqual(2.5 * 2,5.0,"Basic Float Multiplication Test")

# /

testEqual(60 / 3,20,"Basic Integer Division Test")
testEqual(25.5 / 5,5.05,"Basic Float Division Test")

# %

testEqual(30 % 4,2,"Basic Integer Remainder Test")
testEqual(5.4 % 5,0.4,"Basic Float Remainder Test")

# ^

testEqual(3 ^ 3,27,"Basic Integer Exponent Test")
testEqual(2.5 ^ 2,6.25,"Basic Float Exponent Test")