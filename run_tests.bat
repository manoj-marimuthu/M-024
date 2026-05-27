@echo off
echo  Tests For Interpreter
 echo  ------------------------------------------ 
 echo  Arithmetic Test
 interpret.exe "test/test_arithmetic.app" 
 echo  Boolean Test
 interpret.exe "test/test_boolean.app" 
 echo  String Test
 interpret.exe "test/test_strings.app" 
 echo  Comparators Test
 interpret.exe "test/test_comparators.app" 
 echo  type() Test
 interpret.exe "test/test_type.app" 
 echo  len() Test
 interpret.exe "test/test_len.app"