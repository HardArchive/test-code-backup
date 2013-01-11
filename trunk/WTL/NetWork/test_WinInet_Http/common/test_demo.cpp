#pragma once

#include "BwWinINet.h"

using namespace BW_Network;

int main(int argc, char* argv[])
{
    BwWinINet internet;
    internet.OpenInternet();
    
    int error;
    error = internet.ConnectInternet("127.0.0.1", 1032);

    error = internet.OpenRequest("GET", "/Data/test.rar");    ///Data/test.rar


    error = internet.SendRequest(NULL, 0, NULL, 0);

    //Query all heads
    DWORD headsLen = internet.GetHeadAllLength();
    LPSTR pHeads = new CHAR[headsLen];
    internet.QueryHeadAll(pHeads, &headsLen);
    printf("Head List:%s\n", pHeads);
    
    //Query specific head
    DWORD hLen = internet.GetHeadSingleLength("Content-Type");
    LPSTR ph = new CHAR[hLen];
    internet.QueryHeadSingle(ph, "Content-Type", hLen);
    printf("Content-Type:%s\n", ph);

    //Download file
    DWORD size = internet.GetDataSize();
    LPVOID pData = new CHAR[size];
    error = internet.AcquireData(pData, size);
    error = internet.SaveFile("T:/test.rar", (LPSTR)pData, size );

    return 0;
}