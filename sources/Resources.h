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

bool GetAll(BYTE* pBase, uint64_t fileSize, std::vector<ResourceInfo>& resources);

}