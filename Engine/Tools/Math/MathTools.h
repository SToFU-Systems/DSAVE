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

#ifndef __MATH_TOOLS_H
#define __MATH_TOOLS_H

#include <stdint.h>
#include <math.h>
#include <algorithm>

namespace Math
{
	uint64_t alignUp		(uint64_t value, uint64_t aligner);
	uint8_t* alignUpPtr		(uint8_t* value, uint64_t aligner);
	uint64_t alignDown		(uint64_t value, uint64_t aligner);
	uint8_t* alignDownPtr	(uint8_t* value, uint64_t aligner);
}

#endif 
