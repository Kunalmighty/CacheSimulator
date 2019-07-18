Kunal Baran Maiti
kmaiti1@jhu.edu

A cache simulator written in C++, 
that accepts an abbreviated memory trace file as an input, then simulates six different caching techniques on it.

The input to the simulator has to be an abbreviated memory trace file. 
Each line in the trace file should be composed of:
1) the memory access address, 
2) and a flag indicating if the memory operation was a Load (L) or a store (S).
As an example, consider the following three lines:
1fffff50 S 
1fffff58 L 
1fffff88 L 
We first have a store to address 0x1fffff50, 
followed by a load from address 0x1fffff58 
and another load from address 0x1fffff88.

The addresses are in hexadecimal and may be up to 16 characters long (64 bit addresses). 

For each trace file given to the simulator, it simulates the following caching techniques:
1) Direct Mapped 
2) Fully Associative, Least Recently Used 
3) Set Associative, Least Recently Used 
4) Blocked Set Associative, Least Recently Used 
5) Write-Through No Write - Allocate 
6) Prefetch
