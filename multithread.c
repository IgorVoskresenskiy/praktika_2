#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "windows.h"
#include "tchar.h"
#include "strsafe.h"
#include "stdint.h"
#include "time.h"
#include "string.h"
#include "synchapi.h"

#include "multithread.h"

#define ONE_SECOND_IN_MS 1000
typedef struct rngNode
{
    unsigned int rngValue;
    int rngMaxNumber;
    int rngTimeBetweenUpdatesMs;
    int rngTimeSinceUpdateMs;
    struct rngNode* next;
} rngNode;
rngNode* rngListHead = NULL;

HANDLE rngListMutex;
int id = 0;
int n = 0;

int argc = 0;
char argv[4][20] = { 0 };

void command_parse(char* inputCommand)
{
    memset(argv, 0, 80);
    argc = 0;
    int i = 0;
    while (inputCommand[i] != '\0')
    {
        if (inputCommand[i] == ' ')
        {
            argc++;
        }
        i++;
    }
    argc++;
    int counter = 0;
    int argv2 = 0;
    int spaceCount = 0;
    i = 0;
    for (int c = 0; c < argc; c++)
    {
        while ((inputCommand[i] != ' ')&& (inputCommand[i] != '\0'))
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
    WaitForSingleObject(rngListMutex,INFINITE);
    rngNode* tmp = (rngNode*)malloc(sizeof(rngNode));
    if (tmp != NULL)
    {
        tmp->rngMaxNumber = rngNumberInput;
        tmp->rngTimeBetweenUpdatesMs = rngTimeAskedInput;
        tmp->rngTimeSinceUpdateMs = 0;
        if (rngHead == NULL)
        {
            tmp->next = NULL;
            rngHead = &tmp;
        }
        else
        {
            tmp->next = (*rngHead);
            (*rngHead) = tmp;
        }
        ReleaseMutex(rngListMutex);
        return true;
    }
    else
    {
        ReleaseMutex(rngListMutex);
        return false;
    }
}

int count_rngs(rngNode* rngHead)
{
    int rngCount=0;
    WaitForSingleObject(rngListMutex,INFINITE);
    rngNode* tmp = rngHead;
    while (tmp != NULL)
    {
        rngCount += 1;
        tmp = tmp->next;
    }
    ReleaseMutex(rngListMutex);
    return rngCount;
}

int count_rng_summ(rngNode* rngHead)
{
    int rngSumm = 0;
    WaitForSingleObject(rngListMutex,INFINITE);
    rngNode* tmp = rngHead;
    while (tmp != NULL)
    {
        rngSumm += tmp->rngValue;
        tmp = tmp->next;
    }
    ReleaseMutex(rngListMutex);
    return rngSumm;
}

void update_rngs(rngNode* rngHead)
{
    WaitForSingleObject(rngListMutex,INFINITE);
    rngNode* tmp = rngHead;
    while (tmp != NULL)
    {
        tmp->rngTimeSinceUpdateMs += ONE_SECOND_IN_MS;
        if (tmp->rngTimeSinceUpdateMs == tmp->rngTimeBetweenUpdatesMs)
        {
            tmp->rngValue = rand() / tmp->rngMaxNumber;
            tmp->rngTimeSinceUpdateMs = 0;
        }
        tmp = tmp->next;
    }
    ReleaseMutex(rngListMutex);
}

DWORD WINAPI rng_updater(int n)
{
    while (true)
    {
        Sleep(ONE_SECOND_IN_MS);
        update_rngs(rngListHead);
    }
}

int main()
{
    rngListMutex = CreateMutex(NULL,FALSE,NULL);

    CreateThread(NULL,0,rng_updater,NULL,0,&id);

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
    

    while (strstr(argv[0], "exit") == NULL)
    {
        if ((strstr(argv[0], "count_rngs") == NULL) && (strstr(argv[0], "add_rng") == NULL) && (strstr(argv[0], "print_summ") == NULL))
        {
            printf("there is no such command");
            printf("\n");
        }

        if (strstr(argv[0], "count_rngs") != NULL)
        {
            if (argc != 1)
            {
                printf("count_rngs command have to have zero arguments");
                printf("\n");
            }
            else
            {
                printf("amount of rngs active: ");
                printf("\n");
                printf("%d", count_rngs(rngListHead));
                printf("\n");
            }
        }

        if (strstr(argv[0], "print_summ") != NULL)
        {
            if (argc != 1)
            {
                printf("print_summ command have to have zero arguments");
                printf("\n");
            }
            else
            {
                printf("summ of rng values: ");
                printf("\n");
                printf("%d", count_rng_summ(rngListHead));
                printf("\n");
            }
        }

        if (strstr(argv[0], "add_rng") != NULL)
        {
            if (argc != 3)
            {
                printf("add_rng command have to have two arguments");
                printf("\n");
            }
            else
            {
                int MaxNumber = atoi(argv[1]);
                int TimeBetweenUpdatesMs = atoi(argv[2]) * ONE_SECOND_IN_MS;
                if (!push_rng(rngListHead, MaxNumber, TimeBetweenUpdatesMs))
                {
                    printf("there is no free memory");
                    printf("\n");
                }
            }
        }
        gets_s(commandInput, 20);
        command_parse(commandInput);
    }
}