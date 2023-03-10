#include "multithread.h"

#define ONE_SECOND_IN_MS 1000
#define MAX_ARGC 3

typedef struct rngNode
{
    int rngValue;
    int rngMaxNumber;
    int rngTimeBetweenUpdatesMs;
    int rngTimeSinceUpdateMs;
    struct rngNode* next;
} rngNode;
rngNode* rngListHead = NULL;

HANDLE rngListMutex;
int rngSumm = 0;
int rngCount = 0;
int argc = 0;


void rng_init()
{
    rngListMutex = CreateMutex(NULL, FALSE, NULL);
}

bool command_parse(char* inputCommand)
{
    memset(argv, 0, sizeof(argv));
    argc = 0;
    int i = 0;

    char* ptrToParsingString = inputCommand;
    char* ptrToStartOfArg = inputCommand;

    while (ptrToParsingString = strchr(ptrToParsingString, ' '))
    {
        uint8_t sizeOfParsedArg = ptrToParsingString - ptrToStartOfArg;

        memcpy(argv[argc], ptrToStartOfArg, sizeOfParsedArg);

        if (ptrToParsingString == NULL)
        {
            break;
        }

        argc++;

        if (argc > MAX_ARGC)
        {
            return false;
        }

        ptrToParsingString++;
        ptrToStartOfArg = ptrToParsingString;
    }

    uint8_t sizeOfParsedArg = inputCommand + strlen(inputCommand) - ptrToStartOfArg;
    memcpy(argv[argc], ptrToStartOfArg, sizeOfParsedArg);

    argc++;
}

bool add_rng(rngNode** rngHead, int rngNumberInput, int rngTimeAskedInput)
{
    rngNode* tmp = (rngNode*)malloc(sizeof(rngNode));
    if (tmp != NULL)
    {
        tmp->rngMaxNumber = rngNumberInput;
        tmp->rngTimeBetweenUpdatesMs = rngTimeAskedInput;
        tmp->rngTimeSinceUpdateMs = 0;
        WaitForSingleObject(rngListMutex, INFINITE);
        tmp->next = (*rngHead);
        (*rngHead) = tmp;
        ReleaseMutex(rngListMutex);
        rngCount += 1;
        return true;
    }
    else
    {
        return false;
    }
}

void update_rngs(rngNode* rngHead)
{
    rngSumm = 0;
    WaitForSingleObject(rngListMutex,INFINITE);
    rngNode* tmp = rngHead;
    while (tmp != NULL)
    {
        tmp->rngTimeSinceUpdateMs += ONE_SECOND_IN_MS;
        if (tmp->rngTimeSinceUpdateMs == tmp->rngTimeBetweenUpdatesMs)
        {
            int rands = rand();
            tmp->rngValue = rands % tmp->rngMaxNumber;
            tmp->rngTimeSinceUpdateMs = 0;
        }
        rngSumm += tmp->rngValue;
        tmp = tmp->next;
    }
    ReleaseMutex(rngListMutex);
}

DWORD WINAPI rng_updater()
{
    while (true)
    {
        Sleep(ONE_SECOND_IN_MS);
        update_rngs(rngListHead);
    }
}

void command_process()
{
    gets_s(commandInput, 20);
    if (!command_parse(commandInput))
    {
        printf("too much arguments");
        printf("\n");
    }

    if ((strstr(argv[0], "count_rngs") == NULL) && (strstr(argv[0], "add_rng") == NULL) && (strstr(argv[0], "print_summ") == NULL) && (strstr(argv[0], "exit") == NULL))
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
            printf("%d", rngCount);
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
            printf("%d", rngSumm);
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
            if (!add_rng(&rngListHead, MaxNumber, TimeBetweenUpdatesMs))
            {
                printf("there is no free memory");
                printf("\n");
            }
            else
            {
                printf("rng added succesfully");
                printf("\n");
            }
        }
    }
} 