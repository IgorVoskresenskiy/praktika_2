#pragma once

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
extern rngNode* rngListHead;

extern HANDLE rngListMutex;
extern int rngSumm;
extern int rngCount;

void rng_init();
bool add_rng(rngNode** rngHead, int rngNumberInput, int rngTimeAskedInput);
void update_rngs(rngNode* rngHead);

