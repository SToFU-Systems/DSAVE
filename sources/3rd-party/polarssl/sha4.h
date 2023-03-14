/**
 * \file sha4.h
 *
 *  Copyright (C) 2006-2010, Brainspark B.V.
 *
 *  This file is part of PolarSSL (http://www.polarssl.org)
 *  Lead Maintainer: Paul Bakker <polarssl_maintainer at polarssl.org>
 *
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

 /*
 *  This file is a modified version of sha4.h, which is licensed under the GNU General Public License(GPL) version 2.
 *  The original file can be found at https://www.polarssl.org
 *  The modified file is licensed under the GPL version 2.
 */

#ifndef POLARSSL_SHA4_H
#define POLARSSL_SHA4_H

namespace hashes 
{

#include <stdint.h>

#if defined(_MSC_VER) || defined(__WATCOMC__)
  #define UL64(x) x##ui64
  #define int64 __int64
#else
  #define UL64(x) x##ULL
  #define int64 long long
#endif

/**
 * \brief          SHA-512 context structure
 */
typedef struct
{
    uint64_t total[2];          /*!< number of bytes processed  */
    uint64_t state[8];          /*!< intermediate digest state  */
    unsigned char buffer[128];  /*!< data block being processed */

    unsigned char ipad[128];    /*!< HMAC: inner padding        */
    unsigned char opad[128];    /*!< HMAC: outer padding        */
}
sha4_context;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          Output = SHA-512( input buffer )
 *
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   SHA-384/512 checksum result
 */
void sha4( const void* input, uint64_t ilen, unsigned char output[64]);


#ifdef __cplusplus
}
#endif

}

#endif /* sha4.h */
