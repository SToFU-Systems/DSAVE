#pragma once
#include "NTPEParser.h"
#include "ImportScanner.h"

namespace Scanner
{
    struct FileContext
    {
        std::wstring        path;
        uint64_t            hash;
        double              entropy;
        ntpe::IMPORT_LIST   impTable;
        ntpe::SECTIONS_LIST secTable;
    };
}