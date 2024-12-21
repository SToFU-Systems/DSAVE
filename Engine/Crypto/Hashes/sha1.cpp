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

#include <array>
#include <cstdint>
#include <cstring>    // For memcpy and memset
#include <optional>

#include "sha1.h"

namespace SHA1 
{

    // Initial SHA-1 hash values
    constexpr std::array<uint32_t, 5> INITIAL_HASH_VALUES = {
        0x67452301,
        0xEFCDAB89,
        0x98BADCFE,
        0x10325476,
        0xC3D2E1F0
    };

    // Constants used in SHA-1
    constexpr std::array<uint32_t, 4> CONSTANTS = {
        0x5A827999,
        0x6ED9EBA1,
        0x8F1BBCDC,
        0xCA62C1D6
    };

    // Define the SHA-1 context structure
    struct sha1context {
        uint64_t total_bits;    // Total bits processed (bytes 0-7)
        uint32_t state[5];      // SHA-1 state (h0, h1, h2, h3, h4) (bytes 8-27)
        uint32_t buffer_len;    // Current length of the buffer (bytes 28-31)
        uint8_t  buffer[64];    // 64-byte buffer (bytes 32-95)
        // Padding or additional members can be added here if needed
    };  // Ensures no padding is added by the compiler



    // Perform a circular left rotation on a 32-bit value
    constexpr uint32_t leftRotate(uint32_t value, uint32_t shift) {
        return (value << shift) | (value >> (32 - shift));
    }

    // Helper function to read a 32-bit big-endian integer from a byte array
    uint32_t readBigEndian32(const uint8_t* data) {
        return (static_cast<uint32_t>(data[0]) << 24) |
            (static_cast<uint32_t>(data[1]) << 16) |
            (static_cast<uint32_t>(data[2]) << 8) |
            (static_cast<uint32_t>(data[3]));
    }

    // Helper function to write a 64-bit big-endian integer to a byte array
    void writeBigEndian64(uint8_t* data, uint64_t value) {
        for (int i = 0; i < 8; ++i) {
            data[7 - i] = static_cast<uint8_t>((value >> (i * 8)) & 0xFF);
        }
    }

    // Initialize the SHA-1 context
    hash_context init() 
    {
        hash_context ctx{};

        // Cast the raw byte array to sha1context
        sha1context* c = reinterpret_cast<sha1context*>(ctx.data());

        // Initialize total bits processed to zero
        c->total_bits = 0;

        // Initialize hash state
        std::memcpy(c->state, INITIAL_HASH_VALUES.data(), INITIAL_HASH_VALUES.size() * sizeof(uint32_t));

        // Initialize buffer length to zero
        c->buffer_len = 0;

        // Clear the buffer
        std::memset(c->buffer, 0, sizeof(c->buffer));

        return ctx;
    }

    // Process a single 512-bit (64-byte) chunk
    void processChunk(uint32_t state[5], const uint8_t chunk[64]) 
    {
        uint32_t a = state[0];
        uint32_t b = state[1];
        uint32_t c = state[2];
        uint32_t d = state[3];
        uint32_t e = state[4];

        uint32_t w[80];
        // Prepare the message schedule
        for (int i = 0; i < 16; ++i) {
            w[i] = (static_cast<uint32_t>(chunk[i * 4]) << 24) |
                (static_cast<uint32_t>(chunk[i * 4 + 1]) << 16) |
                (static_cast<uint32_t>(chunk[i * 4 + 2]) << 8) |
                (static_cast<uint32_t>(chunk[i * 4 + 3]));
        }
        for (int i = 16; i < 80; ++i) {
            w[i] = leftRotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
        }

        // Main loop
        for (int i = 0; i < 80; ++i) {
            uint32_t f, k;
            if (i < 20) {
                f = (b & c) | ((~b) & d);
                k = CONSTANTS[0];
            }
            else if (i < 40) {
                f = b ^ c ^ d;
                k = CONSTANTS[1];
            }
            else if (i < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = CONSTANTS[2];
            }
            else {
                f = b ^ c ^ d;
                k = CONSTANTS[3];
            }
            uint32_t temp = leftRotate(a, 5) + f + e + k + w[i];
            e = d;
            d = c;
            c = leftRotate(b, 30);
            b = a;
            a = temp;
        }

        // Update the state
        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;
        state[4] += e;
    }

    // Update the SHA-1 context with new data
    bool update(const hash_data_buf& buffer, uint32_t bufSize, hash_context& context, bool isFinalBlock)
    {
        if (bufSize > buffer.size())
            return false; // Check buffer size

        // Cast the raw byte array to sha1context
        sha1context* ctx = reinterpret_cast<sha1context*>(context.data());

        // Update total bits processed
        ctx->total_bits += static_cast<uint64_t>(bufSize) * 8;

        uint32_t buf_offset = 0;

        // If there is data left in the buffer from previous updates, fill it
        if (ctx->buffer_len > 0) {
            uint32_t to_copy = std::min(64 - ctx->buffer_len, bufSize);
            std::memcpy(ctx->buffer + ctx->buffer_len, buffer.data(), to_copy);
            ctx->buffer_len += to_copy;
            buf_offset += to_copy;
            bufSize -= to_copy;

            // If buffer is full, process it
            if (ctx->buffer_len == 64) {
                processChunk(ctx->state, ctx->buffer);
                ctx->buffer_len = 0;
            }
        }

        // Process all 64-byte blocks from the input buffer
        while (bufSize >= 64) {
            processChunk(ctx->state, buffer.data() + buf_offset);
            buf_offset += 64;
            bufSize -= 64;
        }

        // Copy any remaining data to the buffer
        if (bufSize > 0) {
            std::memcpy(ctx->buffer + ctx->buffer_len, buffer.data() + buf_offset, bufSize);
            ctx->buffer_len += bufSize;
        }

        // If it's the final block, handle padding
        if (isFinalBlock) {
            // Append the '1' bit (0x80) to the buffer
            ctx->buffer[ctx->buffer_len++] = 0x80;

            // If buffer_len > 56, need to process current buffer and pad another block
            if (ctx->buffer_len > 56) {
                // Pad the rest of the buffer with zeros
                std::memset(ctx->buffer + ctx->buffer_len, 0, 64 - ctx->buffer_len);
                processChunk(ctx->state, ctx->buffer);
                ctx->buffer_len = 0;
            }

            // Pad with zeros until buffer_len is 56
            if (ctx->buffer_len < 56) {
                std::memset(ctx->buffer + ctx->buffer_len, 0, 56 - ctx->buffer_len);
                ctx->buffer_len = 56;
            }

            // Append the total bits as a 64-bit big-endian integer
            writeBigEndian64(ctx->buffer + 56, ctx->total_bits);
            processChunk(ctx->state, ctx->buffer);

            // No more data
            ctx->buffer_len = 0;
        }

        return true;
    }

    // Finalize the SHA-1 computation and get the result
    hash_result final(hash_context& context)
    {
        hash_result digest{};

        // Cast the raw byte array to sha1context
        const sha1context* ctx = reinterpret_cast<const sha1context*>(context.data());

        // Convert state to bytes in big-endian order
        for (int i = 0; i < 5; ++i) {
            digest[i * 4] = (ctx->state[i] >> 24) & 0xFF;
            digest[i * 4 + 1] = (ctx->state[i] >> 16) & 0xFF;
            digest[i * 4 + 2] = (ctx->state[i] >> 8) & 0xFF;
            digest[i * 4 + 3] = ctx->state[i] & 0xFF;
        }

        return digest;
    }

} // namespace SHA1
