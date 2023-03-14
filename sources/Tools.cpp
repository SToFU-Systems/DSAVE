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
#include "Tools.h"

namespace tools
{
namespace fs = std::filesystem;

bool readFile(std::wstring_view filePath, _Out_ std::vector<char>& fileData)
{
    bool result = false;
    static const uint64_t kBlockSize = 0x100000;
    HANDLE file = 0, map = 0;
    PVOID  data = 0;

    try
    {
        file = CreateFileW(filePath.data(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
        if (file != INVALID_HANDLE_VALUE)
        {
            LARGE_INTEGER fileSize = {};
            if (fileSize.LowPart = GetFileSize(file, (PDWORD)&fileSize.HighPart))
            {
                if (map = CreateFileMappingW(file, 0, PAGE_READONLY, 0, 0, 0))
                {
                    if (data = MapViewOfFile(map, FILE_MAP_READ, 0, 0, 0))
                    {
                        fileData.resize(fileSize.QuadPart);
                        memcpy(fileData.data(), data, fileSize.QuadPart);
                        result = true;
                        UnmapViewOfFile(data);
                        data = 0;
                    }
                    CloseHandle(map);
                    map = 0;
                }
            }
            CloseHandle(file);
            file = INVALID_HANDLE_VALUE;
        }
        return result;
    }
    catch (std::exception&)
    {
        if (data != 0)
            UnmapViewOfFile(data);
        if (map != 0)
            CloseHandle(map);
        if (file != 0 && file != INVALID_HANDLE_VALUE)
            CloseHandle(file);
        return false;
    }
}

FILES_LIST getFilesList(std::wstring_view directory, std::wstring_view matchSpec)
{
    FILES_LIST filesList;
    std::error_code ec{};

    /* if path is directiry */
    if (fs::is_directory(directory, ec))
    {
        /* iterator for revursive directory traversal */
        for (auto const& file : fs::recursive_directory_iterator{ directory, fs::directory_options::skip_permission_denied, ec })
        {
            for (const auto& it : matchSpec)
            {
                if (S_OK == PathMatchSpecExW(file.path().wstring().c_str(), matchSpec.data(), PMSF_MULTIPLE))
                {
                    filesList.emplace(file.path());
                }
            }
        }
    }
    return filesList;
}

std::wstring pathToFullLwr(std::wstring_view path)
{
    std::vector<wchar_t> fullPath(MAX_LONG_PATH_LEN);
    if (GetFullPathNameW(path.data(), fullPath.size(), fullPath.data(), nullptr))
    {
        _wcslwr_s(fullPath.data(), fullPath.size());
        return fullPath.data();
    }
    return {};
}

std::wstring getDirectoryPathFromFilePath(std::wstring_view filePath)
{
    std::vector<wchar_t> fullPath(32 * 0x400);
    wchar_t* filePart = nullptr;

    if (GetFullPathNameW(filePath.data(), fullPath.size(), fullPath.data(), &filePart))
        if (filePart)
            *filePart = 0;

    return fullPath.data();
}

std::wstring toUtf16(std::string_view utf8)
{
    if (utf8.empty())
        return std::wstring();

    std::wstring utf16;
    utf16.resize(utf8.size() * 2);
    int count = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), utf16.data(), static_cast<int>(utf16.size()));
    if (count == 0)
        return std::wstring();

    utf16.resize(count);
    return utf16;
}

//**********************************************************************************
// FUNCTION: entropy(const char* buff, uint64_t buffSize)
// 
// ARGS:
// const void* pBuff - poinet to a buffer.
// uint64_t dwSize   - buffer size.
// 
// DESCRIPTION: 
// Returns information entrophy of given buffer. 
// 
// Links:
// Entropy: https://en.wikipedia.org/wiki/Entropy_(information_theory) 
// 
// RETURN VALUE: 
// uint64_t - entropy of buffer.
// 
//**********************************************************************************
double entropy(const char* buff, uint64_t buffSize)
{
    double entropy = 0;
    if (buff == nullptr || buffSize == 0) 
        return 0;

    /* statistic of characters from pBuff */
    uint64_t statistic[256] = {};
    for (uint32_t i = 0; i < buffSize; i++) 
        statistic[buff[i]]++;

    /* entropy calculation by formula */
    for (uint32_t i = 0; i < 256; i++)
    {
        if (double b = (double)statistic[i] / buffSize)
            entropy += b * (log(b) / log(2));
    };
    /* cast entropy to uint64_t */
    return (entropy * -1);
};

}