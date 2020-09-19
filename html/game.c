#include <stdio.h>
#include <stdlib.h>

int main ()
{
    //Declare a variable named varInt of type integer equal to 123456
    int varInt = 123456;
    //Declare an array of char named arrChar of size 128 with the text "Long
    // char array right there ->" (you can put the size in a declared constant)
    char arrChar[128] = "Long char array right there ->"

    //Declare a pointer to integer named ptr2int pointing to varInt
    int *ptr2int = &varInt;

    //Declare a pointer to pointer to int named ptr2ptr pointing to ptr2int
    int *ptr2ptr = ptr2int;

    //Declare a pointer to pointer to pointer to int named ptr2ptr2 pointing to ptr2ptr
    int *ptr2int = ptr2ptr;


}
