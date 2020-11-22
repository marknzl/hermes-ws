#include <stdio.h>
#include <stdlib.h>
#include <Shlwapi.h>
#include <Windows.h>
#include "filesyshelper.h"

#pragma comment(lib, "Shlwapi.lib")

int PathExists(char* path)
{
    char rootPath[512] = "wwwroot/";
    strcat(rootPath, path);
    return PathFileExistsA(rootPath);
}

void CheckWWWRoot()
{
    // BOOL exists = PathFileExistsA("wwwroot/");

    // if (!exists)
    // {
    //     printf("wwwroot doesn't exist! Creating web root directory...\n");
    //     CreateDirectory("wwwroot/", NULL);
    // }

    if (!PathExists(""))    // Empty string since PathExists() starts at wwwroot/
    {
        printf("wwwroot doesn't exist! Creating web root directory...\n");
        CreateDirectory("wwwroot/", NULL);
    }
}