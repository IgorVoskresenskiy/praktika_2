#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "windows.h"
#include "tchar.h"
#include "strsafe.h"
#include "stdint.h"
#include "time.h"

#include "multithread.h"

#define EXIT_COMMAND 4

int id = 0;
int n = 0;

char argv[15] = { 0 };
int argc[3] = { 0 };

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
    char argcToConvert[5] = { 0 };
    while ((inputCommand[i] != ' ') && (inputCommand[i] != '\0'))
    {
        argv[i] = inputCommand[i];
        i++;
    }
    i++;
    while ((inputCommand[i] != ' ') && (inputCommand[i] != '\0'))
    {
        argcToConvert[counter] = inputCommand[i];
        i++;
        counter++;
    }
    argc[0] = atoi(argcToConvert);
    for (int i = 0; i < 5; i++)
    {
        argcToConvert[i] = 0;
    }
    i++;
    counter = 0;
    while ((inputCommand[i] != ' ') && (inputCommand[i] != '\0'))
    {
        argcToConvert[counter] = inputCommand[i];
        i++;
        counter++;
    }
    argc[1] = atoi(argcToConvert);
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
    printf("commands for rng actions: ");
    printf("\n");
    printf("1 - print amount of rngs");
    printf("\n");
    printf("2 - add rng");
    printf("\n");
    printf("3 - print the summ of all rng results");
    printf("\n");
    printf("4 - exit");
    printf("\n");

    uint8_t commandInput[3] = { 0 };
    gets_s(commandInput, 3);
    uint8_t command = atoi(commandInput);
    commandInput[0] = 0;
    commandInput[1] = 0;
    commandInput[2] = 0;

    CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size  
        MyThreadFunction,       // thread function name
        NULL,                   // argument to thread function 
        0,                      // use default creation flags 
        &id);

    while (command != 4)
    {
        if (command == 1)
        {

        }

        if (command == 2)
        {
            
        }

        if (command == 3)
        {

        }

        gets_s(commandInput, 3);
        command = atoi(commandInput);
        commandInput[0] = 0;
        commandInput[1] = 0;
        commandInput[2] = 0;
       /* printf_s("%s\n", __FUNCTION__);
        Sleep(100);*/
    }

}