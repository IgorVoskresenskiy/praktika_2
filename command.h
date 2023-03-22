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

bool command_parse(char* inputCommand);
bool command_process();

