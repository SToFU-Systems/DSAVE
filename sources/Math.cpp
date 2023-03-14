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
#include "Math.h"

namespace math
{
    uint64_t alignUp(uint64_t value, uint64_t align)
    {
        uint64_t mod = value % align;
        return value + (mod ? (align - mod) : 0);
    };


    double entropy(const void* buff, uint64_t size)
    {
        double entropy = 0;
        if (!buff || !size)
            return 0;

        uint64_t abc[0x100] = {};
        for (size_t i = 0; i < size; ++i)
            ++abc[((unsigned char*)buff)[i]];

        //          255
        // H(x) = - Sum( f_i * log2(f_i) )
        //          i=0

        for (uint64_t i = 0; i < _countof(abc); i++)
        {
            double f = static_cast<double>(abc[i]) / static_cast<double>(size);
            entropy -= f * log2(f);
        }

        return entropy;
    }


    HashData hash(const void* buff, uint64_t size)
    {
        HashData snap = {};

        hashes::md5(buff, size,  snap.md5);
        hashes::sha1(buff, size, snap.sha1);
        hashes::sha4(buff, size, snap.sha512);

        snap.ssdeep = hashes::ssdeepHash(buff, size);
        snap.tlsh   = hashes::tlshHash(buff, size);

        return snap;
    }


    MathData analyze(const void* buff, uint64_t size)
    {
        MathData snap = {};
        if (!buff || !size)
            return snap;

        static const uint32_t kMaxBytes = 0x100;
        uint64_t maxAbsFreq = 0;

        for (uint64_t i = 0; i < size; i++)
            snap.frequency.abs[((const unsigned char*)buff)[i]]++;

        for (uint64_t i = 0; i < kMaxBytes; i++)
            maxAbsFreq = max(maxAbsFreq, snap.frequency.abs[i]);

        for (uint64_t i = 0; i < kMaxBytes; i++)
            snap.frequency.rel[i] = (snap.frequency.abs[i] * kRelativeFreqValue) / maxAbsFreq;

        snap.hashes  = hash(buff, size);
        snap.entropy = entropy(buff, size);

        return snap;
    }


    FuzzDiff diff(const HashData& a, const HashData& b)
    {
        FuzzDiff res;
        res.ssdeep = hashes::ssdeepDiff(a.ssdeep, b.ssdeep);
        res.tlsh   = hashes::tlshDiff(a.tlsh, b.tlsh);
        return res;
    }

}