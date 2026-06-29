@echo off
echo  Tests For Interpreter
echo  ------------------------------------------ 
echo  Arithmetic Test
M024.exe "test/test_arithmetic.mscf" 
echo  Boolean Test
M024.exe "test/test_logical.mscf" 
echo  String Test
M024.exe "test/test_strings.mscf" 
echo  Comparators Test
M024.exe "test/test_comparators.mscf" 
echo  type() Test
M024.exe "test/test_type.mscf" 
echo  len() Test
M024.exe "test/test_len.mscf"
echo for loop Test
M024.exe "test/test_for_loop.mscf"
echo function kill Test
M024.exe "test/test_function_kill.mscf"
echo variable kill Test
M024.exe "test/test_variable_kill.mscf"
echo list Test
M024.exe "test/test_list.mscf"