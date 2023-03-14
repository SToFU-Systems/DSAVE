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

#include <vector>
#include <string>

#include "3rd-party/polarssl/md5.h"
#include "3rd-party/polarssl/sha1.h"
#include "3rd-party/polarssl/sha4.h"
#include "3rd-party/ssdeep/ssdeep_wrap.h"
#include "3rd-party/tlsh/tlsh_wrap.h"

namespace math
{
    static const uint32_t kRelativeFreqValue = 10;

    struct HashData
    {
        BYTE  md5[16];
        BYTE  sha1[20];
        BYTE  sha512[64];
        std::string ssdeep;
        std::string tlsh;
    };

    struct MathData
    {
        float entropy;
        struct
        {
            uint64_t abs[0x100];
            uint16_t rel[0x100];
        } frequency;
        HashData hashes;
    };

    struct FuzzDiff
    {
        int ssdeep; 
        int tlsh; 
    };

    uint64_t alignUp (uint64_t value,   uint64_t align);
    MathData analyze (const void* base, uint64_t size);
    HashData hash    (const void* base, uint64_t size);
    FuzzDiff diff    (const HashData& a, const HashData& b);
}
