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
#include "FileTypes.h"

namespace Files
{
	Graph<FileType> TypeTable;

    // Initializes the TypeTable with magic byte sequences and their file types
    bool initializeTypeTable() 
    {
        const FileTypeRecord* recordArray = fileTypeArray.records;

        // Iterate over the records in fileTypeArray
        for (const auto& record : fileTypeArray.records) 
        {
            if (record.fileType == FileType::ext_unknown) 
            {
                // Stop processing if the file type is unknown
                break;
            }

            // Extract the magic byte sequence
            std::vector<uint8_t> magicBytes(record.MagicNumbers, record.MagicNumbers + record.numberOfNumbers);

            // Add the magic byte sequence and its associated file type to the graph
            TypeTable[magicBytes] = record.fileType;
        }

        return true; // Successfully initialized
    }

    // Determines the file type based on the magic byte sequence from the file
    FileType getFileType(const std::vector<uint8_t>& fileData) 
    {
        // Extract the first few bytes (up to maxMagicSequenceSize) for comparison
        std::vector<uint8_t> magicBytes(fileData.begin(), fileData.begin() + std::min<size_t>(fileData.size(), maxMagicSequenceSize));

        // Find the most relevant match in the TypeTable
        auto relevantEntry = TypeTable.findRelevant(magicBytes);

        // Return the associated file type if found, otherwise return unknown
        if (relevantEntry == TypeTable.end()) {
            return FileType::ext_unknown;
        }
        return relevantEntry->second;
    }
}


