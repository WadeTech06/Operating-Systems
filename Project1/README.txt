Lakiel Wade 2601283


Assignment Description
-----------------------
-----------------------
Two programs thr.c and proc.c which uses threads and processes to cuncurrently compute the sum of
square roots from 1 to n, where n is a mutiple of 3 and is specified in the command line.
-----------------------
Compiling instruction
-----------------------
----------------------
Program 1
run -> gcc -o thread  thr.c -lpthread -lm
-> ./thread [argument]

Program 2
run ->  gcc -o process proc.c -lm
-> ./process 9999
------------------------
A sample test run
------------------------
-----------------------
Program 1
lakiel@Six:~/cis345/Project 1$ ./thread 9999
Sum of square roots: 666616.459197

Program 2
lakiel@Six:~/cis345/Project 1$ ./process 9999
Sum of square roots: 666616.459197


