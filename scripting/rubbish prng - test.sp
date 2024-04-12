#include <sourcemod>

#include "rubbish prng"

public void OnPluginStart()
{
    LoadTranslations("common.phrases");
    PrintToServer("--------------------------------------------------------");

    srand(GetTime());
    for (int i = 0; i < 10; ++i)
        PrintToServer("rand iteration %i: %i", i, rand());

    PrintToServer("\n\nnotnheavy - rubbish prng\n--------------------------------------------------------");
}