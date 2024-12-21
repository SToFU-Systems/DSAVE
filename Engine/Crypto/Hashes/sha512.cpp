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
#include "sha512.h"

namespace SHA512
{

    // SHA-512/384 Round Constants
    constexpr std::array<uint64_t, 80> SHA512_K = {
        0x428A2F98D728AE22ULL, 0x7137449123EF65CDULL, 0xB5C0FBCFEC4D3B2FULL, 0xE9B5DBA58189DBBCULL,
        0x3956C25BF348B538ULL, 0x59F111F1B605D019ULL, 0x923F82A4AF194F9BULL, 0xAB1C5ED5DA6D8118ULL,
        0xD807AA98A3030242ULL, 0x12835B0145706FBEULL, 0x243185BE4EE4B28CULL, 0x550C7DC3D5FFB4E2ULL,
        0x72BE5D74F27B896FULL, 0x80DEB1FE3B1696B1ULL, 0x9BDC06A725C71235ULL, 0xC19BF174CF692694ULL,
        0xE49B69C19EF14AD2ULL, 0xEFBE4786384F25E3ULL, 0x0FC19DC68B8CD5B5ULL, 0x240CA1CC77AC9C65ULL,
        0x2DE92C6F592B0275ULL, 0x4A7484AA6EA6E483ULL, 0x5CB0A9DCBD41FBD4ULL, 0x76F988DA831153B5ULL,
        0x983E5152EE66DFABULL, 0xA831C66D2DB43210ULL, 0xB00327C898FB213FULL, 0xBF597FC7BEEF0EE4ULL,
        0xC6E00BF33DA88FC2ULL, 0xD5A79147930AA725ULL, 0x06CA6351E003826FULL, 0x142929670A0E6E70ULL,
        0x27B70A8546D22FFCULL, 0x2E1B21385C26C926ULL, 0x4D2C6DFC5AC42AEDULL, 0x53380D139D95B3DFULL,
        0x650A73548BAF63DEULL, 0x766A0ABB3C77B2A8ULL, 0x81C2C92E47EDAEE6ULL, 0x92722C851482353BULL,
        0xA2BFE8A14CF10364ULL, 0xA81A664BBC423001ULL, 0xC24B8B70D0F89791ULL, 0xC76C51A30654BE30ULL,
        0xD192E819D6EF5218ULL, 0xD69906245565A910ULL, 0xF40E35855771202AULL, 0x106AA07032BBD1B8ULL,
        0x19A4C116B8D2D0C8ULL, 0x1E376C085141AB53ULL, 0x2748774CDF8EEB99ULL, 0x34B0BCB5E19B48A8ULL,
        0x391C0CB3C5C95A63ULL, 0x4ED8AA4AE3418ACBULL, 0x5B9CCA4F7763E373ULL, 0x682E6FF3D6B2B8A3ULL,
        0x748F82EE5DEFB2FCULL, 0x78A5636F43172F60ULL, 0x84C87814A1F0AB72ULL, 0x8CC702081A6439ECULL,
        0x90BEFFFA23631E28ULL, 0xA4506CEBDE82BDE9ULL, 0xBEF9A3F7B2C67915ULL, 0xC67178F2E372532BULL,
        0xCA273ECEEA26619CULL, 0xD186B8C721C0C207ULL, 0xEADA7DD6CDE0EB1EULL, 0xF57D4F7FEE6ED178ULL,
        0x06F067AA72176FBAULL, 0x0A637DC5A2C898A6ULL, 0x113F9804BEF90DAEULL, 0x1B710B35131C471BULL,
        0x28DB77F523047D84ULL, 0x32CAAB7B40C72493ULL, 0x3C9EBE0A15C9BEBCULL, 0x431D67C49C100D4CULL,
        0x4CC5D4BECB3E42B6ULL, 0x597F299CFC657E2AULL, 0x5FCB6FAB3AD6FAECULL, 0x6C44198C4A475817ULL
    };

