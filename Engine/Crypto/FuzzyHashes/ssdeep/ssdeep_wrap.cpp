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

#include <iostream>
#include "ssdeep_wrap.h"
#include "ssdeep.h" // Include the actual SSDEEP library header

// Namespace containing SSDEEP related functions
namespace SSDEEP
{
    struct SsdeepCtx
    {
        fuzzy_state* data;
    };

    hash_context init()
    {
        hash_context ctx = {};
        SsdeepCtx* ssdeepCtx = (SsdeepCtx*)&ctx;
        ssdeepCtx->data = fuzzy_new();
        return ctx;
    }

    bool update(const hash_data_buf& buffer, uint32_t bufSize, hash_context& context, bool isFinalBlock)
    {
        SsdeepCtx* ssdeepCtx = (SsdeepCtx*)&context;
        fuzzy_update(ssdeepCtx->data, (const unsigned char*)&buffer, bufSize);
        return true;
    }

    hash_result final(hash_context& context)
    {
        hash_result result = {};
        SsdeepCtx* ssdeepCtx = (SsdeepCtx*)&context;
        fuzzy_digest(ssdeepCtx->data, (char*)&result, 0);
        fuzzy_free(ssdeepCtx->data);
        return result;
    }

    int diff(const hash_result& hash1, const hash_result& hash2)
    {
        return fuzzy_compare((const char*)&hash1, (const char*)&hash2);
    }
}
