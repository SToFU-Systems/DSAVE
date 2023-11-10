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

#pragma once

namespace Resources
{

struct ResourceInfo
{
    DWORD Size;     // Size of the resource data
    PBYTE data;     // Offset of the resource data from the beginning of the file

    union {
        WORD TypeID;                        // Resource type ID or
        PIMAGE_RESOURCE_DIR_STRING_U Type;  // resource type
        
    };

    union {
        WORD NameID;                        // Resource name ID or
        PIMAGE_RESOURCE_DIR_STRING_U Name;  // resource name
    };

    WORD  Language; // Language of the resource
};

std::optional<std::vector<ResourceInfo>> GetAll(BYTE* pBase, uint64_t fileSize);

}