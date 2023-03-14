#pragma once

namespace console
{
    void printcolor (std::string str, uint32_t color);
    void printhex   (const std::vector<BYTE>& data, uint32_t minPos = 0, uint32_t maxPos = 80);
}
