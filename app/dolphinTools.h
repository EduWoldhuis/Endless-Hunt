#pragma once

#include <Windows.h>
#include <Psapi.h>
#include <string>
#include <tlhelp32.h>
#include <string>
#include <iostream>
#include <chrono>
#include <fstream>


#include "../dolphin_memory/DolphinAccessor.h"
#include "../dolphin_memory/MemoryScanner/MemoryScanner.h"
#include <string>



static std::string wchar_to_string(static WCHAR input[])
{
    char ch[260];
    char DefChar = ' ';
    WideCharToMultiByte(CP_ACP, 0, input, -1, ch, 260, &DefChar, NULL);

    //A std:string using the char* constructor.
    std::string output(ch);
    
    std::cout << "szExefile: " << output << std::endl;
    return output;
}


static bool findPID()
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    int m_PID;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        do
        {

            std::string szExeFile = wchar_to_string(entry.szExeFile);
            if (szExeFile == "Dolphin.exe" ||
                szExeFile == "DolphinQt2.exe" ||
                szExeFile == "DolphinWx.exe")
            {
                m_PID = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &entry) == TRUE);
    }

    CloseHandle(snapshot);
    if (m_PID == -1)
        // Here, Dolphin doesn't appear to be running on the system
        return false;

    // Get the handle if Dolphin is running since it's required on Windows to read or write into the
    // RAM of the process and to query the RAM mapping information
    HANDLE m_hDolphin = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ |
        PROCESS_VM_WRITE,
        FALSE, m_PID);
    // PID stored in m_PID
    return true;
}

