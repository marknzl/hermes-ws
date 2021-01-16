#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mimehelper.h"

char* GetMimeType(char* resource)
{
	const char* extensionStart = strrchr(resource, '.') + 1;

	if (!strcmp(extensionStart, "html"))
	{
		return "text/html";
	}
    else if (!strcmp(extensionStart, "css"))
    {
        return "text/css";
    }
    else if (!strcmp(extensionStart, "ico"))
    {
        return "image/png";
    }

    return "text/plain";
}