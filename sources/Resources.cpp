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
#include "Resources.h"
#include "NTPEParser.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>

namespace Resources 
{

//**********************************************************************************
/**
 * Function: Resources::GetAll()
 * 
 * @brief Retrieves all resources from a Portable Executable (PE) file.
 *
 * This function parses the PE file structure starting from the base address
 * and extracts information about all resources. It populates a vector with
 * details about each resource, including language, size, type, name, and
 * data content.
 *
 * @param pBase A pointer to the base of the PE file in memory.
 * @param resources A reference to a vector that will be filled with ResourceInfo
 * structures, each representing a resource found within the PE file.
 *
 */
//**********************************************************************************

std::optional<std::vector<ResourceInfo>> GetAll(BYTE* pBase, uint64_t fileSize)
{
    IMAGE_RESOURCE_DIRECTORY* pTypesDirectory = nullptr;
    std::vector<ResourceInfo> resources;

    try
    {
        //********************************************************
        //  parse PE header
        //********************************************************
        IMAGE_DOS_HEADER* pDosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(pBase);
        IMAGE_NT_HEADERS* pNtHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(pBase + pDosHeader->e_lfanew);

        // Verify that the PE signature is valid, indicating a valid PE file.
        if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
            return std::nullopt;

        // Depending on the machine type (32-bit or 64-bit), obtain the resource directory data.
        IMAGE_DATA_DIRECTORY resourceDirectory;
        switch (pNtHeaders->FileHeader.Machine)
        {
        case IMAGE_FILE_MACHINE_I386:
            resourceDirectory = reinterpret_cast<IMAGE_NT_HEADERS32*>(pNtHeaders)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE];
            break;
        case IMAGE_FILE_MACHINE_AMD64:
            resourceDirectory = reinterpret_cast<IMAGE_NT_HEADERS64*>(pNtHeaders)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE];
            break;
        default:
            return std::nullopt;
        };

        // If the resource directory is empty, exit as there are no resources.
        if (resourceDirectory.Size == 0)
            return std::nullopt;

        // Convert the RVA of the resources to a RAW offset
        uint64_t resourceBase = ntpe::RvaToOffset(pBase, resourceDirectory.VirtualAddress);
        IMAGE_RESOURCE_DIRECTORY* pResourceDir = reinterpret_cast<IMAGE_RESOURCE_DIRECTORY*>(pBase + resourceBase);

        //********************************************************
        // Start parsing the resource directory
        //********************************************************
        // Iterate through type entries in the resource directory.
        // parse types
        pTypesDirectory = pResourceDir;
        IMAGE_RESOURCE_DIRECTORY_ENTRY* pTypeEntries = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pTypesDirectory + 1);

        for (uint64_t ti = 0; ti < pTypesDirectory->NumberOfNamedEntries + pTypesDirectory->NumberOfIdEntries; ti++)
        {
            // parse names
            IMAGE_RESOURCE_DIRECTORY_ENTRY* pTypeEntry = &pTypeEntries[ti];
            IMAGE_RESOURCE_DIRECTORY* pNamesDirectory = (IMAGE_RESOURCE_DIRECTORY*)(pBase + (pTypeEntry->OffsetToDirectory & 0x7FFFFFFF) + resourceBase);
            for (uint64_t ni = 0; ni < pNamesDirectory->NumberOfNamedEntries + pNamesDirectory->NumberOfIdEntries; ni++)
            {
                //  parse langs
                IMAGE_RESOURCE_DIRECTORY_ENTRY* pNamesEntries = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pNamesDirectory + 1);
                IMAGE_RESOURCE_DIRECTORY_ENTRY* pNameEntry = &pNamesEntries[ni];
                IMAGE_RESOURCE_DIRECTORY* pLangsDirectory = (IMAGE_RESOURCE_DIRECTORY*)(pBase + (pNameEntry->OffsetToDirectory & 0x7FFFFFFF) + resourceBase);
                for (uint64_t li = 0; li < pLangsDirectory->NumberOfNamedEntries + pLangsDirectory->NumberOfIdEntries; li++)
                {
                    //  parse data
                    IMAGE_RESOURCE_DIRECTORY_ENTRY* pLangsEntries = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pLangsDirectory + 1);
                    IMAGE_RESOURCE_DIRECTORY_ENTRY* pLangEntry = &pLangsEntries[li];
                    IMAGE_RESOURCE_DATA_ENTRY* pDataEntry = (IMAGE_RESOURCE_DATA_ENTRY*)(pBase + resourceBase + pLangEntry->OffsetToData);

                    // Save the resource information in a structured format.
                    ResourceInfo entry = {};
                    entry.Language = pLangsEntries->Id;
                    entry.Size = pDataEntry->Size;
                    entry.Type = (PIMAGE_RESOURCE_DIR_STRING_U)(pTypeEntry->NameIsString) ? (PIMAGE_RESOURCE_DIR_STRING_U)(pBase + pTypeEntry->NameOffset + resourceBase) : (PIMAGE_RESOURCE_DIR_STRING_U)(pTypeEntry->Id);
                    entry.Name = (PIMAGE_RESOURCE_DIR_STRING_U)(pNameEntry->NameIsString) ? (PIMAGE_RESOURCE_DIR_STRING_U)(pBase + pNameEntry->NameOffset + resourceBase) : (PIMAGE_RESOURCE_DIR_STRING_U)(pNameEntry->Id);
                    entry.data = ntpe::RvaToRaw(pBase, pDataEntry->OffsetToData);
                    resources.push_back(entry);
                }
            }
        }

        return resources;
    }
    catch (std::exception&)
    {
        return std::nullopt;
    };
}


}