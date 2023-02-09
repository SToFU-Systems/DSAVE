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

namespace tools
{
    typedef std::set<std::wstring> FILES_LIST;

    uint64_t alignUp(uint64_t value, uint64_t align);
    bool readFile(std::wstring_view filePath, _Out_ std::vector<char>& fileData);
    std::set<std::wstring> getFilesList(std::wstring_view directory, std::wstring_view matchSpec);
    std::wstring toUtf16(std::string_view utf8);
    std::wstring pathToFullLwr(std::wstring_view path);
    std::wstring getDirectoryPathFromFilePath(std::wstring_view filePath);

}