#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "nethelper.h"
#include "filesyshelper.h"

int main(void)
{
    CheckWWWRoot();
    RunWebServer();

    return 0;
}