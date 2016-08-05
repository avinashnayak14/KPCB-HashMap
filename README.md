____________________________________________________________________
 KPCB Engineering Fellows Application Challenge Question ReadMe File

 Name: Avinash Nayak                 
 College: Princeton University       
 Major: Computer Science             
____________________________________________________________________

This readme file contains instructions on how to compile
and run my hashmap program to test out my implementation
of the fixed-size hash map within "hashmap_AvinashNayak.c".
My code is written in the C programming language, and
therefore needs to import a few of C's standard libraries,
but there are no fully external libraries used. Additionally,
only primitive types are used, meaning no explicit arrays, data structures, or external abstract data types.

I implemented a linear probing hashmap in the C programming
language. My knowledge of C from Princeton's COS 217 Programming Systems course was instrumental to completing this challenge, and I would like to acknowledge my professors Jennifer Rexford and Robert Dondero for their great organization of the course and teaching as well.


Here are the instructions for running my implementation:

1. Please read through the implementation and its own
   documentation to see my code details and structure.
2. Please open up a terminal window. Since the application
   specification said that the code can run on a *nix
   operating system, please run the commands that are stated
   below in this terminal window.
3. Navigate to the local directory on which you have stored
   my file, "hashmap_AvinashNayak.c". The testing is in the
   same file as the implementation, since the specification 
   said everything should be in one "class" (file for C).
4. Run the following command on the terminal window to 
   compile my C code:
		gcc hashmap_AvinashNayak.c -o hashmap_AvinashNayak
5. Run the following command on the terminal window to
   run my C code:
   		./hashmap_AvinashNayak
   This will run my C code and the tests I have written in
   it. Please look over the test code within my main() function
   to fully understand the output that my program produces, and 
   how it tests all the functions that I have written.
