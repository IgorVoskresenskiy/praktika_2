#include "command.h"
#include "rng.h"

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
    WaitForSingleObject(rngListMutex, INFINITE);
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
