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
#include "NTPEParser.h"

namespace importKB
{
    struct ApiData
    {
        uint32_t cnt = 0;
    };
    struct ModData
    {
        uint32_t cnt = 0;
        std::map<std::string, ApiData> ApiMap;
    };

    typedef std::vector<ntpe::IMPORT_LIST> IMPORT_KNOWLEDGE_BASE;
    typedef std::map<std::string, ModData> IMPORT_STAT;
    
    IMPORT_STAT           createStatisticBase(const IMPORT_KNOWLEDGE_BASE& knowledgeBase);
    IMPORT_KNOWLEDGE_BASE createKnowledgeBase(const tools::FILES_LIST& fileList);
}