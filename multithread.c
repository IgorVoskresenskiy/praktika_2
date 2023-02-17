#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "windows.h"
#include "tchar.h"
#include "strsafe.h"
#include "stdint.h"

#include "multithread.h"

int id = 0;
int n = 0;

typedef struct rng {
    int rngNumber;
    int rngTimeAsked;
    struct rng* next;
} rng;

uint8_t buf[1024];

DWORD WINAPI MyThreadFunction(int n, int f)
{

    while (true)
    {
        n += 1;
        printf_s("%d\n", n);
        Sleep(1000);
    }

}

int main()
{

    CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size  
        MyThreadFunction,       // thread function name
        n,                 // argument to thread function 
        0,                      // use default creation flags 
        &id);
    while (true)
    {
        printf_s("%s\n", __FUNCTION__);
        Sleep(3000);
    }

}