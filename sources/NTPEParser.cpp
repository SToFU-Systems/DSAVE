#include "stdafx.h"

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

namespace ntpe
{
    static constexpr uint64_t g_kRvaError = -1;

    // These types is defined in NTPEParser.h
    // typedef std::map<std::string, std::set<std::string>> IMPORT_LIST;
    // typedef std::vector<IMAGE_SECTION_HEADER> SECTIONS_LIST;

    //**********************************************************************************
    // FUNCTION: alignUp(DWORD value, DWORD align)
    // 
    // ARGS:
    // DWORD value - value to align.
    // DWORD align - alignment.
    // 
    // DESCRIPTION: 
    // Aligns argument value with the given alignment.
    // 
    // Documentation links:
    // Alignment: https://learn.microsoft.com/en-us/cpp/cpp/alignment-cpp-declarations?view=msvc-170
    // 
    // RETURN VALUE: 
    // DWORD aligned value.
    // 
    //**********************************************************************************
    DWORD alignUp(DWORD value, DWORD align)
    {
        DWORD mod = value % align;
        return value + (mod ? (align - mod) : 0);
    };

    //**********************************************************************************
    // FUNCTION: rva2offset(IMAGE_NTPE_DATA& ntpe, DWORD rva)
    // 
    // ARGS:
    // IMAGE_NTPE_DATA& ntpe - data from PE file.
    // DWORD rva - relative virtual address.
    // 
    // DESCRIPTION: 
    // Parse RVA (relative virtual address) to offset.
    // 
    // RETURN VALUE: 
    // int64_t offset. 
    // g_kRvaError (-1) in case of error.
    // 
    //**********************************************************************************
    int64_t rva2offset(IMAGE_NTPE_DATA& ntpe, uint64_t rva)
    {
        /* retrieve first section */
        try
        {
            /* if rva is inside MZ header */
            PIMAGE_SECTION_HEADER sec = ntpe.sectionDirectories;
            if (!ntpe.fileHeader->NumberOfSections || rva < sec->VirtualAddress)
                return rva;

            /* walk on sections */
            for (uint32_t sectionIndex = 0; sectionIndex < ntpe.fileHeader->NumberOfSections; sectionIndex++, sec++)
            {
                /* count section end and allign it after each iteration */
                DWORD secEnd = ntpe::alignUp(sec->Misc.VirtualSize, ntpe.SecAlign) + sec->VirtualAddress;
                if (sec->VirtualAddress <= rva && secEnd > rva)
                    return rva - sec->VirtualAddress + sec->PointerToRawData;
            };
        }
        catch (std::exception&)
        {
        }

        return g_kRvaError;
    };


