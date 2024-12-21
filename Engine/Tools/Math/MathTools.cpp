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
#include "Math.h"
#include <cstdint>

namespace Math 
{

    /**
     * alignUp - Align a 64-bit value upwards to the nearest multiple of aligner
     * @value: The value to align
     * @aligner: The alignment boundary (must be a power of two)
     *
     * Returns the smallest multiple of aligner greater than or equal to value.
     */
    uint64_t alignUp(uint64_t value, uint64_t aligner)
    {
        if (!aligner)
            return value;
        if (!value)
            return aligner;

        if ((aligner & (aligner - 1)) == 0)
            return (value + aligner - 1) & ~(aligner - 1);

        uint64_t remainder = value % aligner;
        return remainder ? value + (aligner - remainder) : value;
    }

    /**
     * alignUpPtr - Align a pointer upwards to the nearest multiple of aligner
     * @value: The pointer to align
     * @aligner: The alignment boundary in bytes (must be a power of two)
     *
     * Returns the aligned pointer.
     */
    uint8_t* alignUpPtr(uint8_t* value, uint64_t aligner)
    {
        uintptr_t addr = reinterpret_cast<uintptr_t>(value);
        uintptr_t aligned = alignUp(addr, aligner);
        return reinterpret_cast<uint8_t*>(aligned);
    }

    /**
     * alignDown - Align a 64-bit value downwards to the nearest multiple of aligner
     * @value: The value to align
     * @aligner: The alignment boundary (must be a power of two)
     *
     * Returns the largest multiple of aligner less than or equal to value.
     */
    uint64_t alignDown(uint64_t value, uint64_t aligner)
    {
        if (!aligner)
            return value;

        if ((aligner & (aligner - 1)) == 0)
            return value & ~(aligner - 1);

        return value - (value % aligner);
    }

    /**
     * alignDownPtr - Align a pointer downwards to the nearest multiple of aligner
     * @value: The pointer to align
     * @aligner: The alignment boundary in bytes (must be a power of two)
     *
     * Returns the aligned pointer.
     */
    uint8_t* alignDownPtr(uint8_t* value, uint64_t aligner)
    {
        uintptr_t addr = reinterpret_cast<uintptr_t>(value);
        uintptr_t aligned = alignDown(addr, aligner);
        return reinterpret_cast<uint8_t*>(aligned);
    }

} // namespace Math
