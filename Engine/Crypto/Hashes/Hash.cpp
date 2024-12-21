#include "stdafx.h"
#include "hash.h"

namespace HASHES
{
    // Initialize all hash contexts and set enabled hashes
    HashContext init(uint32_t flUsesHashes)
    {
        HashContext ctx{};
        ctx.flEnabledHashes = flUsesHashes;

        if (ctx.flEnabledHashes & USE_CRC32)
            ctx.crc32 = CRC32::init();

        if (ctx.flEnabledHashes & USE_MD5)
            ctx.md5 = MD5::init();

        if (ctx.flEnabledHashes & USE_SHA1)
            ctx.sha1 = SHA1::init();

        if (ctx.flEnabledHashes & USE_SHA256)
            ctx.sha256 = SHA256::init();

        if (ctx.flEnabledHashes & USE_SHA512)
            ctx.sha512 = SHA512::init();

        if (ctx.flEnabledHashes & USE_SSDEEP)
            ctx.ssdeep = SSDEEP::init();

        if (ctx.flEnabledHashes & USE_TLSH)
            ctx.tlsh = TLSH::init();

        return ctx;
    }

    // Update all enabled hash contexts with the provided buffer
    bool update(const hash_data_buf& buffer, uint32_t bufSize, HashContext& context, bool isFinalBlock)
    {
        if (bufSize > buffer.size())
            return false; // Check buffer size

        if (context.flEnabledHashes & USE_CRC32)
            CRC32::update(buffer, bufSize, context.crc32, isFinalBlock);

        if (context.flEnabledHashes & USE_MD5)
            MD5::update(buffer, bufSize, context.md5, isFinalBlock);

        if (context.flEnabledHashes & USE_SHA1)
            SHA1::update(buffer, bufSize, context.sha1, isFinalBlock);

        if (context.flEnabledHashes & USE_SHA256)
            SHA256::update(buffer, bufSize, context.sha256, isFinalBlock);

        if (context.flEnabledHashes & USE_SHA512)
            SHA512::update(buffer, bufSize, context.sha512, isFinalBlock);

        if (context.flEnabledHashes & USE_SSDEEP)
            SSDEEP::update(buffer, bufSize, context.ssdeep, isFinalBlock);

        if (context.flEnabledHashes & USE_TLSH)
            TLSH::update(buffer, bufSize, context.tlsh, isFinalBlock);

        return true;
    }

    // Finalize all enabled hash contexts and collect results
    HashResult final(HashContext& context)
    {
        HashResult result{};
        result.flEnabledHashes = context.flEnabledHashes;

        if (context.flEnabledHashes & USE_CRC32)
            result.crc32 = CRC32::final(context.crc32);

        if (context.flEnabledHashes & USE_MD5)
            result.md5 = MD5::final(context.md5);

        if (context.flEnabledHashes & USE_SHA1)
            result.sha1 = SHA1::final(context.sha1);

        if (context.flEnabledHashes & USE_SHA256)
            result.sha256 = SHA256::final(context.sha256);

        if (context.flEnabledHashes & USE_SHA512)
            result.sha512 = SHA512::final(context.sha512);

        if (context.flEnabledHashes & USE_SSDEEP)
            result.ssdeep = SSDEEP::final(context.ssdeep);

        if (context.flEnabledHashes & USE_TLSH)
            result.tlsh = TLSH::final(context.tlsh);

        return result;
    }

    // Compute hashes for a data buffer based on enabled hash flags
    HashResult hashData(const std::vector<uint8_t>& buffer, uint32_t flUsesHashes)
    {
        HashContext ctx = init();

        // If flUsesHashes is -1, enable all hashes
        if (flUsesHashes == static_cast<uint32_t>(-1))
            flUsesHashes = USE_CRC32 | USE_MD5 | USE_SHA1 | USE_SHA256 | USE_SHA512 | USE_SSDEEP | USE_TLSH;

        ctx.flEnabledHashes = flUsesHashes;

        hash_data_buf dataBuffer{};
        size_t copySize = buffer.size() > dataBuffer.size() ? dataBuffer.size() : buffer.size();
        std::memcpy(dataBuffer.data(), buffer.data(), copySize);

        update(dataBuffer, static_cast<uint32_t>(copySize), ctx, true);
        return final(ctx);
    }

    // Compute hashes for a file based on enabled hash flags
    HashResult hashFile(const std::wstring& filePath, uint32_t flUsesHashes)
    {
        HashContext ctx = init();

        ctx.flEnabledHashes = flUsesHashes;

        std::ifstream file(filePath, std::ios::binary);
        if (!file)
            return final(ctx); // Return empty result if file cannot be opened

        hash_data_buf buffer{};
        while (file)
        {
            file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
            std::streamsize bytesRead = file.gcount();
            if (bytesRead > 0)
            {
                update(buffer, static_cast<uint32_t>(bytesRead), ctx, false);
            }
        }

        update(buffer, 0, ctx, true); // Finalize with any remaining data
        return final(ctx);
    }
} // namespace HASHES
