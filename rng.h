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
rngNode* rngListHead = NULL;

HANDLE rngListMutex;
int rngSumm = 0;
int rngCount = 0;
int argc = 0;
