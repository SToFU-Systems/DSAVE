#include "stdafx.h"
#include "FileOps.h"

namespace Files
{
    /**
    * @brief Reads the entire content of a file into a byte vector.
    *
    * @param path Wide string path to the file.
    * @return std::vector<uint8_t> Vector containing the file data.
    *         Returns an empty vector on failure.
    */
    std::vector<uint8_t> readFullFile(const std::wstring& path)
    {
        std::vector<uint8_t> fileData;

        try
        {
            // Construct a filesystem path from the wide string
            std::filesystem::path filePath(path);

            // Open the file in binary mode and position the cursor at the end
            std::ifstream fileStream(filePath, std::ios::binary | std::ios::ate);
            if (!fileStream)
                return fileData;

            // Determine the size of the file
            std::ifstream::pos_type fileSize = fileStream.tellg();
            if (fileSize <= 0)
                return fileData;

            // Resize the vector to hold the file data
            fileData.resize(static_cast<size_t>(fileSize));

            // Seek back to the beginning of the file
            fileStream.seekg(0, std::ios::beg);

            // Read the file content into the vector
            if (!fileStream.read(reinterpret_cast<char*>(fileData.data()), fileSize))
            {
                // Read operation failed
                fileData.clear();
                return fileData;
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            // Handle filesystem-related errors
            // Consider logging e.what() in a real-world scenario
            return std::vector<uint8_t>();
        }
        catch (...)
        {
            // Suppress all other exceptions
         
            return std::vector<uint8_t>();
        }
        return fileData;
    }
}