    // SHA-512/384 Padding
    constexpr std::array<unsigned char, 128> SHA512_PADDING = {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    // Utility functions for big-endian conversions
    inline uint64_t GET_UINT64_BE(const unsigned char* buffer, size_t offset) {
        return (static_cast<uint64_t>(buffer[offset]) << 56) |
            (static_cast<uint64_t>(buffer[offset + 1]) << 48) |
            (static_cast<uint64_t>(buffer[offset + 2]) << 40) |
            (static_cast<uint64_t>(buffer[offset + 3]) << 32) |
            (static_cast<uint64_t>(buffer[offset + 4]) << 24) |
            (static_cast<uint64_t>(buffer[offset + 5]) << 16) |
            (static_cast<uint64_t>(buffer[offset + 6]) << 8) |
            (static_cast<uint64_t>(buffer[offset + 7]));
    }

    inline void PUT_UINT64_BE(uint64_t value, unsigned char* buffer, size_t offset) {
        buffer[offset] = static_cast<unsigned char>(value >> 56);
        buffer[offset + 1] = static_cast<unsigned char>(value >> 48);
        buffer[offset + 2] = static_cast<unsigned char>(value >> 40);
        buffer[offset + 3] = static_cast<unsigned char>(value >> 32);
        buffer[offset + 4] = static_cast<unsigned char>(value >> 24);
        buffer[offset + 5] = static_cast<unsigned char>(value >> 16);
        buffer[offset + 6] = static_cast<unsigned char>(value >> 8);
        buffer[offset + 7] = static_cast<unsigned char>(value);
    }

    // SHA-512/384 context structure
    struct SHA512Context {
        std::array<uint64_t, 8> state;
        uint64_t total;
        std::array<unsigned char, 128> buffer;
        bool is384;

        // Initialize context
        void init(bool is384_) {
            total = 0;
            is384 = is384_;
            if (!is384) {
                // SHA-512 initial values
                state = {
                    0x6A09E667F3BCC908ULL, 0xBB67AE8584CAA73BULL,
                    0x3C6EF372FE94F82BULL, 0xA54FF53A5F1D36F1ULL,
                    0x510E527FADE682D1ULL, 0x9B05688C2B3E6C1FULL,
                    0x1F83D9ABFB41BD6BULL, 0x5BE0CD19137E2179ULL
                };
            }
            else {
                // SHA-384 initial values
                state = {
                    0xCBBB9D5DC1059ED8ULL, 0x629A292A367CD507ULL,
                    0x9159015A3070DD17ULL, 0x152FECD8F70E5939ULL,
                    0x67332667FFC00B31ULL, 0x8EB44A8768581511ULL,
                    0xDB0C2E0D64F98FA7ULL, 0x47B5481DBEFA4FA4ULL
                };
            }
            buffer.fill(0);
        }

        // Rotate right
        static constexpr uint64_t rotr(uint64_t x, unsigned int n) {
            return (x >> n) | (x << (64 - n));
        }

        // SHA-512/384 transformation
        void process(const unsigned char data[128]) {
            std::array<uint64_t, 80> W;
            // Prepare message schedule
            for (size_t i = 0; i < 16; ++i) {
                W[i] = GET_UINT64_BE(data, i * 8);
            }
            for (size_t i = 16; i < 80; ++i) {
                uint64_t S0 = rotr(W[i - 15], 1) ^ rotr(W[i - 15], 8) ^ (W[i - 15] >> 7);
                uint64_t S1 = rotr(W[i - 2], 19) ^ rotr(W[i - 2], 61) ^ (W[i - 2] >> 6);
                W[i] = W[i - 16] + S0 + W[i - 7] + S1;
            }

            // Initialize working variables
            uint64_t A = state[0];
            uint64_t B = state[1];
            uint64_t C = state[2];
            uint64_t D = state[3];
            uint64_t E = state[4];
            uint64_t F = state[5];
            uint64_t G = state[6];
            uint64_t H = state[7];

            // Main compression loop
            for (size_t i = 0; i < 80; ++i) {
                uint64_t S1 = rotr(E, 14) ^ rotr(E, 18) ^ rotr(E, 41);
                uint64_t ch = (E & F) ^ (~E & G);
                uint64_t temp1 = H + S1 + ch + SHA512_K[i] + W[i];
                uint64_t S0 = rotr(A, 28) ^ rotr(A, 34) ^ rotr(A, 39);
                uint64_t maj = (A & B) ^ (A & C) ^ (B & C);
                uint64_t temp2 = S0 + maj;

                H = G;
                G = F;
                F = E;
                E = D + temp1;
                D = C;
                C = B;
                B = A;
                A = temp1 + temp2;
            }

            // Update state
            state[0] += A;
            state[1] += B;
            state[2] += C;
            state[3] += D;
            state[4] += E;
            state[5] += F;
            state[6] += G;
            state[7] += H;
        }

        // Update SHA-512/384 context with input data
        void update(const unsigned char* input, size_t length) {
            size_t fill = total % 128;
            total += length;

            if (fill && length >= (128 - fill)) {
                std::memcpy(buffer.data() + fill, input, 128 - fill);
                process(buffer.data());
                input += 128 - fill;
                length -= 128 - fill;
                fill = 0;
            }

            while (length >= 128) {
                process(input);
                input += 128;
                length -= 128;
            }

            if (length > 0) {
                std::memcpy(buffer.data() + fill, input, length);
            }
        }

        // Finalize SHA-512/384 and produce digest
        void finalize(unsigned char* output) {
            // Message length in bits
            uint64_t total_bits_low = total << 3;
            uint64_t total_bits_high = (total >> 61);

            // Append '1' bit and '0' bits
            size_t padn = (total % 128 < (is384 ? 112 : 112)) ?
                (112 - total % 128) :
                (240 - total % 128);
            update(SHA512_PADDING.data(), padn);

            // Append message length in bits
            std::array<unsigned char, 16> msglen;
            for (size_t i = 0; i < 8; ++i) {
                msglen[i] = static_cast<unsigned char>((total_bits_high >> (56 - i * 8)) & 0xFF);
                msglen[i + 8] = static_cast<unsigned char>((total_bits_low >> (56 - i * 8)) & 0xFF);
            }
            update(msglen.data(), 16);

            // Produce the final hash
            for (size_t i = 0; i < (is384 ? 6 : 8); ++i) {
                PUT_UINT64_BE(state[i], output, i * 8);
            }
        }
    };

    // Compute SHA-512 or SHA-384 hash of input data
    inline void sha512(const unsigned char* input, size_t length, unsigned char* output, bool is384 = false) {
        SHA512Context ctx;
        ctx.init(is384);
        ctx.update(input, length);
        ctx.finalize(output);
        std::memset(&ctx, 0, sizeof(ctx)); // Clear sensitive data
    }

    // SHA-512/384 HMAC implementation
    struct HMAC_SHA512Context {
        SHA512Context inner;
        SHA512Context outer;
        bool is384;

        // Initialize HMAC context with key
        void init(const unsigned char* key, size_t keylen, bool is384_) 
        {
            is384 = is384_;
            std::array<unsigned char, 128> ipad;
            std::array<unsigned char, 128> opad;
            ipad.fill(0x36);
            opad.fill(0x5C);

            if (keylen > 128) {
                unsigned char hash[64];
                sha512(key, keylen, hash, is384);
                key = hash;
                keylen = is384 ? 48 : 64;
            }

            for (size_t i = 0; i < keylen; ++i) {
                ipad[i] ^= key[i];
                opad[i] ^= key[i];
            }

            // Initialize inner SHA-512/384 context
            inner.init(is384);
            inner.update(ipad.data(), 128);

            // Initialize outer SHA-512/384 context
            outer.init(is384);
            outer.update(opad.data(), 128);
        }

        // Update HMAC with input data
        void update(const unsigned char* input, size_t length) {
            inner.update(input, length);
        }

        // Finalize HMAC and produce digest
        void finalize(unsigned char* output) {
            unsigned char inner_hash[64];
            inner.finalize(inner_hash);
            outer.update(inner_hash, is384 ? 48 : 64);
            outer.finalize(output);
            std::memset(inner_hash, 0, sizeof(inner_hash)); // Clear sensitive data
        }
    };

    // Compute HMAC-SHA-512 or HMAC-SHA-384
    inline void hmac_sha512(const unsigned char* key, size_t keylen,
        const unsigned char* input, size_t ilen,
        unsigned char* output, bool is384 = false) {
        HMAC_SHA512Context ctx;
        ctx.init(key, keylen, is384);
        ctx.update(input, ilen);
        ctx.finalize(output);
        std::memset(&ctx, 0, sizeof(ctx)); // Clear sensitive data
    }

    hash_context init()
    {
        hash_context ctx = {};
        SHA512Context* sha512ctx = (SHA512Context*)&ctx;
        sha512ctx->init(false);
        return ctx;
    }

    bool update(const hash_data_buf& buffer, uint32_t bufSize, hash_context& context, bool isFinalBlock)
    {
        SHA512Context* sha512ctx = (SHA512Context*)&context;
        sha512ctx->update((const unsigned char*)&buffer, bufSize);
        return true;
    };

    hash_result  final(hash_context& context)
    {
        hash_result result = {};
        SHA512Context* sha512ctx = (SHA512Context*)&context;
        sha512ctx->finalize((unsigned char*)&result);;
        return result;
    }

}