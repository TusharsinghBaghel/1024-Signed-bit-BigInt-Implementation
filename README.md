# IMPLEMENTATION OF SIGNED BIGINT 1024 BIT INTEGERS WITH ADDITION, SUBTRACTION AND MULTIPLICATION in C

Range : -10^309 to 10^309 
Using int datatype to store the BigInt bits. To get maximum memory efficiency we can take the base of INT_MAX.
Although the problem with this would be conversion, as it would require division of BigInt with INT_MAX for bit conversion. 

So used the BASE as 10^9 as there would not be a need of bit conversion. Just grouping the number to 9 digits and 
storing as elements in array of integers would suffice. And also because 10^9 is the closest to INT_MAX
