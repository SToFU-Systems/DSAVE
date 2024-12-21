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
 * Commercial Licensing Contact: license@stofu.io
 */

#include "stdafx.h"

namespace ntpe
{
    uint64_t RvaToOffset(PBYTE pBase, uint64_t rva)
    {
        __try
        {
            IMAGE_DOS_HEADER* pDosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(pBase);
            IMAGE_NT_HEADERS* pNtHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(pBase + pDosHeader->e_lfanew);
            IMAGE_SECTION_HEADER* pSectionHeaders = IMAGE_FIRST_SECTION(pNtHeaders);

            if (pNtHeaders->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64 &&
                pNtHeaders->FileHeader.Machine != IMAGE_FILE_MACHINE_I386)
                return ERROR_INVALID_OFFSET;

            uint64_t sectionAlignment = pNtHeaders->FileHeader.Machine == IMAGE_FILE_MACHINE_I386 ?
                reinterpret_cast<IMAGE_NT_HEADERS64*>(pNtHeaders)->OptionalHeader.SectionAlignment :
                reinterpret_cast<IMAGE_NT_HEADERS32*>(pNtHeaders)->OptionalHeader.SectionAlignment;

            for (UINT i = 0; i < pNtHeaders->FileHeader.NumberOfSections; ++i)
            {
                uint64_t sectionStartRVA = pSectionHeaders[i].VirtualAddress;
                uint64_t sectionEndRVA = math::alignUp(sectionStartRVA + pSectionHeaders[i].Misc.VirtualSize, sectionAlignment);

                if (rva >= sectionStartRVA && rva < sectionEndRVA)
                {
                    uint64_t sectionStartRAW = pSectionHeaders[i].PointerToRawData;
                    return rva - sectionStartRVA + sectionStartRAW;
                }
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
        }
        return ERROR_INVALID_OFFSET;
    }


    PBYTE RvaToRaw(PBYTE pBase, uint64_t rva)
    {
        uint64_t offset = RvaToOffset(pBase, rva);
        if (offset == ERROR_INVALID_OFFSET)
            return (PBYTE)ERROR_INVALID_ADDRESS;
        return pBase + offset;
    }


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

