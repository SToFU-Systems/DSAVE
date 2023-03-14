#include "stdafx.h"
#include "console.h"

namespace console
{
    void printcolor(std::string str, uint32_t color)
    {
        HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFOEX info = {}; 
        info.cbSize = sizeof(info);
        GetConsoleScreenBufferInfoEx(stdOut, &info);
        WORD oldAttributes = info.wAttributes;
        info.wAttributes = color; 
        SetConsoleScreenBufferInfoEx(stdOut, &info);
        std::cout << str;
        info.wAttributes = oldAttributes;
        SetConsoleScreenBufferInfoEx(stdOut, &info);
    };

    void printhex(const std::vector<BYTE>& data, uint32_t minPos, uint32_t maxPos)
    {
        DWORD byteIndex = 0;
        for (uint32_t byteIndex = 0; byteIndex < data.size(); byteIndex++)
        {
            if ((byteIndex % (maxPos / 2)) == 0 && byteIndex)
            {
                std::cout << std::endl;
                for (uint32_t i = 0; i < minPos; i++)
                    std::cout << " ";
            }

            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data.at(byteIndex));
        }
        std::cout << std::endl;
    }
}