#include "command.h"
#include "rng.h"

static char argv[4][20] = { 0 };
static uint8_t commandInput[20] = { 0 };
static unsigned int argc = 0;

bool command_parse(char* inputCommand)
{
    memset(argv, 0, sizeof(argv));
    argc = 0;
    int i = 0;

    char* ptrToParsingString = inputCommand;
    char* ptrToStartOfArg = inputCommand;

    while (ptrToParsingString = strchr(ptrToParsingString, ' '))
    {
        uint8_t sizeOfParsedArg = (uint8_t)(ptrToParsingString - ptrToStartOfArg);

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

    uint8_t sizeOfParsedArg = inputCommand + ((uint8_t)(strlen(inputCommand)) - (uint8_t)(ptrToStartOfArg));
    memcpy(argv[argc], ptrToStartOfArg, sizeOfParsedArg);

    argc++;
    return true;
}

bool command_process()
{
    gets_s(commandInput, 20);
    if (strcmp(commandInput, "exit")==0)
    {
        return false;
    }

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
    return true;
} 