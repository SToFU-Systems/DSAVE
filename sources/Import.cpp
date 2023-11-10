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
#include "import.h"

namespace Import
{
bool removeNonNativeModules(Import::IMPORT_LIST& importList)
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


//**********************************************************************************
// FUNCTION: getImportList(IMAGE_NTPE_DATA& ntpe)
// 
// ARGS:
// IMAGE_NTPE_DATA& ntpe - data from PE file.
// 
// DESCRIPTION: 
// Retrieves IMPORT_LIST(std::map<std::string, std::set<std::string>>) with all loaded into PE libraries names and imported functions.
// Map key: loaded dll's names. 
// Map value: set of imported functions names.
//
// Documentation links:
// Import Directory Table: https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#import-directory-table
//
// RETURN VALUE: 
// std::optional<IMPORT_LIST>. 
// std::nullopt in case of error.
// 
//**********************************************************************************
std::optional<IMPORT_LIST> getAll(PBYTE pBase, uint64_t fileSize)
{
    ntpe::IMAGE_NTPE_CONTEXT ntpe = {};
    std::optional<ntpe::IMAGE_NTPE_CONTEXT> ntpeOptional = ntpe::getNTPEContext(pBase, fileSize);
    if (ntpeOptional == std::nullopt)
        return std::nullopt;
    ntpe = *ntpeOptional;

    try
    {
        /* if no imaage import directory in file returns std::nullopt */
        if (ntpe.dataDirectories[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress == 0)
            return std::nullopt;

        IMPORT_LIST result;

        /* import table offset */
        uint64_t impOffset = ntpe::rva2offset(ntpe, ntpe.dataDirectories[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

        /* imoprt table descriptor from import table offset + file base adress */
        PIMAGE_IMPORT_DESCRIPTOR impTable = (PIMAGE_IMPORT_DESCRIPTOR)(impOffset + ntpe.fileBase);

        /* while names in import table */
        while (impTable->Name != 0)
        {
            /* pointer to DLL name from offset of current section name + file base adress */
            std::string modname = rva2offset(ntpe, impTable->Name) + (char*)ntpe.fileBase;
            std::transform(modname.begin(), modname.end(), modname.begin(), ::toupper);

            /* start adress of names in look up table from import table name RVA */
            char* cell = (char*)ntpe.fileBase + ((impTable->OriginalFirstThunk) ? rva2offset(ntpe, impTable->OriginalFirstThunk) : rva2offset(ntpe, impTable->FirstThunk));

            /* while names in look up table */
            for (;; cell += ntpe.CellSize)
            {
                int64_t rva = 0;

                /* break if rva = 0 */
                memcpy(&rva, cell, ntpe.CellSize);
                if (!rva)
                    break;

                /* if rva > 0 function was imported by name. if rva < 0 function was imported by ordinall */
                if (rva > 0)
                    result[modname].emplace((char*)ntpe.fileBase + ntpe::rva2offset(ntpe, rva) + 2);
                else
                    result[modname].emplace(std::string("#ord: ") + std::to_string(rva & 0xFFFF));
            };
            impTable++;
        };
        return result;
    }
    catch (std::exception&)
    {
        return std::nullopt;
    }
};

};