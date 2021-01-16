#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "file.h"

struct File* FileLoad(char* path)
{
    FILE* fIn = NULL;
    fopen_s(&fIn, path, "rb");

    if (fIn == NULL)
    {
        //perror("fopen_s");
        return NULL;
    }

    struct File* file = malloc(sizeof(struct File));
    if (file == NULL)
        return NULL;

    fseek(fIn, 0, SEEK_END);
    long fileSize = ftell(fIn);
    file->fileSize = fileSize;
    rewind(fIn);

    char* buffer = malloc(fileSize * sizeof(char) + 1);
    if (buffer == NULL)
        return NULL;
    //fread_s(buffer, fileSize, 1, fileSize, fIn);
    fread(buffer, 1, fileSize, fIn);
    buffer[fileSize - 1] = '\0';    // NULL termination character...

    file->fileContent = buffer;
    file->accessTime = time(NULL);
    char* fileName = malloc((sizeof(char) * strlen(path)) + 1);
    if (fileName == NULL)
        return NULL;
    strcpy_s(fileName, strlen(path) + 1, path);
    file->fileName = fileName;
    fclose(fIn);

    return file;
}

void FileDestroy(struct File* file)
{
    printf("Freeing file '%s'\n", file->fileName);
    if (file != NULL)
    {
        free(file->fileContent);
        free(file->fileName);
        free(file);
    }
}

int FileCopy(struct File* file, char* copyName)
{
    if (!strcmp(file->fileName, copyName))
    {
        return 0;
    }

    FILE* fOut = NULL;
    fopen_s(&fOut, copyName, "wb");
    if (fOut == NULL)
        return 0;

    fwrite(file->fileContent, file->fileSize, 1, fOut);
    fclose(fOut);
    return 1;
}