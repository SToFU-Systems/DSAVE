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
#include <array>
#include "sha256.h"

namespace SHA256
{

    // SHA-256 constants
    constexpr std::array<uint32_t, 64> SHA256_K = {
        0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5,
        0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
        0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
        0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
        0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC,
        0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
        0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7,
        0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
        0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
        0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
        0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,
        0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
        0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5,
        0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
        0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
        0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
    };

    // SHA-256 padding
    constexpr std::array<unsigned char, 64> SHA256_PADDING = {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    // Utility functions for big-endian conversions
    inline uint32_t GET_UINT32_BE(const unsigned char* buffer, size_t offset) {
        return (static_cast<uint32_t>(buffer[offset]) << 24) |
            (static_cast<uint32_t>(buffer[offset + 1]) << 16) |
            (static_cast<uint32_t>(buffer[offset + 2]) << 8) |
            (static_cast<uint32_t>(buffer[offset + 3]));
    }

    inline void PUT_UINT32_BE(uint32_t value, unsigned char* buffer, size_t offset) {
        buffer[offset] = static_cast<unsigned char>(value >> 24);
        buffer[offset + 1] = static_cast<unsigned char>(value >> 16);
        buffer[offset + 2] = static_cast<unsigned char>(value >> 8);
        buffer[offset + 3] = static_cast<unsigned char>(value);
    }

    // SHA-256 context structure
    struct SHA256Context 
    {
        std::array<uint32_t, 8> state;
        uint64_t total;
        std::array<unsigned char, 64> buffer;

        // Initialize context
        void init(bool is224) 
        {
            total = 0;
            if (!is224) {
                // SHA-256 initial values
                state = { 0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
                          0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19 };
            }
            else {
                // SHA-224 initial values
                state = { 0xC1059ED8, 0x367CD507, 0x3070DD17, 0xF70E5939,
                          0xFFC00B31, 0x68581511, 0x64F98FA7, 0xBEFA4FA4 };
            }
        }

        // Rotate right
        static constexpr uint32_t rotr(uint32_t x, unsigned int n) {
            return (x >> n) | (x << (32 - n));
        }

        // SHA-256 transformation
        void process(const unsigned char data[64]) {
            std::array<uint32_t, 64> W;
            // Prepare message schedule
            for (size_t i = 0; i < 16; ++i) {
                W[i] = GET_UINT32_BE(data, i * 4);
            }
            for (size_t i = 16; i < 64; ++i) {
                uint32_t s0 = rotr(W[i - 15], 7) ^ rotr(W[i - 15], 18) ^ (W[i - 15] >> 3);
                uint32_t s1 = rotr(W[i - 2], 17) ^ rotr(W[i - 2], 19) ^ (W[i - 2] >> 10);
                W[i] = W[i - 16] + s0 + W[i - 7] + s1;
            }

            // Initialize working variables
            uint32_t a = state[0];
            uint32_t b = state[1];
            uint32_t c = state[2];
            uint32_t d = state[3];
            uint32_t e = state[4];
            uint32_t f = state[5];
            uint32_t g = state[6];
            uint32_t h = state[7];

            // Main compression loop
            for (size_t i = 0; i < 64; ++i) {
                uint32_t S1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
                uint32_t ch = (e & f) ^ (~e & g);
                uint32_t temp1 = h + S1 + ch + SHA256_K[i] + W[i];
                uint32_t S0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
                uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
                uint32_t temp2 = S0 + maj;

                h = g;
                g = f;
                f = e;
                e = d + temp1;
                d = c;
                c = b;
                b = a;
                a = temp1 + temp2;
            }

            // Update state
            state[0] += a;
            state[1] += b;
            state[2] += c;
            state[3] += d;
            state[4] += e;
            state[5] += f;
            state[6] += g;
            state[7] += h;
        }

        // Update SHA-256 context with input data
        void update(const unsigned char* input, size_t length) {
            size_t fill = static_cast<size_t>(total) % 64;
            total += length;

            if (fill && length >= (64 - fill)) {
                std::memcpy(buffer.data() + fill, input, 64 - fill);
                process(buffer.data());
                input += 64 - fill;
                length -= 64 - fill;
                fill = 0;
            }

            while (length >= 64) {
                process(input);
                input += 64;
                length -= 64;
            }

            if (length > 0) {
                std::memcpy(buffer.data() + fill, input, length);
            }
        }

        // Finalize SHA-256 and produce digest
        void finalize(unsigned char output[32], bool is224) {
            // Message length in bits
            uint64_t total_bits = total * 8;

            // Append '1' bit and '0' bits
            size_t padn = (static_cast<size_t>(total) % 64 < (is224 ? 56 : 56)) ?
                (56 - static_cast<size_t>(total) % 64) :
                (120 - static_cast<size_t>(total) % 64);
            update(SHA256_PADDING.data(), padn);

            // Append message length in bits
            std::array<unsigned char, 8> msglen;
            for (size_t i = 0; i < 8; ++i) {
                msglen[7 - i] = static_cast<unsigned char>(total_bits >> (i * 8));
            }
            update(msglen.data(), 8);

            // Produce the final hash
            for (size_t i = 0; i < (is224 ? 7 : 8); ++i) {
                PUT_UINT32_BE(state[i], output, i * 4);
            }
        }
    };

    // Compute SHA-256 or SHA-224 hash of input data
    inline void sha256(const unsigned char* input, size_t length, unsigned char output[32], bool is224 = false) {
        SHA256Context ctx;
        ctx.init(is224);
        ctx.update(input, length);
        ctx.finalize(output, is224);
    }

    // SHA-256 HMAC implementation
    struct SHA256HMACContext {
        SHA256Context inner;
        SHA256Context outer;
        bool is224;

        // Initialize HMAC context with key
        void init(const unsigned char* key, size_t keylen, bool is224_) {
            is224 = is224_;
            std::array<unsigned char, 64> ipad = { 0 };
            std::array<unsigned char, 64> opad = { 0 };

            if (keylen > 64) {
                unsigned char hash[32];
                sha256(key, keylen, hash, is224);
                key = hash;
                keylen = is224 ? 28 : 32;
            }

            // Initialize inner and outer pads
            std::memset(ipad.data(), 0x36, 64);
            std::memset(opad.data(), 0x5C, 64);
            for (size_t i = 0; i < keylen; ++i) {
                ipad[i] ^= key[i];
                opad[i] ^= key[i];
            }

            // Initialize inner SHA-256 context
            inner.init(is224);
            inner.update(ipad.data(), 64);

            // Initialize outer SHA-256 context
            outer.init(is224);
            outer.update(opad.data(), 64);
        }

        // Update HMAC with input data
        void update(const unsigned char* input, size_t length) {
            inner.update(input, length);
        }

        // Finalize HMAC and produce digest
        void finalize(unsigned char output[32]) {
            unsigned char inner_hash[32];
            inner.finalize(inner_hash, is224);
            outer.update(inner_hash, is224 ? 28 : 32);
            outer.finalize(output, is224);
            std::memset(inner_hash, 0, sizeof(inner_hash));
        }
    };

    // Compute HMAC-SHA-256 or HMAC-SHA-224
    inline void hmac_sha256(const unsigned char* key, size_t keylen,
        const unsigned char* input, size_t ilen,
        unsigned char output[32], bool is224 = false) {
        SHA256HMACContext ctx;
        ctx.init(key, keylen, is224);
        ctx.update(input, ilen);
        ctx.finalize(output);
        // Clear sensitive data
        std::memset(&ctx, 0, sizeof(ctx));
    }

    hash_context init()
    {
        hash_context ctx = {};
        SHA256Context* sha256ctx = (SHA256Context*)&ctx;
        sha256ctx->init(false);
        return ctx;
        return ctx;
    };

    bool update(const hash_data_buf& buffer, uint32_t bufSize, hash_context& context, bool isFinalBlock)
    {
        SHA256Context* sha256ctx = (SHA256Context*)&context;
        sha256ctx->update((const unsigned char*)&buffer, bufSize);
        return true;
    };

    hash_result  final(hash_context& context)
    {
        hash_result result = {};
        SHA256Context* sha256ctx = (SHA256Context*)&context;
        sha256ctx->finalize((unsigned char*)&result, false);
        return result;
    }

}