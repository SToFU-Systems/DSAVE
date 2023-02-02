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
    struct IMAGE_NTPE_DATA
    {
        PCHAR                       fileBase;
        PIMAGE_FILE_HEADER          fileHeader;
        PIMAGE_DATA_DIRECTORY       dataDirectories;
        PIMAGE_SECTION_HEADER       sectionDirectories;
        DWORD                       SecAlign;
        WORD                        CellSize;			//	size of cell of LookupTable/AddressTable
    };

    typedef std::map<std::string, std::set<std::string>> IMPORT_LIST;
    typedef std::vector<IMAGE_SECTION_HEADER> SECTIONS_LIST;

    DWORD                          alignUp              (DWORD value, DWORD align);
    std::optional<IMAGE_NTPE_DATA> getNTPEData          (char* fileMapBase, uint64_t fileSize);
    std::optional<SECTIONS_LIST>   getSectionsList      (IMAGE_NTPE_DATA& ntpe);
    std::optional<IMPORT_LIST>     getImportList        (IMAGE_NTPE_DATA& ntpe);
    std::optional<IMPORT_LIST>	   getImportList        (std::wstring_view filePath);
}


