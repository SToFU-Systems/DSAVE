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
 * Commercial Licensing Contact: license@stofu.io
 */
#ifndef __IMPORT_H
#define __IMPORT_H

#include "NTPEParser.h"

namespace Import
{
    typedef std::map<std::string, std::set<std::string>> IMPORT_LIST;
    std::optional<IMPORT_LIST> GetAll                 (PBYTE pBase, uint64_t fileSize);
    bool                       removeNonNativeModules (Import::IMPORT_LIST& importList);
}

#endif

