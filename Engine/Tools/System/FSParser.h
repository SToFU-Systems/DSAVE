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

#ifndef __FILESYSTEMPARSER_H
#define __FILESYSTEMPARSER_H

#include <mutex>
#include <thread>
#include <queue>
#include <string>
#include <optional>
#include <filesystem>
#include <functional>
#include "Tools/Containers/ThreadSafeVector.h"

namespace fs = std::filesystem;
namespace FileSystemParser
{
    using filesqueue    = tsvector<std::wstring>;
    using filecallback  = std::function<bool(const std::wstring&)>;
    class Parser
    {
    public:
        Parser( const std::wstring& directory, 
                filesqueue& _Out_   fileQueue,
                filecallback        callback,
                uint64_t            maxQueueSize = 0
        );
        ~Parser();
        bool isFinished();
    private:
        void parse();
        void handleFile(const std::wstring& filePath);
    private:
        filesqueue&                          flQueue;                 // Queues for file paths
        std::wstring                         directory;               // Directory to parse
        std::atomic<bool>                    stopFlag = false;        // Flag to stop parsing
        std::thread                          parserThread;            // Thread for file parsing
        uint64_t                             maxQueueSize;            // Maximum size of the queue (if 0 - queue size are not limited)
        filecallback                         fileCallback;            // Callback function
        bool                                 bFinished = false;       // Task is finished
    };
}
#endif
