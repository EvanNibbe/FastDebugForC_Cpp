# FastDebugForC_Cpp
This repository contains the information necessary to run a program with println(__LINE__); at each point where the programmer considers the program likely to fail.

You include the header file in the code you wish to debug with println(__LINE__); at each step.
You then run gcc signal_dependency_for_debug.c -o dependency.out && ./dependency.out deppid.bin
on one terminal;
and ./__your_compiled_code__ __your_command_line_arguments__
on another terminal on the same multi-core computer.
As soon as your code fails, run:
ps -aux | grep "dependency"
#take the first number this gives you to put in
kill -27 <above number>
  
  The results printed out under the now closing ./dependency.out deppid.bin will give you the exact location where your program failed.
