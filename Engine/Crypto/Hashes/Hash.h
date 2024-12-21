#ifndef __HASH_H
#define __HASH_H

#include "../HashGlobals.h"
#include "crc32.h"
#include "md5.h"
#include "sha1.h"
#include "sha256.h"
#include "sha512.h"
#include "../FuzzyHashes/ssdeep/ssdeep_wrap.h"
#include "../FuzzyHashes/tlsh/tlsh_wrap.h"

namespace HASHES
{
	static const uint32_t USE_CRC32		= 1 << 0;
	static const uint32_t USE_MD5		= 1 << 1;
	static const uint32_t USE_SHA1		= 1 << 2;
	static const uint32_t USE_SHA256	= 1 << 3;
	static const uint32_t USE_SHA512	= 1 << 4;
	static const uint32_t USE_SSDEEP	= 1 << 5;
	static const uint32_t USE_TLSH		= 1 << 7;

	typedef struct 
	{
		hash_context crc32;
		hash_context md5;
		hash_context sha1;
		hash_context sha256;
		hash_context sha512;
		hash_context ssdeep;
		hash_context tlsh;
		uint32_t flEnabledHashes;
	} HashContext;

	typedef struct 
	{
		hash_result crc32;
		hash_result md5;
		hash_result sha1;
		hash_result sha256;
		hash_result sha512;
		hash_result ssdeep;
		hash_result tlsh;
		uint32_t flEnabledHashes;
	} HashResult;

	HashContext  init	(uint32_t flUsesHashes = -1);
	bool		 update	(const hash_data_buf& buffer, uint32_t bufSize, HashContext& context, bool isFinalBlock = false);
	HashResult   final	(HashContext& context);
	HashResult hashData	(const std::vector<uint8_t>& buffer, uint32_t flUsesHashes = -1);	// based on init, update, final
	HashResult hashFile	(const std::wstring& filePath, uint32_t flUsesHashes = -1);			// based on init, update, final
}

#endif

