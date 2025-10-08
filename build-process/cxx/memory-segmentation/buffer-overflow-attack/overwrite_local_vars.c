#include<stdio.h>
#include<stdlib.h>

void read_line()
{
    int flag = 0x0;
    char buffer[8];
    
    // In gcc 14, the gets() function was removed from the standard library due to security issues.
    gets(buffer);   

    if(flag == 0x0)
    {
        printf("Access rejected!!\n");
    }
    else
    {
        printf("Access permited!!\n");
    }
}


int main(int argc, char** argv)
{
    read_line();

    return 0;
}
