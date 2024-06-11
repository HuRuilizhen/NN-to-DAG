#ifndef __GRAPHKIT__WINDOWS_H__
#define __GRAPHKIT__WINDOWS_H__

#include "GraphKit.h"
#include <windows.h>
#include <psapi.h>
#include <stdio.h>

double GraphKit::getProcessMemory()
{
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc));
    return pmc.WorkingSetSize / 1024.0 / 1024.0;
}

#endif // __GRAPHKIT__WINDOWS_H__