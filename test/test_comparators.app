load 'lib/tester.app'

# This test file provides a range of test cases for checking
# the comparators >,<,==,!=,>=,<= with numbers and booleans only
# since string comparisons are limited (lexicographically) and most
# are already covered in test_strings.app

# > 

testEqual(10 > 1,True,"Greater-than number test")
testEqual(10.3 > 10.1,True,"Greater-than float test")
testEqual(True > False,True,"Greater-than boolean test")

# <

testEqual(10 < 1,False,"Less-than number test")
testEqual(10.1 < 11.1,True,"Less-than float test")
testEqual(True < False,False,"Less-than boolean test")

# ==

testEqual(10,10,"Number Equality Test")
testEqual(10.2,10.20,"Float Equality Test")
testEqual(True,1,"Boolean Equality Test")

# != 

testNotEqual(10,11,"Number Inequality Test")
testNotEqual(10.2,10.21,"Float Inequality Test")
testNotEqual(True,False,"Boolean Inequality Test")

# >= 

testEqual(10 >= 1,True,"Greater-than-or-equal-to number test")
testEqual(10.3 >= 10.3,True,"Greater-than-or-equal-to float test")
testEqual(True >= False,True,"Greater-than-or-equal-to boolean test")

# <=

testEqual(10 <= 1,False,"Less-than-or-equal-to number test")
testEqual(10.1 <= 10.1,True,"Less-than-or-equal-to float test")
testEqual(True <= False,False,"Less-than-or-equal-to boolean test")
