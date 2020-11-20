#include <stdio.h>
#include <Shlwapi.h>
#include <Windows.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Shlwapi.lib")

void CheckWWWRoot()
{
    BOOL exists = PathFileExistsA("wwwroot/");

    if (!exists)
    {
        printf("wwwroot doesn't exist! Creating web root directory...\n");
        CreateDirectory("wwwroot/", NULL);
    }
}