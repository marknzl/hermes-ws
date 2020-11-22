#include <string.h>
#include "mimehelper.h"

char* GetMimeType(char* resourcePath)
{
    char* extension = strrchr(resourcePath, '.');
    extension++;

    if (!strcmp(extension, "html"))
    {
        return "text/html;";
    }
    else if (!strcmp(extension, "css"))
    {
        return "text/css;";
    } else if (!strcmp(extension, "ico"))
    {
        return "image/png;";
    }

    return "text/plain;";
}