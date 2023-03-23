#include "rng.h"

HANDLE rngListMutex;
unsigned int rngSumm = 0;
unsigned int rngCount = 0;

rngNode* rngListHead = NULL;

void rng_init()
{
    rngListMutex = CreateMutex(NULL, FALSE, NULL);
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
        if (tmp->rngValue > 0)
        {
            rngSumm += tmp->rngValue;
        }
        tmp = tmp->next;
    }
    ReleaseMutex(rngListMutex);
}
