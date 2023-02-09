/*
 *
 *  Copyright (C) 2022, SToFU Systems S.L.
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "stdafx.h"
#include <windows.h>
#include "importTools.h"

namespace importTools
{
bool removeNonNativeModules(ntpe::IMPORT_LIST& importList)
{
    std::vector<char> modPath(0x8000);

    for (auto modIt = importList.begin(); modIt != importList.end(); )
    {
        if (!SearchPathA(nullptr, modIt->first.c_str(), 0, modPath.size(), modPath.data(), nullptr))
            modIt = importList.erase(modIt);
        else
            modIt++;
    }
    return true;
};

bool createKnowledgeBase(const tools::FILES_LIST& fileList, FILE_INFORMATION_DATABASE& db)
{
    static const uint32_t kNumOfThreadsPerCore = 3;
    SYSTEM_INFO sysInfo = {};
    std::mutex  statGuard;

    

    try
    {
        auto worker = [&](uint32_t coreIndex)
        {
            auto it    = std::next(fileList.begin(), fileList.size() * coreIndex / sysInfo.dwNumberOfProcessors);
            auto itEnd = std::next(fileList.begin(), fileList.size() * (coreIndex + 1) / sysInfo.dwNumberOfProcessors);

            for (it; it != itEnd && it != fileList.end(); it++)
            {
                std::wstring path = tools::pathToFullLwr(*it);
                std::vector<char> file;
                tools::readFile(path, file);
                auto ntpeCtx = ntpe::getNTPEContext(file.data(), file.size());
                if (!ntpeCtx)
                    continue;

                if (auto importList = ntpe::getImportList(*ntpeCtx))
                {
                    if (!importList->empty())
                    {
                        //  remove not native modules from list
                        removeNonNativeModules(*importList);

                        //  add import list
                        std::scoped_lock<std::mutex> lock(statGuard);
                        db[path].setPath(path);
                        db[path].setPE(*ntpeCtx);
                        db[path].setImport(*importList);
                        
                    }
                }
            }
        };

        GetSystemInfo(&sysInfo);
        sysInfo.dwNumberOfProcessors *= kNumOfThreadsPerCore;
        std::vector<std::thread> threadPool;
        for (uint32_t coreIndex = 0; coreIndex < sysInfo.dwNumberOfProcessors; coreIndex++)
            threadPool.emplace_back(worker, coreIndex);
        for (auto& it : threadPool)
            it.join();
    }
    catch (std::exception& )
    {
    }

    return true;
};

};