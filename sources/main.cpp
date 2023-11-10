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
#include <fstream>
#include <windows.h>
#include <vector>
#include <string>
#include "resources.h"





math::MathData printHashes(std::wstring_view filePath)
{
    std::ifstream fileStream(filePath.data(), std::ios::binary);
    std::vector<BYTE> file(std::istreambuf_iterator<char>(fileStream), {});
    math::MathData info = math::analyze(file.data(), file.size());

    console::printcolor("File    : ", FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::wcout << filePath << std::endl;

    console::printcolor("MD5     : ", FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    console::printhex({ info.hashes.md5, info.hashes.md5 + sizeof(info.hashes.md5) }, 10, 80);

    console::printcolor("SHA1    : ", FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    console::printhex({ info.hashes.sha1, info.hashes.sha1 + sizeof(info.hashes.sha1) }, 17, 80);

    console::printcolor("SHA512  : ", FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    console::printhex({ info.hashes.sha512, info.hashes.sha512 + sizeof(info.hashes.sha512) }, 10, 80);

    console::printcolor("SSDEEP  : ", FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    console::printhex({ info.hashes.ssdeep.begin(), info.hashes.ssdeep.end() }, 10, 80);

    console::printcolor("TLSH    : ", FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    console::printhex({ info.hashes.tlsh.begin(), info.hashes.tlsh.end() }, 10, 80);

    std::cout << std::endl << std::endl;
    return info;
}


int main()
{
printf("\
Copyright(C) 2022, SToFU Systems S.L.\r\n\
All rights reserved.\r\n\
\r\n\
This program is free software; you can redistribute itand /or modify\r\n\
it under the terms of the GNU General Public License as published by\r\n\
the Free Software Foundation; either version 2 of the License, or\r\n\
(at your option) any later version.\r\n\
\r\n\
This program is distributed in the hope that it will be useful,\r\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\r\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the\r\n\
GNU General Public License for more details.\r\n\r\n\r\n");

    int argc = 0;
    wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    std::wstring fileName1 = argv[1];
    std::wstring fileName2 = argv[2];

    std::vector<BYTE> file1;
    std::vector<BYTE> file2;
    tools::readFile(fileName1, file1);
    tools::readFile(fileName2, file2);

    //  parse import
    auto imp = Import::GetAll(file1.data(), file1.size());

    //  parse resources
    auto res = Resources::GetAll(file1.data(), file1.size());
    
    //  hash calculator
    SetConsoleTitleA("DSAVE Hash calculator");
    auto f1 = printHashes(fileName1);
    auto f2 = printHashes(fileName2);

    auto diff = math::diff(f1.hashes, f2.hashes);

    console::printcolor("ssdeep distance: ", FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << std::dec << diff.ssdeep << std::endl;
    console::printcolor("tlsh   distance: ", FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << std::dec << diff.tlsh << std::endl;

    return 1; 
}

