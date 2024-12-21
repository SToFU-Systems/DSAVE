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
#include <string>
#include "FSParser.h"

#ifdef _WIN32
#include <windows.h> // For Windows API
#elif __APPLE__
#include <dirent.h>  // For POSIX API
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/mount.h> // For macOS-specific filesystem
#else
#include <dirent.h>  // For POSIX API
#include <sys/stat.h>
#endif

namespace FileSystemParser {

    /*
     * Parser::Parser
     * Constructor initializes directory, file queues, queue guards, and starts parsing
     */
    Parser::Parser(const std::wstring& directory,
        filesqueue& fileQueue,
        filecallback callback,
        uint64_t maxQueuesSize)
        : directory(directory), flQueue(fileQueue), stopFlag(false), fileCallback(callback), maxQueueSize(maxQueueSize)
    {
        flQueue.reserve(maxQueuesSize);
        flQueue.set_max(maxQueuesSize);
        parserThread = std::thread(&Parser::parse, this); // Start parsing thread
    }

    /*
     * Parser::~Parser
     * Destructor stops parsing and joins the thread
     */
    Parser::~Parser()
    {
        if (parserThread.joinable()) 
        {
            stopFlag = true; // Set stop flag
            parserThread.join(); // Join the parsing thread
        }
    }

    /*
     * Parser::isFinished
     * Returns true if parsing is complete
     */
    bool Parser::isFinished()
    {
        return bFinished;
    }

    /*
     * Parser::parse
     * Main parsing logic
     */
    void Parser::parse()
    {
#ifdef _WIN32
        // Recursive function for Windows API
        std::function<void(const std::wstring&)> traverse = [this, &traverse](const std::wstring& dir) {
            WIN32_FIND_DATA findFileData;
            HANDLE hFind = FindFirstFile((dir + L"\\*").c_str(), &findFileData);

            if (hFind == INVALID_HANDLE_VALUE) {
                return; // Exit if directory handle is invalid
            }

            do {
                std::wstring name = findFileData.cFileName;
                if (name == L"." || name == L"..") {
                    continue; // Skip special entries
                }

                std::wstring fullPath = dir + L"\\" + name;
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    traverse(fullPath); // Recurse into subdirectory
                }
                else {
                    handleFile(fullPath); // Process file
                }
            } while (FindNextFile(hFind, &findFileData) != 0);

            FindClose(hFind); // Close directory handle
        };

        traverse(directory);
#elif __APPLE__
        // Recursive function for macOS using POSIX API
        std::function<void(const std::string&)> traverse = [this, &traverse](const std::string& dir) {
            DIR* dp = opendir(dir.c_str());
            if (!dp) {
                return; // Exit if directory cannot be opened
            }

            struct dirent* entry;
            while ((entry = readdir(dp)) != nullptr) {
                std::string name = entry->d_name;
                if (name == "." || name == "..") {
                    continue; // Skip special entries
                }

                std::string fullPath = dir + "/" + name;
                struct stat pathStat;
                if (stat(fullPath.c_str(), &pathStat) == -1) {
                    continue; // Skip if stat fails
                }

                if (S_ISDIR(pathStat.st_mode)) {
                    traverse(fullPath); // Recurse into subdirectory
                }
                else if (S_ISREG(pathStat.st_mode)) {
                    handleFile(std::wstring(fullPath.begin(), fullPath.end())); // Process file
                }
            }

            closedir(dp); // Close directory handle
        };

        traverse(std::string(directory.begin(), directory.end()));
#else
        // Recursive function for Linux using POSIX API
        std::function<void(const std::string&)> traverse = [this, &traverse](const std::string& dir) {
            DIR* dp = opendir(dir.c_str());
            if (!dp) {
                return; // Exit if directory cannot be opened
            }

            struct dirent* entry;
            while ((entry = readdir(dp)) != nullptr) {
                std::string name = entry->d_name;
                if (name == "." || name == "..") {
                    continue; // Skip special entries
                }

                std::string fullPath = dir + "/" + name;
                struct stat pathStat;
                if (stat(fullPath.c_str(), &pathStat) == -1) {
                    continue; // Skip if stat fails
                }

                if (S_ISDIR(pathStat.st_mode)) {
                    traverse(fullPath); // Recurse into subdirectory
                }
                else if (S_ISREG(pathStat.st_mode)) {
                    handleFile(std::wstring(fullPath.begin(), fullPath.end())); // Process file
                }
            }

            closedir(dp); // Close directory handle
        };

        traverse(std::string(directory.begin(), directory.end()));
#endif

        bFinished = true; // Mark parsing as complete
    }

    /*
     * Parser::handleFile
     * Processes a file and distributes it among multiple queues using round-robin
     */
    void Parser::handleFile(const std::wstring& filePath)
    {
        if (stopFlag) {
            return; // Exit if stop is signaled
        }

        if (fileCallback(filePath)) {
            flQueue.push_back(filePath);  // Add file to the current queue
        }
    }

}
