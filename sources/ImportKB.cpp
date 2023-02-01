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
#include "importKB.h"

namespace importKB
{
bool removePackedFiles(IMPORT_KNOWLEDGE_BASE& knowledgeBase)
{
    static const uint32_t kMinValue = 4;
    for (auto fileIt = knowledgeBase.begin(); fileIt != knowledgeBase.end(); )
    {
        bool needToDelete = false;
        if (fileIt->size() <= kMinValue || (*fileIt)["KERNEL32.DLL"].size() <= kMinValue)
            fileIt = knowledgeBase.erase(fileIt);
        else
            fileIt++;
    }
    return true;
}

bool removeNonNativeModules(IMPORT_KNOWLEDGE_BASE& knowledgeBase)
{
    std::vector<char> modPath(0x8000);

    for (auto fileIt = knowledgeBase.begin(); fileIt != knowledgeBase.end(); )
    {
        for (auto it = fileIt->begin(); it != fileIt->end(); )
        {
            if (!SearchPathA(nullptr, it->first.c_str(), 0, modPath.size(), modPath.data(), nullptr))
                it = fileIt->erase(it);
            else 
                it++;
        }

        if (fileIt->empty())
            fileIt = knowledgeBase.erase(fileIt);
        else 
            fileIt++;
    }
    return true;
};


std::vector<ntpe::IMPORT_LIST> createKnowledgeBase(const tools::FILES_LIST& fileList)
{
    static const uint32_t kNumOfThreadsPerCore = 3;
    IMPORT_KNOWLEDGE_BASE knowledgeBase;
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
                if (auto importList = ntpe::getImportList(*it))
                {
                    if (!importList->empty())
                    {
                        std::scoped_lock<std::mutex> lock(statGuard);
                        knowledgeBase.emplace_back(*importList);
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

    removeNonNativeModules(knowledgeBase);
    removePackedFiles(knowledgeBase);
    std::sort(knowledgeBase.begin(), knowledgeBase.end(), [](const ntpe::IMPORT_LIST& a, const ntpe::IMPORT_LIST& b) {
            return a.size() > b.size();
        });
    return knowledgeBase;
};


IMPORT_STAT createStatisticBase(const IMPORT_KNOWLEDGE_BASE& knowledgeBase)
{
    IMPORT_STAT impStat;

    for (const auto& it : knowledgeBase)
    {
        for (auto const& itMod : it)
        {
            impStat[itMod.first].cnt++;
            for (auto const& itApi : itMod.second)
            {
                impStat[itMod.first].ApiMap[itApi].cnt++;
            }
        }
    }

    return impStat;
}

};