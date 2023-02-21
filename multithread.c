#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "windows.h"
#include "tchar.h"
#include "strsafe.h"
#include "stdint.h"
#include "time.h"
#include "string.h"

#include "multithread.h"

int id = 0;
int n = 0;
int command = 0;

char argc[25] = { 0 };
char argv[4][20] = { 0 };

typedef struct rngNode
{
    int rngNumber;
    int rngTimeAsked;
    struct rngNode* next;
} rngNode;
rngNode* rngListHead = NULL;

void command_parse(char* inputCommand)
{
    int i = 0;
    int counter = 0;
    int argv2 = 0;
    char argcToConvert[5] = { 0 };
    while (inputCommand[i] != '\0')
    {
        while ((inputCommand[i] != ' ') && (inputCommand[i] != '\0'))
        {
            argv[counter][argv2] = inputCommand[i];
            i++;
            argv2++;
        }
        argv2 = 0;
        i++;
        counter++;
    }
}

bool push_rng(rngNode** rngHead, int rngNumberInput, int rngTimeAskedInput)
{
    rngNode* tmp = (rngNode*)malloc(sizeof(rngNode));
    if (tmp != NULL)
    {
        tmp->rngNumber = rngNumberInput;
        tmp->rngTimeAsked = rngTimeAskedInput;
        tmp->next = (*rngHead);
        (*rngHead) = tmp;
        return true;
    }
    else
    {
        return false;
    }
}

int count_rngs(rngNode* rngHead)
{
    int rngCount=0;
    rngNode* tmp = rngHead;
    while (tmp != NULL)
    {
        rngCount += 1;
        tmp = tmp->next;
    }
    return rngCount;
}

int count_rng_summ(rngNode* rngHead)
{
    int rngSumm = 0;
    rngNode* tmp = rngHead;
    while (tmp != NULL)
    {
        rngSumm += tmp->rngNumber;
        tmp = tmp->next;
    }
    return rngSumm;
}
uint8_t buf[1024];

DWORD WINAPI MyThreadFunction(int n)
{
    int rngSumm = 0;
    while (true)
    {
        n += 1;
        printf_s("%d\n", n);
        rngSumm = count_rng_summ(rngListHead);
        Sleep(1000);

    }
}

int main()
{
    printf("Commands for rng actions: ");
    printf("\n\n");
    printf("To print the amount of rngs working type: \"count_rngs\"");
    printf("\n\n");
    printf("To add rng type: \"add_rng N T\" where N is the upper limit of generatable number, T is the time between generations");
    printf("\n\n");
    printf("To print the summ of all rng results type: \"print_summ\"");
    printf("\n\n");
    printf("To exit type: \"exit\"");
    printf("\n");

    uint8_t commandInput[20] = { 0 };
    gets_s(commandInput, 20);
    command_parse(commandInput);
    for (int i = 0; i < 20; i++)
    {
        commandInput[i] = 0;
    }

    CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size  
        MyThreadFunction,       // thread function name
        NULL,                   // argument to thread function 
        0,                      // use default creation flags 
        &id);
    
    while (strstr(argv[0], "exit") == NULL)
    {
        if (strstr(argv[0], "count_rngs") != NULL)
        {
            command = 1;
        }

        if (strstr(argv[0], "print_summ") != NULL)
        {
            command = 2;
        }

        if (strstr(argv[0], "add_rng") != NULL)
        {
            command = 3;
        }

        gets_s(commandInput, 20);
        command_parse(commandInput);
        for (int i = 0; i < 20; i++)
        {
            commandInput[i] = 0;
        }
       /* printf_s("%s\n", __FUNCTION__);
        Sleep(100);*/
    }

}