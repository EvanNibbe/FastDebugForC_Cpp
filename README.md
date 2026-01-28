# FastDebugForC_Cpp
This repository contains the information necessary to run a program with println(&#95;&#95;LINE&#95;&#95;); at each point where the programmer considers the program likely to fail.

You include the header file in the code you wish to debug with println(&#95;&#95;LINE&#95;&#95;); at each step. </br>
You then run gcc signal_dependency_for_debug.c -o dependency.out && ./dependency.out deppid.bin </br>
on one terminal; </br>
and ./__your_compiled_code__ __your_command_line_arguments__ </br>
on another terminal on the same multi-core computer. </br>
As soon as your code fails, run: </br>
ps -aux | grep "dependency" </br>
#take the first number (called num below) this gives you to put in </br>
kill -27 num  </br>
   </br>
  The results printed out under the now closing ./dependency.out deppid.bin will give you the exact location where your program failed. </br>
