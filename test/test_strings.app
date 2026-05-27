load 'lib/tester.app'

# This test file provides a range of test cases for checking
# the string operations such as equality, inequality, String
# indexing, replication and concatenation.

# string equality tests

testEqual("abc","abc","String Equality Test 1")
testEqual("hello world","hello world","String Equality Test 2")

# string inequality tests

testNotEqual("abc","aabc","String Inequality Test 1")
testNotEqual("hello world","hellddo world","String Inequality Test 2")

# sample string for indexing, replication and concatenation tests
const sample_string = "Hello Tests !"

# string indexing tests

testEqual(sample_string[0],'H',"String indexing test 1")
testEqual(sample_string[3],'l',"String indexing test 2")

# string concatenation tests

testEqual(sample_string + ' Welcome!',"Hello Tests ! Welcome!","String concatenation test 1")
testEqual("Hello World" + "!","Hello World!","String concatenation test 2")

# string replication tests

testEqual(sample_string*2,"Hello Tests !Hello Tests !","String replication test 1")
testEqual("Hi" * 3,"HiHiHi","String replication test 2")