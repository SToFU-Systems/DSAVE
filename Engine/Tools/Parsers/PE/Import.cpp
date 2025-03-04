﻿/*
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
#include <windows.h>
#include "import.h"

namespace ntpe
{

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
std::optional<ImportList> getImports(uint8_t* pBase, uint64_t fileSize)
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

        ImportList result;

        /* import table offset */
        uint64_t impOffset = ntpe::RvaToOffset(ntpe.fileBase, ntpe.dataDirectories[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

        /* imoprt table descriptor from import table offset + file base adress */
        ImageImportDescriptor* impTable = (ImageImportDescriptor*)(impOffset + ntpe.fileBase);

        /* while names in import table */
        while (impTable->Name != 0)
        {
            /* pointer to DLL name from offset of current section name + file base adress */
            std::string modname = ntpe::RvaToOffset(ntpe.fileBase, impTable->Name) + (char*)ntpe.fileBase;
            std::transform(modname.begin(), modname.end(), modname.begin(), ::toupper);

            /* start adress of names in look up table from import table name RVA */
            char* cell = (char*)ntpe.fileBase + ((impTable->OriginalFirstThunk) ? ntpe::RvaToOffset(ntpe.fileBase, impTable->OriginalFirstThunk) : ntpe::RvaToOffset(ntpe.fileBase, impTable->FirstThunk));

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
                    result[modname].emplace((char*)ntpe.fileBase + ntpe::RvaToOffset(ntpe.fileBase, rva) + 2);
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