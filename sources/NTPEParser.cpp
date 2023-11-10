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
    uint64_t rva2offset(const ntpe::IMAGE_NTPE_CONTEXT& ntpe, uint64_t rva)
    {
        /* retrieve first section */
        try
        {
            /* walk on sections */
            for (uint64_t secIndex = 0; secIndex < ntpe.ntHeader64->FileHeader.NumberOfSections; secIndex++)
            {
                PIMAGE_SECTION_HEADER sec = ntpe.sectionDirectories + secIndex;
                DWORD secEnd = math::alignUp(sec->Misc.VirtualSize, ntpe.SecAlign) + sec->VirtualAddress;
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
    PBYTE ntstdHeader       = (PBYTE)fileHeader + sizeof(IMAGE_FILE_HEADER); \
    data.fileBase           = fileMapBase; \
    data.fileSize           = fileSize; \
    data.ntHeader64         = (PIMAGE_NT_HEADERS64)peSignature; \
    HeaderType* optHeader   = (HeaderType*)ntstdHeader; \
    data.sectionDirectories = (PIMAGE_SECTION_HEADER)(ntstdHeader + sizeof(HeaderType)); \
    data.SecAlign           = optHeader->SectionAlignment; \
    data.FileAlign          = optHeader->FileAlignment; \
    data.dataDirectories    = optHeader->DataDirectory; \
    data.CellSize           = cellSize;	\
    }
    std::optional<IMAGE_NTPE_CONTEXT> getNTPEContext(PBYTE fileMapBase, uint64_t fileSize)
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
            if ((PBYTE)peSignature <= fileMapBase || (PBYTE)peSignature - fileMapBase >= fileSize)
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
            IMAGE_NTPE_CONTEXT data = {};

            /* addresses of PIMAGE_SECTION_HEADER, PIMAGE_DATA_DIRECTORIES, SectionAlignment, CellSize depending on processor architecture */
            switch (fileHeader->Machine)
            {
            case IMAGE_FILE_MACHINE_I386:
                initNTPE(IMAGE_OPTIONAL_HEADER32, 4);
                break;

            case IMAGE_FILE_MACHINE_AMD64:
                initNTPE(IMAGE_OPTIONAL_HEADER64, 8);
                break;

            default:
                return std::nullopt;
            }

            return data;
        }
        catch (std::exception&)
        {
        }
        return std::nullopt;
    }

    std::optional<IMAGE_NTPE_DATA> getNTPEData(const ntpe::IMAGE_NTPE_CONTEXT& ntCtx)
    {
        try
        {
            ntpe::IMAGE_NTPE_DATA data;
            data.dosHeader  = *(PIMAGE_DOS_HEADER)(ntCtx.fileBase);
            data.ntHeader64 = *ntCtx.ntHeader64;
            for (uint64_t secIndex = 0; secIndex < ntCtx.ntHeader64->FileHeader.NumberOfSections; secIndex++)
                data.secHeaders.push_back(*(ntCtx.sectionDirectories + secIndex));
        }
        catch (std::exception&)
        {
        }
        return std::nullopt;
    }


}