    //**********************************************************************************
    // FUNCTION: getNTPEData(char* fileMapBase)
    // 
    // ARGS:
    // char* fileMapBase - the starting address of the mapped file.
    // 
    // DESCRIPTION: 
    // Parses following data from mapped PE file.
    //  
    // Documentation links:
    // PE format structure: https://learn.microsoft.com/en-us/windows/win32/debug/pe-format
    //
    // RETURN VALUE: 
    // std::optional<IMAGE_NTPE_DATA>. 
    // std::nullopt in case of error.
    // 
    //**********************************************************************************
    #define initNTPE(HeaderType, cellSize) \
    { \
    char* ntstdHeader       = (char*)fileHeader + sizeof(IMAGE_FILE_HEADER); \
    HeaderType* optHeader   = (HeaderType*)ntstdHeader; \
    data.sectionDirectories = (PIMAGE_SECTION_HEADER)(ntstdHeader + sizeof(HeaderType)); \
    data.SecAlign           = optHeader->SectionAlignment; \
    data.dataDirectories    = optHeader->DataDirectory; \
    data.CellSize           = cellSize;	\
    }
    std::optional<IMAGE_NTPE_DATA> getNTPEData(char* fileMapBase, uint64_t fileSize)
    {
        try
        {
            /* PIMAGE_DOS_HEADER from starting address of the mapped view*/
            PIMAGE_DOS_HEADER dosHeader = (IMAGE_DOS_HEADER*)fileMapBase;

            /* return std::nullopt in case of no IMAGE_DOS_SIGNATUR signature */
            if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
                return std::nullopt;

            /* PE signature adress from base address + offset of the PE header relative to the beginning of the file */
            PDWORD peSignature = (PDWORD)(fileMapBase + dosHeader->e_lfanew);
            if ((char*)peSignature <= fileMapBase || (char*)peSignature - fileMapBase >= fileSize)
                return std::nullopt;

            /* return std::nullopt in case of no PE signature */
            if (*peSignature != IMAGE_NT_SIGNATURE)
                return std::nullopt;

            /* file header address from PE signature address */
            PIMAGE_FILE_HEADER fileHeader = (PIMAGE_FILE_HEADER)(peSignature + 1);
            if (fileHeader->Machine != IMAGE_FILE_MACHINE_I386 &&
                fileHeader->Machine != IMAGE_FILE_MACHINE_AMD64)
                return std::nullopt;

            /* result IMAGE_NTPE_DATA structure with info from PE file */
            IMAGE_NTPE_DATA data = {};

            /* base address and File header address assignment */
            data.fileBase = fileMapBase;
            data.fileHeader = fileHeader;

            /* addresses of PIMAGE_SECTION_HEADER, PIMAGE_DATA_DIRECTORIES, SectionAlignment, CellSize depending on processor architecture */
            switch (fileHeader->Machine)
            {
            case IMAGE_FILE_MACHINE_I386:
                initNTPE(IMAGE_OPTIONAL_HEADER32, 4);
                return data;

            case IMAGE_FILE_MACHINE_AMD64:
                initNTPE(IMAGE_OPTIONAL_HEADER64, 8);
                return data;
            }
        }
        catch (std::exception&)
        {

        }
        return std::nullopt;
    }


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
    std::optional<IMPORT_LIST> getImportList(IMAGE_NTPE_DATA& ntpe)
    {
        try
        {
            /* if no imaage import directory in file returns std::nullopt */
            if (ntpe.dataDirectories[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress == 0)
                return std::nullopt;

            IMPORT_LIST result;

            /* import table offset */
            uint64_t impOffset = rva2offset(ntpe, ntpe.dataDirectories[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

            /* imoprt table descriptor from import table offset + file base adress */
            PIMAGE_IMPORT_DESCRIPTOR impTable = (PIMAGE_IMPORT_DESCRIPTOR)(impOffset + ntpe.fileBase);

            /* while names in import table */
            while (impTable->Name != 0)
            {
                /* pointer to DLL name from offset of current section name + file base adress */
                std::string modname = rva2offset(ntpe, impTable->Name) + ntpe.fileBase;
                std::transform(modname.begin(), modname.end(), modname.begin(), ::toupper);

                /* start adress of names in look up table from import table name RVA */
                char* cell = ntpe.fileBase + ((impTable->OriginalFirstThunk) ? rva2offset(ntpe, impTable->OriginalFirstThunk) : rva2offset(ntpe, impTable->FirstThunk));

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
                        result[modname].emplace(ntpe.fileBase + rva2offset(ntpe, rva) + 2);
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

    //**********************************************************************************
    // FUNCTION: getImportList(IMAGE_NTPE_DATA& ntpe)
    // 
    // ARGS:
    // std::wstring_view filePath - path to file.
    // 
    // DESCRIPTION: 
    // Retrieves IMPORT_LIST(std::map<std::string, std::set<std::string>>) with all loaded into PE libraries names and imported functions bu path.
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
    std::optional<IMPORT_LIST> getImportList(std::wstring_view filePath)
    {
        std::vector<char> buffer;
        /* obtain base address of mapped file from tools::readFile function */
        bool result = tools::readFile(filePath, buffer);
        /* return nullopt if readFile failes or obtained buffer is empty */
        if (!result || buffer.empty())
            return std::nullopt;
        /* get IMAGE_NTPE_DATA from base address of mapped file */
        std::optional<IMAGE_NTPE_DATA> ntpe = getNTPEData(buffer.data(), buffer.size());
        if (!ntpe)
            return std::nullopt;
        /* return result of overloaded getImportList function with IMAGE_NTPE_DATA as argument */
        return getImportList(*ntpe);
    }


    //**********************************************************************************
    // FUNCTION: getSectionsList(IMAGE_NTPE_DATA& ntpe)
    // 
    // ARGS:
    // IMAGE_NTPE_DATA& ntpe - data from PE file.
    // 
    // DESCRIPTION: 
    // Retrieves SECTIONS_LIST from IMAGE_NTPE_DATA.
    // SECTIONS_LIST - vector of sections headers from portable executable file.
    // Sections names exmaple: .data, .code, .src
    //  
    // Documentation links:
    // IMAGE_SECTION_HEADER: https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-image_section_header
    // Section Table (Section Headers): https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#section-table-section-headers
    // 
    // RETURN VALUE: 
    // std::optional<SECTIONS_LIST>. 
    // std::nullopt in case of error.
    // 
    //**********************************************************************************
    std::optional<SECTIONS_LIST> getSectionsList(IMAGE_NTPE_DATA& ntpe)
    {
        try
        {
            /* result vector of section directories */
            SECTIONS_LIST result;

            /* iterations through all image section headers poiners in IMAGE_NTPE_DATA structure */
            for (uint64_t sectionIndex = 0; sectionIndex < ntpe.fileHeader->NumberOfSections; sectionIndex++)
            {
                /* pushing IMAGE_SECTION_HEADER from iamge section headers */
                result.push_back(ntpe.sectionDirectories[sectionIndex]);
            }
            return result;
        }
        catch (std::exception&)
        {
        }
        /* returns nullopt in case of error */
        return std::nullopt;
    }
}