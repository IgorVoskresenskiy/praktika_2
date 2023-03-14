#include "command.h"
#include "rng.h"

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