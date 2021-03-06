/* Copyright (c) 2010 Sophos Group.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __SXE_UTIL_H__
#define __SXE_UTIL_H__

#include "sxe-log.h"

#define SXE_UNSIGNED_MAXIMUM        (~0U)
#define SXE_UNUSED_PARAMETER(param) (void)(param)

/* TODO: Figure out a way to do a safe cast, something like this:
#define SXE_SAFE_CAST_UNSIGNED_CHAR(value) \
    ((unsigned char)(SXEV81((unsigned long)value, <= 0xFF, "%lu cannot be safely cast to unsigned char", (unsigned long)value)))
*/

/* Fun macros for relocatable data structures
 */
#define SXE_PTR_FIX(base, type, ptr_rel) ((type)((char *)(ptr_rel) + (size_t)(base)))
#define SXE_PTR_REL(base, type, ptr_fix) ((type)((char *)(ptr_fix) - (size_t)(base)))

#define SXE_ROT13_CHAR(character) (sxe_rot13_char[(unsigned char)(character)])

extern unsigned char sxe_rot13_char[];

#include "sxe-hex-proto.h"
#include "sxe-mkpath-proto.h"
#include "sxe-rot13-proto.h"
#include "sxe-str-proto.h"

#endif
