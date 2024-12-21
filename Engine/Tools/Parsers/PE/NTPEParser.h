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

#ifndef __NTPEPARSER_H
#define __NTPEPARSER_H

#include <fstream>
#include "PEStructures.h"

#define ERROR_INVALID_ADDRESS (PBYTE)-1
#define ERROR_INVALID_OFFSET  -1

namespace ntpe
{
    struct IMAGE_NTPE_CONTEXT
    {
        PBYTE                       fileBase;
        size_t                      fileSize;
        union {
            PIMAGE_NT_HEADERS32     ntHeader32;
            PIMAGE_NT_HEADERS64     ntHeader64;
        };
        PIMAGE_DATA_DIRECTORY       dataDirectories;
        PIMAGE_SECTION_HEADER       sectionDirectories;
        DWORD                       SecAlign;
        DWORD                       FileAlign;
        WORD                        CellSize;			//	size of cell of LookupTable/AddressTable
    };

    struct IMAGE_NTPE_DATA
    {
        IMAGE_DOS_HEADER dosHeader;
        union {
            IMAGE_NT_HEADERS32 ntHeader32;
            IMAGE_NT_HEADERS64 ntHeader64;
        };
        std::vector<IMAGE_SECTION_HEADER> secHeaders;
    };
    uint64_t                           RvaToOffset      (PBYTE pBase, uint64_t rva);
    PBYTE                              RvaToRaw         (PBYTE pBase, uint64_t rva);
    std::optional<IMAGE_NTPE_CONTEXT>  getNTPEContext   (PBYTE fileMapBase, uint64_t fileSize);
    std::optional<IMAGE_NTPE_DATA>     getNTPEData      (const IMAGE_NTPE_CONTEXT& ntCtx);
}

#endif

