#ifndef FILE_H
#define FILE_H

struct File
{
	time_t accessTime;
	long fileSize;
	char* fileName;
	char* fileContent;
};

struct File* FileLoad(char* path);
void FileDestroy(struct File* file);
int FileCopy(struct File* file, char* copyName);

#endif