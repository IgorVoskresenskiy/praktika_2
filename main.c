#include "command.h"
#include "rng.h"

int threadID = 0;

DWORD WINAPI background_task(LPVOID lpVoid)
{
    while (true)
    {
        Sleep(ONE_SECOND_IN_MS);
        update_rngs(rngListHead);
    }
}

void main()
{
    rng_init();

    CreateThread(NULL, 0, background_task, NULL, 0, &threadID);

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

    while (command_process())
    {

    }
}