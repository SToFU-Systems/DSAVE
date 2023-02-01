#pragma once

#include "Tools.h"
#include "NTPEParser.h"
#include "importKB.h"


namespace ImportScanner
{
    struct FileContext
    {
        uint64_t          hash; 
        std::wstring      filePath;
        ntpe::IMPORT_LIST impTable;
    };
}