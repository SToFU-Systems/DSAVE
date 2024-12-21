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
#include "tlsh_wrap.h"
#include "tlsh.h" // Include the actual TLSH library header


// Namespace containing TLSH related functions
namespace TLSH
{
    struct TlshCtx
    {
        Tlsh* data; 
    };

    hash_context init()
    {
        hash_context ctx = {};
        TlshCtx* tlshCtx = (TlshCtx*)&ctx;
        tlshCtx->data = new Tlsh();
        return ctx;
    }

    bool update(const hash_data_buf& buffer, uint32_t bufSize, hash_context& context, bool isFinalBlock)
    {
        TlshCtx* tlshCtx = (TlshCtx*)&context;
        tlshCtx->data->update((const unsigned char*)&buffer, bufSize);
        return true;
    }

    hash_result final(hash_context& context)
    {
        hash_result result  = {};
        TlshCtx* tlshCtx    = (TlshCtx*)&context;
        tlshCtx->data->final();
        tlshCtx->data->getHash((char*)&result, result.size() - 1);
        delete tlshCtx->data;
        return result;
    }

    int diff(const hash_result& hash1, const hash_result& hash2)
    {
        Tlsh t1, t2;
        t1.fromTlshStr((const char*)&hash1);
        t2.fromTlshStr((const char*)&hash2);
        return t1.totalDiff(&t2);
    }


}
