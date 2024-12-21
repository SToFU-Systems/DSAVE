/**
 * Modified levenshtein distance calculation
 *
 * This program can be used, redistributed or modified under any of
 * Boost Software License 1.0, GPL v2 or GPL v3
 * See the file COPYING for details.
 *
 * $Id$
 *
 * Copyright (C) 2014 kikairoya <kikairoya@gmail.com>
 * Copyright (C) 2014 Jesse Kornblum <research@jessekornblum.com>
 */

#include <cstddef>

namespace SSDEEP

{


#define EDIT_DISTN_MAXLEN 64 /* MAX_SPAMSUM */
#define EDIT_DISTN_INSERT_COST 1
#define EDIT_DISTN_REMOVE_COST 1
#define EDIT_DISTN_REPLACE_COST 2

#define MIN(x,y) ((x)<(y)?(x):(y))


int edit_distn(const char* s1, size_t s1len, const char* s2, size_t s2len) {
    int t[2][EDIT_DISTN_MAXLEN + 1];
    int* t1 = t[0];
    int* t2 = t[1];
    int* t3;
    size_t i1, i2;
    for (i2 = 0; i2 <= s2len; i2++)
        t[0][i2] = static_cast<int>(i2 * EDIT_DISTN_REMOVE_COST);
    for (i1 = 0; i1 < s1len; i1++) {
        t2[0] = static_cast<int>((i1 + 1) * EDIT_DISTN_INSERT_COST);
        for (i2 = 0; i2 < s2len; i2++) {
            int cost_a = t1[i2 + 1] + EDIT_DISTN_INSERT_COST;
            int cost_d = t2[i2] + EDIT_DISTN_REMOVE_COST;
            int cost_r = t1[i2] + (s1[i1] == s2[i2] ? 0 : EDIT_DISTN_REPLACE_COST);
            t2[i2 + 1] = MIN(MIN(cost_a, cost_d), cost_r);
        }
        t3 = t1;
        t1 = t2;
        t2 = t3;
    }
    return t1[s2len];
}

}