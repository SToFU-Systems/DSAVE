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
#include "Tools/Math/MathTools.h"
#include "PEStructures.h"

#define ERROR_INVALID_ADDRESS (uint8_t*)-1
#define ERROR_INVALID_OFFSET  -1

namespace ntpe
{
    struct IMAGE_NTPE_CONTEXT
    {
        uint8_t*                       fileBase;
        uint32_t                       fileSize;
        uint32_t                       SecAlign;
        uint32_t                       FileAlign;
        uint32_t                       CellSize;			//	size of cell of LookupTable/AddressTable

        //  pointers
        ImageDosHeader*           dosHeader; 
        ImageFileHeader*          fileHeader;

        union {
            ImageNtHeaders32* ntHeader32;
            ImageNtHeaders64* ntHeader64;
        };
        ImageDataDirectory*       dataDirectories;
        ImageSectionHeader*       sectionDirectories;
    };

    uint64_t                           RvaToOffset      (uint8_t* pBase, uint64_t rva);
    uint8_t*                           RvaToRaw         (uint8_t* pBase, uint64_t rva);
    std::optional<IMAGE_NTPE_CONTEXT>  getNTPEContext   (uint8_t* fileMapBase, uint64_t fileSize);
}

#endif

