#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void attack()
{
    printf("attacker's code is executed!!\n");
    exit(0);
}


void get_input()
{   
    char buffer[8];
 
    // In gcc 14, the gets() function was removed from the standard library due to security issues.
    gets(buffer);
    puts(buffer);
}


int main(int argc, char** argv)
{
    get_input();

    return 0;
}
