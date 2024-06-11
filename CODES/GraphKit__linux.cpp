#ifndef __GRAPHKIT__LINUX_H__
#define __GRAPHKIT__LINUX_H__

#include "GraphKit.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

double GraphKit::getProcessMemory()
{
    FILE *fp = fopen("/proc/self/status", "r");
    char line[128];
    while (fgets(line, 128, fp) != NULL)
    {
        if (strncmp(line, "VmRSS:", 6) == 0)
        {
            fclose(fp);
            return atof(line + 6) / 1024.0 / 1024.0;
            break;
        }
    }
    fclose(fp);
    return 0;
}

#endif // __GRAPHKIT__LINUX_H__