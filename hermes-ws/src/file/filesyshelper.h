#ifndef FILESYSHELPER_H
#define FILESYSHELPER_H

void CheckWWWRoot();
int PathExists(const char* path);
char* GetFullFilePath(char* resourcePath);

#endif