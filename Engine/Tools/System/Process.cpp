/*
* This Software is part of the SToFU Systems S.L. project and is licensed under the
* SToFU Non-Commercial License Agreement (Based on AGPL).
*
* Use, modification, and distribution are permitted exclusively by private individuals
* for personal, non-commercial purposes. Any use by corporations, organizations,
* institutions, or for any commercial or profit-oriented activity is strictly prohibited
* without prior written permission from SToFU Systems S.L.
*
* This Software may include third-party components under separate licenses. In such cases,
* their terms take precedence for those components.
*
* For full details, disclaimers, and commercial licensing inquiries, please refer to:
* https://stofu.io or the LICENSE file included with this Software.
*
* All implied warranties, including merchantability and fitness, are disclaimed.
* Neither copyright owners nor contributors are liable for damages, including
* loss of data, profits, or business interruption, arising from use,
* even if warned of such possibilities.
*
* Commercial Licensing Contact: contacts@stofu.io
*/

#include "stdafx.h"

#include <iostream>
#include <thread>
#include <chrono>

#if defined(_WIN32) || defined(_WIN64)  // Windows
#include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)  // Linux è macOS
#include <unistd.h>
#include <sys/resource.h>
#endif

#include "Process.h"


namespace Process
{

/*
* set_low_priority - Set the current process priority to low.
*
* This function adjusts the priority of the current process to a low level:
* - On Windows, it sets the process to BELOW_NORMAL_PRIORITY_CLASS.
* - On Linux/macOS, it sets the nice value to 19, the lowest priority.
*
* Returns:
*  true if successful, false otherwise.
*/
    bool setLowPriority()
    {
        #if defined(_WIN32) || defined(_WIN64)  /* Windows implementation */
        HANDLE hProcess;

        hProcess = GetCurrentProcess();
        if (!SetPriorityClass(hProcess, BELOW_NORMAL_PRIORITY_CLASS)) {
            /* Failed to set priority */
            return false;
        }
        return true;

        #elif defined(__linux__) || defined(__APPLE__)  /* Linux/macOS implementation */
        /*
         * setpriority(PRIO_PROCESS, 0, 19):
         * - PRIO_PROCESS: Affects the calling process.
         * - 0: The calling process ID.
         * - 19: The "nicest" (lowest priority) value.
         */
        if (setpriority(PRIO_PROCESS, 0, 19) != 0) {
            /* Failed to set nice value */
            return false;
        }
        return true;

        #else
        /* Unsupported platform */
        return false;
        #endif
}

}