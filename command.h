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

char argv[4][20] = { 0 };
int threadID = 0;
uint8_t commandInput[20] = { 0 };

DWORD WINAPI rng_updater();