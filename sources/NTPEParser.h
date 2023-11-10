#pragma once

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

#include <fstream>
#include "Tools.h"

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

    uint64_t                           rva2offset       (const ntpe::IMAGE_NTPE_CONTEXT& ntpeCtx, uint64_t rva);
    std::optional<IMAGE_NTPE_CONTEXT>  getNTPEContext   (PBYTE fileMapBase, uint64_t fileSize);
    std::optional<IMAGE_NTPE_DATA>     getNTPEData      (const IMAGE_NTPE_CONTEXT& ntCtx);
}


