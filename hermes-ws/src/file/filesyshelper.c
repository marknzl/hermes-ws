#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Shlwapi.h>
#include <Windows.h>

#include "filesyshelper.h"
#include "../constants.h"

int PathExists(const char* path)
{
	size_t rootDirLength = strlen(ROOT_DIR);
	size_t pathLength = strlen(path);
	size_t totalLength = rootDirLength + pathLength;

	char* buf = malloc((sizeof(char) * totalLength) + 1);	// The '+1' is for the null termination ('\0') character.
	if (buf == NULL)
		return 1;
	memcpy(buf, ROOT_DIR, rootDirLength);
	memcpy(buf + 8, path, pathLength);
	buf[totalLength] = '\0';

	int exists = PathFileExistsA(buf);
	free(buf);

	return exists;
}

void CheckWWWRoot()
{
	if (!PathExists(""))
	{
		printf("wwwroot doesn't exist! Creating web root directory...\n");
		CreateDirectory(L"wwwroot/", NULL);	// 'L' prefix = wide character literal
	}
}

char* GetFullFilePath(char* resourcePath)
{
	size_t totalLength = strlen(ROOT_DIR) + strlen(resourcePath + 1) + 1;
	char* buf = malloc(sizeof(char) * totalLength);
	if (buf == NULL)
		return;
	memcpy(buf, ROOT_DIR, strlen(ROOT_DIR));
	memcpy(buf + strlen(ROOT_DIR), resourcePath + 1, strlen(resourcePath + 1));
	buf[totalLength - 1] = '\0';

	return buf;
}