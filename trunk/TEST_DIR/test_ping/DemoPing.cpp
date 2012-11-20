// DemoPing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Ping.h"

int main(int argc, char* argv[])
{
	CPing pinger;
    pinger.SetConfigure("192.168.30.124",0);
    pinger.Ping();
    pinger.Cleanup();
	getchar();
	return 0;
}
