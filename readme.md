## M-024 Programming Language

M-024 is a general-purpose programming language that is developed for rapid prototyping, testing and learning. The purpose for the language is under exploration and it will soon have one. M-024 is an interpreted language  and supports a wide range of features. I specifically designed it for my personal use and learning. I suppose it's advantage is its size which is less than 1mb and because of which it's setup is quick and easy. Its syntax was inspired from python and it is guaranteed to change often until a smooth convension is acheived. The language supports functions and no object oriented Features (yet). There are multiple versions (see versions column on the right) but the first one to come with a name is v0.4.0 and before that it was nameless (until v0.3.0).

### Setup

M-024 can be built using GCC or other c compilers in different Operating systems. Its development was primarily in the Windows operating system and the makefile has to modified if you experience an error during compilation (It may work directly in Linux and Mac with the make command).

- First clone from github using
```bash
git clone https://www.github.com/manoj-marimuthu/M-024.git
```
- Run the following command inside your preferred working directory
```bash
mkdir build
```
- To execute the makefile use the ```make``` command (Keep in mind that some compilers requires different command to run, example - ```mingw32-make``` for the mingw32 compiler)
```bash
make
```
- You will now see a build folder that contains all the object files produced by the c compiler. To use M-024 run the command:
```powershell
./M024 <filename>
```
- The above command is for linux-based OS and Powershell. For CMD, just run
```cmd
M024.exe <filename>
```
- Remember that the file must of .mscf extension where mscf stands for "M-024 source code file". I plan on improving portability and automate the build process for different operating systems soon.

### Will you need M-024 ?

Probably not. I designed it for my personal use and not many people have visited it in github. Python offers a better ecosystem and much more bug-free programming (bug inside the interpreter and not the logic of your program). Speed is also becoming a concern with the growing features but I will add the VM Bytecode soon to counter it. But it could be a great project for exploring interpreter theory internals and could also be used as an alternative when:

- You need a quick setup (especially in windows)
- You need a place to test your skills since
the language deliberately lacks certain features (or i was just lazy to add them) that you need for traditional problem solving and that could force you into an alternative solution.
- You need rapid coding speed (syntax optimised for speed). For example - a for loop in python for looping from one integer to another uses a range function but in M-024 just use <start,end,skip>. (Ongoing work)

### Testing (for contributors)

- Use the batch script to run all tests if you are in windows. Example in cmd:
```cmd
run_tests
```
- If you are in other operating systems, I will soon make a script for that too.
- Otherwise I suggest you to run tests for the specific feature you worked on or the features that are affected by it since the test folder contains tests for many features making it hard to infer what you need. Example - suppose you work on strings, you could just do
```cmd
./M024 "test/test_strings.mscf"
```
- I have followed a general convention of test_component.mscf for writing tests for any component. All test files require the ```tester.mscf``` standard library.

### Final thoughts

To learn the language, I suggest you to go through the documentation inside the ```docs``` folder and all the documentations are in markdown, so it is better to stick with github to reading. Contributors, Bug reports and any random thoughts on M-024 are welcome. I built M-024 for fast coding, reducing long syntax struggles and of course for learning about interpreters and their design.

 Happy Hacking !