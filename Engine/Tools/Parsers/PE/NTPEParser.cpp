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
#include "NTPEParser.h"
#include "PEStructures.h"

namespace ntpe
{
    uint64_t RvaToOffset(uint8_t* pBase, uint64_t rva)
    {
        try
        {
            ImageDosHeader* pDosHeader = reinterpret_cast<ImageDosHeader*>(pBase);
            ImageNtHeaders32* pNtHeaders = reinterpret_cast<ImageNtHeaders32*>(pBase + pDosHeader->e_lfanew);
            ImageSectionHeader* pSectionHeaders = IMAGE_FIRST_SECTION(pNtHeaders);

            if (pNtHeaders->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64 &&
                pNtHeaders->FileHeader.Machine != IMAGE_FILE_MACHINE_I386)
                return ERROR_INVALID_OFFSET;

            uint64_t sectionAlignment = pNtHeaders->FileHeader.Machine == IMAGE_FILE_MACHINE_I386 ?
                reinterpret_cast<ImageNtHeaders64*>(pNtHeaders)->OptionalHeader.SectionAlignment :
                reinterpret_cast<ImageNtHeaders32*>(pNtHeaders)->OptionalHeader.SectionAlignment;

            for (uint32_t i = 0; i < pNtHeaders->FileHeader.NumberOfSections; ++i)
            {
                uint64_t sectionStartRVA = pSectionHeaders[i].VirtualAddress;
                uint64_t sectionEndRVA = Math::alignUp(sectionStartRVA + pSectionHeaders[i].Misc.VirtualSize, sectionAlignment);

                if (rva >= sectionStartRVA && rva < sectionEndRVA)
                {
                    uint64_t sectionStartRAW = pSectionHeaders[i].PointerToRawData;
                    return rva - sectionStartRVA + sectionStartRAW;
                }
            }
        }
        catch (std::exception&)
        {
        }
        return ERROR_INVALID_OFFSET;
    }


    uint8_t* RvaToRaw(uint8_t* pBase, uint64_t rva)
    {
        uint64_t offset = RvaToOffset(pBase, rva);
        if (offset == ERROR_INVALID_OFFSET)
            return (uint8_t*)ERROR_INVALID_ADDRESS;
        return pBase + offset;
    }


    /**
    * @brief Retrieves the NTPE context from a mapped PE file.
    *
    * @param fileMapBase Pointer to the base of the mapped file.
    * @param fileSize    Size of the mapped file.
    * @return std::optional<IMAGE_NTPE_CONTEXT> Returns the context if successful, std::nullopt otherwise.
    */
    std::optional<IMAGE_NTPE_CONTEXT> getNTPEContext(uint8_t* fileMapBase, uint64_t fileSize)
    {
        try 
        {
            // Validate input pointers and file size
            if (!fileMapBase || fileSize < sizeof(ImageDosHeader*))
                return std::nullopt;

            // Initialize DOS header
            ImageDosHeader* dosHeader = reinterpret_cast<ImageDosHeader*>(fileMapBase);
            if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
                return std::nullopt;

            // Ensure e_lfanew is within file bounds
            if (dosHeader->e_lfanew + sizeof(uint32_t) > fileSize)
                return std::nullopt;

            // Initialize NT headers signature
            uint32_t signature = *reinterpret_cast<uint32_t*>(fileMapBase + dosHeader->e_lfanew);
            if (signature != IMAGE_NT_SIGNATURE)
                return std::nullopt;

            // Determine if PE is 32-bit or 64-bit
            ImageNtHeaders32* ntHeader32 = reinterpret_cast<ImageNtHeaders32*>(fileMapBase + dosHeader->e_lfanew);
            ImageNtHeaders64* ntHeader64 = nullptr;
            bool is64Bit = false;

            if (ntHeader32->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
            {
                is64Bit = true;
                ntHeader64 = reinterpret_cast<ImageNtHeaders64*>(ntHeader32);
            }
            else if (ntHeader32->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC)
            {
                return std::nullopt;
            }

            // Populate IMAGE_NTPE_CONTEXT
            IMAGE_NTPE_CONTEXT context{};
            context.fileBase       = fileMapBase;
            context.fileSize       = fileSize;
            context.dosHeader      = dosHeader;
            context.fileHeader     = &ntHeader32->FileHeader;
            context.SecAlign       = ntHeader32->OptionalHeader.SectionAlignment;
            context.FileAlign      = ntHeader32->OptionalHeader.FileAlignment;
            context.CellSize       = (is64Bit)?8:4; // Set appropriately if used

            if (is64Bit)
            {
                context.ntHeader64       = ntHeader64;
                context.dataDirectories  = ntHeader64->OptionalHeader.DataDirectory;
            }
            else
            {
                context.ntHeader32       = ntHeader32;
                context.dataDirectories  = ntHeader32->OptionalHeader.DataDirectory;
            }

            // Calculate section headers position
            uint32_t numberOfSections = context.fileHeader->NumberOfSections;
            size_t sectionHeaderOffset = dosHeader->e_lfanew +
                                            sizeof(uint32_t) +
                                            sizeof(ImageFileHeader*) +
                                            ntHeader32->FileHeader.SizeOfOptionalHeader;

            if (sectionHeaderOffset + numberOfSections * sizeof(ImageSectionHeader*) > fileSize)
                return std::nullopt;

            context.sectionDirectories = reinterpret_cast<ImageSectionHeader*>(fileMapBase + sectionHeaderOffset);

            return context;
        }
        catch (...)
        {
            return std::nullopt;
        }
    }

}

