/*
 *  Original implementation based on:
 *    XySSL: Copyright (C) 2006-2008 Christophe Devine
 *    PolarSSL: Copyright (C) 2009 Paul Bakker
 *
 *  Modifications and optimizations by SToFU Systems S.L.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of the original authors nor the names of their
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED "AS IS" BY SToFU SYSTEMS S.L. AND CONTRIBUTORS.
 *  ALL EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNERS OR CONTRIBUTORS
 *  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 *  OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 *  OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <cstdint>
#include <cstring>
#include "md5.h"

namespace MD5
{

    // MD5 padding table
    constexpr unsigned char MD5_PADDING[64] = {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    // Utility functions for little-endian conversions
    inline uint32_t GET_ULONG_LE(const unsigned char* b, int i) {
        return static_cast<uint32_t>(b[i]) |
            (static_cast<uint32_t>(b[i + 1]) << 8) |
            (static_cast<uint32_t>(b[i + 2]) << 16) |
            (static_cast<uint32_t>(b[i + 3]) << 24);
    }

    inline void PUT_ULONG_LE(uint32_t n, unsigned char* b, int i) {
        b[i] = static_cast<unsigned char>(n);
        b[i + 1] = static_cast<unsigned char>(n >> 8);
        b[i + 2] = static_cast<unsigned char>(n >> 16);
        b[i + 3] = static_cast<unsigned char>(n >> 24);
    }

    // MD5 context structure
    struct MD5Context {
        uint32_t state[4];
        uint64_t total;
        unsigned char buffer[64];
    };

    // Left rotate function
    constexpr inline uint32_t ROTL(uint32_t x, int n) {
        return (x << n) | (x >> (32 - n));
    }

    // MD5 auxiliary functions
    constexpr inline uint32_t F(uint32_t x, uint32_t y, uint32_t z) {
        return (z ^ (x & (y ^ z)));
    }

    constexpr inline uint32_t G(uint32_t x, uint32_t y, uint32_t z) {
        return (y ^ (z & (x ^ y)));
    }

    constexpr inline uint32_t H(uint32_t x, uint32_t y, uint32_t z) {
        return (x ^ y ^ z);
    }

    constexpr inline uint32_t I(uint32_t x, uint32_t y, uint32_t z) {
        return (y ^ (x | ~z));
    }

    // MD5 transformation
    inline void MD5Transform(MD5Context& ctx, const unsigned char block[64]) {
        uint32_t X[16];
        for (int i = 0; i < 16; ++i) {
            X[i] = GET_ULONG_LE(block, i * 4);
        }

        uint32_t A = ctx.state[0];
        uint32_t B = ctx.state[1];
        uint32_t C = ctx.state[2];
        uint32_t D = ctx.state[3];

        // Round 1
#define ROUND1(a, b, c, d, k, s, t) \
        a += F(b, c, d) + X[k] + t; \
        a = ROTL(a, s) + b;

        ROUND1(A, B, C, D, 0, 7, 0xD76AA478);
        ROUND1(D, A, B, C, 1, 12, 0xE8C7B756);
        ROUND1(C, D, A, B, 2, 17, 0x242070DB);
        ROUND1(B, C, D, A, 3, 22, 0xC1BDCEEE);
        ROUND1(A, B, C, D, 4, 7, 0xF57C0FAF);
        ROUND1(D, A, B, C, 5, 12, 0x4787C62A);
        ROUND1(C, D, A, B, 6, 17, 0xA8304613);
        ROUND1(B, C, D, A, 7, 22, 0xFD469501);
        ROUND1(A, B, C, D, 8, 7, 0x698098D8);
        ROUND1(D, A, B, C, 9, 12, 0x8B44F7AF);
        ROUND1(C, D, A, B, 10, 17, 0xFFFF5BB1);
        ROUND1(B, C, D, A, 11, 22, 0x895CD7BE);
        ROUND1(A, B, C, D, 12, 7, 0x6B901122);
        ROUND1(D, A, B, C, 13, 12, 0xFD987193);
        ROUND1(C, D, A, B, 14, 17, 0xA679438E);
        ROUND1(B, C, D, A, 15, 22, 0x49B40821);

        // Round 2
#define ROUND2(a, b, c, d, k, s, t) \
        a += G(b, c, d) + X[k] + t; \
        a = ROTL(a, s) + b;

        ROUND2(A, B, C, D, 1, 5, 0xF61E2562);
        ROUND2(D, A, B, C, 6, 9, 0xC040B340);
        ROUND2(C, D, A, B, 11, 14, 0x265E5A51);
        ROUND2(B, C, D, A, 0, 20, 0xE9B6C7AA);
        ROUND2(A, B, C, D, 5, 5, 0xD62F105D);
        ROUND2(D, A, B, C, 10, 9, 0x02441453);
        ROUND2(C, D, A, B, 15, 14, 0xD8A1E681);
        ROUND2(B, C, D, A, 4, 20, 0xE7D3FBC8);
        ROUND2(A, B, C, D, 9, 5, 0x21E1CDE6);
        ROUND2(D, A, B, C, 14, 9, 0xC33707D6);
        ROUND2(C, D, A, B, 3, 14, 0xF4D50D87);
        ROUND2(B, C, D, A, 8, 20, 0x455A14ED);
        ROUND2(A, B, C, D, 13, 5, 0xA9E3E905);
        ROUND2(D, A, B, C, 2, 9, 0xFCEFA3F8);
        ROUND2(C, D, A, B, 7, 14, 0x676F02D9);
        ROUND2(B, C, D, A, 12, 20, 0x8D2A4C8A);

        // Round 3
#define ROUND3(a, b, c, d, k, s, t) \
        a += H(b, c, d) + X[k] + t; \
        a = ROTL(a, s) + b;

        ROUND3(A, B, C, D, 5, 4, 0xFFFA3942);
        ROUND3(D, A, B, C, 8, 11, 0x8771F681);
        ROUND3(C, D, A, B, 11, 16, 0x6D9D6122);
        ROUND3(B, C, D, A, 14, 23, 0xFDE5380C);
        ROUND3(A, B, C, D, 1, 4, 0xA4BEEA44);
        ROUND3(D, A, B, C, 4, 11, 0x4BDECFA9);
        ROUND3(C, D, A, B, 7, 16, 0xF6BB4B60);
        ROUND3(B, C, D, A, 10, 23, 0xBEBFBC70);
        ROUND3(A, B, C, D, 13, 4, 0x289B7EC6);
        ROUND3(D, A, B, C, 0, 11, 0xEAA127FA);
        ROUND3(C, D, A, B, 3, 16, 0xD4EF3085);
        ROUND3(B, C, D, A, 6, 23, 0x04881D05);
        ROUND3(A, B, C, D, 9, 4, 0xD9D4D039);
        ROUND3(D, A, B, C, 12, 11, 0xE6DB99E5);
        ROUND3(C, D, A, B, 15, 16, 0x1FA27CF8);
        ROUND3(B, C, D, A, 2, 23, 0xC4AC5665);

        // Round 4
#define ROUND4(a, b, c, d, k, s, t) \
        a += I(b, c, d) + X[k] + t; \
        a = ROTL(a, s) + b;

        ROUND4(A, B, C, D, 0, 6, 0xF4292244);
        ROUND4(D, A, B, C, 7, 10, 0x432AFF97);
        ROUND4(C, D, A, B, 14, 15, 0xAB9423A7);
        ROUND4(B, C, D, A, 5, 21, 0xFC93A039);
        ROUND4(A, B, C, D, 12, 6, 0x655B59C3);
        ROUND4(D, A, B, C, 3, 10, 0x8F0CCC92);
        ROUND4(C, D, A, B, 10, 15, 0xFFEFF47D);
        ROUND4(B, C, D, A, 1, 21, 0x85845DD1);
        ROUND4(A, B, C, D, 8, 6, 0x6FA87E4F);
        ROUND4(D, A, B, C, 15, 10, 0xFE2CE6E0);
        ROUND4(C, D, A, B, 6, 15, 0xA3014314);
        ROUND4(B, C, D, A, 13, 21, 0x4E0811A1);
        ROUND4(A, B, C, D, 4, 6, 0xF7537E82);
        ROUND4(D, A, B, C, 11, 10, 0xBD3AF235);
        ROUND4(C, D, A, B, 2, 15, 0x2AD7D2BB);
        ROUND4(B, C, D, A, 9, 21, 0xEB86D391);

        // Update state
        ctx.state[0] += A;
        ctx.state[1] += B;
        ctx.state[2] += C;
        ctx.state[3] += D;

        // Cleanup
#undef ROUND1
#undef ROUND2
#undef ROUND3
#undef ROUND4
    }

    // Initialize MD5 context
    inline void MD5Init(MD5Context& ctx) {
        ctx.state[0] = 0x67452301;
        ctx.state[1] = 0xEFCDAB89;
        ctx.state[2] = 0x98BADCFE;
        ctx.state[3] = 0x10325476;
        ctx.total = 0;
    }

    // Update MD5 context with input data
    inline void MD5Update(MD5Context& ctx, const unsigned char* input, size_t len) {
        size_t fill = (ctx.total % 64);
        ctx.total += len;

        if (fill && (len >= (64 - fill))) {
            std::memcpy(ctx.buffer + fill, input, 64 - fill);
            MD5Transform(ctx, ctx.buffer);
            input += 64 - fill;
            len -= 64 - fill;
            fill = 0;
        }

        while (len >= 64) {
            MD5Transform(ctx, input);
            input += 64;
            len -= 64;
        }

        if (len > 0) {
            std::memcpy(ctx.buffer + fill, input, len);
        }
    }

    // Finalize MD5 and produce digest
    inline void MD5Final(MD5Context& ctx, unsigned char output[16]) {
        unsigned char msglen[8];
        uint64_t total_bits = ctx.total * 8;

        // Encode total length in little endian
        for (int i = 0; i < 8; ++i) {
            msglen[i] = static_cast<unsigned char>((total_bits >> (8 * i)) & 0xFF);
        }

        // Padding
        size_t padn = (ctx.total % 64 < 56) ? (56 - ctx.total % 64) : (120 - ctx.total % 64);
        MD5Update(ctx, MD5_PADDING, padn);
        MD5Update(ctx, msglen, 8);

        // Output state in little endian
        for (int i = 0; i < 4; ++i) {
            PUT_ULONG_LE(ctx.state[i], output, i * 4);
        }
    }

    // Compute MD5 digest of input data
    inline void MD5(const unsigned char* input, size_t len, unsigned char output[16])
    {
        MD5Context ctx;
        MD5Init(ctx);
        MD5Update(ctx, input, len);
        MD5Final(ctx, output);
    }

    hash_context init()
    {
        hash_context ctx = {};
        MD5Init(*(MD5Context*)&ctx);
        return ctx;
    };

    bool update(const hash_data_buf& buffer, uint32_t bufSize, hash_context& context, bool isFinalBlock)
    {
        MD5Context* md5ctx = (MD5Context*)&context;
        MD5Update(*md5ctx, (const unsigned char*)&buffer, bufSize);
        return true;
    };

    hash_result  final(hash_context& context)
    {
        MD5Context* md5ctx = (MD5Context*)&context;
        hash_result result = {};
        MD5Final(*md5ctx, (unsigned char*)&result);
        return result;
    }
}