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

#ifndef __SXE_LOG_H__
#define __SXE_LOG_H__

#ifndef SXE_DEBUG
#   define SXE_DEBUG 0
#endif

#define SXE_TIMESTAMP_SIZE 20   /* YYYYmmDDHHMMSS.hhh + '\0' + pad */

/* TODO: Design and document a consistent strategy for using the various log levels
 */

extern unsigned sxe_log_level;

/* The following macros and types are here because sxe-log.h is the lowest level package
 */

#define SXE_UNUSED_ARGUMENT(x) (void)x

#define SXE_EARLY_OR_ERROR_OUT goto SXE_EARLY_OUT; SXE_EARLY_OUT: goto SXE_ERROR_OUT; SXE_ERROR_OUT

typedef enum SXE_BOOL { SXE_FALSE = 0, SXE_TRUE = 1 } SXE_BOOL;

typedef enum SXE_RETURN {
    SXE_RETURN_OK = 0,
    SXE_RETURN_UNCATEGORIZED,
    SXE_RETURN_WARN_CACHE_DOUBLE_INITIALIZED,
    SXE_RETURN_WARN_WOULD_BLOCK,
    SXE_RETURN_ERROR_CACHE_UNINITIALIZED,
    SXE_RETURN_ERROR_ALLOC,
    SXE_RETURN_ERROR_INTERNAL,
    SXE_RETURN_ERROR_NO_CONNECTION,
    SXE_RETURN_ERROR_ALREADY_CONNECTED,
    SXE_RETURN_ERROR_INVALID_URI,
    SXE_RETURN_ERROR_ADDRESS_IN_USE,
    SXE_RETURN_WARN_ALREADY_CLOSED,
    SXE_RETURN_ERROR_INTERRUPTED,
    SXE_RETURN_ERROR_COMMAND_NOT_RUN,
    SXE_RETURN_INVALID_VALUE    /* This must be the last value. */
} SXE_RETURN;

/**
 * - Logging macros
 *   - SXE<log type><log level><log arguments>
 *     - Where:
 *       - <log type> is one of:
 *         - 'E' procedure (e)ntry
 *         - 'L' regular (l)og line
 *         - 'R' procedure (r)eturn
 *         - 'A' (a)assertion
 *         - 'V' (v)erify
 *       - <log level> is one of:
 *         - 1 critical
 *         - 2 high
 *         - 3 medium
 *         - 4 low
 *         - 5 oem; useful for OEM integration
 *         - 6-9 debug; for Sophos development as follows:
 *           - 6-7 logging within the sxe caller
 *           - 8-9 logging within the sxe core packages
 *           - 6&8 tell the "story" of what is going on
 *           - 7&9 more detailed and verbose debug information
 *       - <log arguments> is one of:
 *         - 1 to 8
 * - Notes:
 *   - Use 'make SXE_DEBUG=1' to use instrumentation macros
 *   - Log type Verify keeps the condition if 'make SXE_DEBUG=0'
 *   - We don't use 'variadic macros' since they were introduced
 *     in C99 and this is C89 :-)
 *     - http://en.wikipedia.org/wiki/Variadic_macro
 */

/**
 * - Example C source code:
 *   - void
 *   - foo1 ( int a, int b )
 *   - {
 *   -     int result;
 *   -     SXEE62("foo1(a=%d, b=%d)", a, b);
 *   -     SXEL10("foo1 example critical:1\n");
 *   -     SXEL20("foo1 example high:2\n");
 *   -     SXEL30("foo1 example medium:3\n");
 *   -     SXEL40("foo1 example low:4\n");
 *   -     SXEL50("foo1 example oem:5\n");
 *   -     SXEL60("foo1 example debug:6\n");
 *   -     SXEV61((result = foo2(b,a)),== 10,"result=%d",result);
 *   -     SXER60("return");
 *   - }
 *   -
 *   - int
 *   - foo2 ( int c, int d )
 *   - {
 *   -     SXEE62("foo2(c=%d, d=%d)", c, d);
 *   -     SXEL10("foo2 example critical:1\n");
 *   -     SXEL20("foo2 example high:2\n");
 *   -     SXEL30("foo2 example medium:3\n");
 *   -     SXEL40("foo2 example low:4\n");
 *   -     SXEL50("foo2 example oem:5\n");
 *   -     SXEL60("foo2 example debug:6\n");
 *   -     SXER60("return");
 *   -     return (c+d);
 *   - }
 * - Example output with 'make SXE_DEBUG=1':
 *   - + foo1(a=1, b=2) { // foo.c:32
 *   -   - foo1 example critical:1
 *   -   - foo1 example high:2
 *   -   - foo1 example medium:3
 *   -   - foo1 example low:4
 *   -   - foo1 example oem:5
 *   -   - foo1 example debug:6
 *   -   + foo2(c=2, d=1) { // foo.c:46
 *   -     - foo2 example critical:1
 *   -     - foo2 example high:2
 *   -     - foo2 example medium:3
 *   -     - foo2 example low:4
 *   -     - foo2 example oem:5
 *   -     - foo2 example debug:6
 *   -     } // foo.c:53
 *   - ERROR: debug assertion '(result = foo2(b,a))==10' failed
 *     at sxe.c:39; result=3
 * - Example output with 'make SXE_DEBUG=0':
 *   - - foo1 example critical:1
 *   - - foo2 example critical:1
 */

#ifndef SXE_IF_VERBOSITY_AT_LEAST
#define SXE_IF_VERBOSITY_AT_LEAST(line_level) if((line_level)<=sxe_log_level)
#endif

#define SXEL10(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(~0U,1,fmt                           );}
#define SXEL11(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(~0U,1,fmt,a1                        );}
#define SXEL12(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(~0U,1,fmt,a1,a2                     );}
#define SXEL13(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(~0U,1,fmt,a1,a2,a3                  );}
#define SXEL14(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(~0U,1,fmt,a1,a2,a3,a4               );}
#define SXEL15(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(~0U,1,fmt,a1,a2,a3,a4,a5            );}
#define SXEL16(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(~0U,1,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL17(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(~0U,1,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL18(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(~0U,1,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL19(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(~0U,1,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEL20(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(~0U,2,fmt                           );}
#define SXEL21(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(~0U,2,fmt,a1                        );}
#define SXEL22(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(~0U,2,fmt,a1,a2                     );}
#define SXEL23(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(~0U,2,fmt,a1,a2,a3                  );}
#define SXEL24(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(~0U,2,fmt,a1,a2,a3,a4               );}
#define SXEL25(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(~0U,2,fmt,a1,a2,a3,a4,a5            );}
#define SXEL26(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(~0U,2,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL27(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(~0U,2,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL28(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(~0U,2,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL29(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(~0U,2,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEL30(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(~0U,3,fmt                           );}
#define SXEL31(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(~0U,3,fmt,a1                        );}
#define SXEL32(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(~0U,3,fmt,a1,a2                     );}
#define SXEL33(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(~0U,3,fmt,a1,a2,a3                  );}
#define SXEL34(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(~0U,3,fmt,a1,a2,a3,a4               );}
#define SXEL35(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(~0U,3,fmt,a1,a2,a3,a4,a5            );}
#define SXEL36(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(~0U,3,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL37(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(~0U,3,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL38(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(~0U,3,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL39(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(~0U,3,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEL40(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(~0U,4,fmt                           );}
#define SXEL41(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(~0U,4,fmt,a1                        );}
#define SXEL42(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(~0U,4,fmt,a1,a2                     );}
#define SXEL43(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(~0U,4,fmt,a1,a2,a3                  );}
#define SXEL44(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(~0U,4,fmt,a1,a2,a3,a4               );}
#define SXEL45(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(~0U,4,fmt,a1,a2,a3,a4,a5            );}
#define SXEL46(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(~0U,4,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL47(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(~0U,4,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL48(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(~0U,4,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL49(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(~0U,4,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEL50(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(~0U,5,fmt                           );}
#define SXEL51(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(~0U,5,fmt,a1                        );}
#define SXEL52(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(~0U,5,fmt,a1,a2                     );}
#define SXEL53(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(~0U,5,fmt,a1,a2,a3                  );}
#define SXEL54(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(~0U,5,fmt,a1,a2,a3,a4               );}
#define SXEL55(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(~0U,5,fmt,a1,a2,a3,a4,a5            );}
#define SXEL56(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(~0U,5,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL57(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(~0U,5,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL58(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(~0U,5,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL59(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(~0U,5,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}

#define SXEA10(con,fmt)                                 if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt                           );}
#define SXEA11(con,fmt,a1)                              if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1                        );}
#define SXEA12(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2                     );}
#define SXEA13(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3                  );}
#define SXEA14(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4               );}
#define SXEA15(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5            );}
#define SXEA16(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEA17(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEA18(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEA19(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEA20(con,fmt)                                 if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt                           );}
#define SXEA21(con,fmt,a1)                              if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1                        );}
#define SXEA22(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2                     );}
#define SXEA23(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3                  );}
#define SXEA24(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4               );}
#define SXEA25(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5            );}
#define SXEA26(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEA27(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEA28(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEA29(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEA30(con,fmt)                                 if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt                           );}
#define SXEA31(con,fmt,a1)                              if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1                        );}
#define SXEA32(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2                     );}
#define SXEA33(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3                  );}
#define SXEA34(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4               );}
#define SXEA35(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5            );}
#define SXEA36(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEA37(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEA38(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEA39(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEA40(con,fmt)                                 if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt                           );}
#define SXEA41(con,fmt,a1)                              if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1                        );}
#define SXEA42(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2                     );}
#define SXEA43(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3                  );}
#define SXEA44(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4               );}
#define SXEA45(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5            );}
#define SXEA46(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEA47(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEA48(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEA49(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEA50(con,fmt)                                 if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt                           );}
#define SXEA51(con,fmt,a1)                              if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1                        );}
#define SXEA52(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2                     );}
#define SXEA53(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3                  );}
#define SXEA54(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4               );}
#define SXEA55(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5            );}
#define SXEA56(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEA57(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEA58(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEA59(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}

#define SXEV10(func,con,fmt)                            if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV11(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV12(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV13(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV14(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV15(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV16(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV17(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV18(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV19(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEV20(func,con,fmt)                            if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV21(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV22(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV23(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV24(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV25(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV26(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV27(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV28(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV29(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEV30(func,con,fmt)                            if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV31(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV32(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV33(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV34(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV35(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV36(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV37(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV38(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV39(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEV40(func,con,fmt)                            if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV41(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV42(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV43(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV44(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV45(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV46(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV47(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV48(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV49(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEV50(func,con,fmt)                            if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV51(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV52(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV53(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV54(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV55(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV56(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV57(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV58(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV59(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}

#define SXED10(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log_dump_memory(~0U,1,ptr,len);}
#define SXED20(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log_dump_memory(~0U,2,ptr,len);}
#define SXED30(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log_dump_memory(~0U,3,ptr,len);}
#define SXED40(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log_dump_memory(~0U,4,ptr,len);}
#define SXED50(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log_dump_memory(~0U,5,ptr,len);}

#if SXE_DEBUG
#define SXEL60(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt                           );}
#define SXEL61(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1                        );}
#define SXEL62(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1,a2                     );}
#define SXEL63(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1,a2,a3                  );}
#define SXEL64(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1,a2,a3,a4               );}
#define SXEL65(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1,a2,a3,a4,a5            );}
#define SXEL66(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL67(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL68(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL69(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEE60(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(~0U,6,__FILE__,__LINE__,fmt                           );}
#define SXEE61(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(~0U,6,__FILE__,__LINE__,fmt,a1                        );}
#define SXEE62(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(~0U,6,__FILE__,__LINE__,fmt,a1,a2                     );}
#define SXEE63(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(~0U,6,__FILE__,__LINE__,fmt,a1,a2,a3                  );}
#define SXEE64(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(~0U,6,__FILE__,__LINE__,fmt,a1,a2,a3,a4               );}
#define SXEE65(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(~0U,6,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5            );}
#define SXEE66(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(~0U,6,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEE67(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(~0U,6,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEE68(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(~0U,6,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEE69(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(~0U,6,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXER60(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt                           ); sxe_log_return(~0U,6,__FILE__,__LINE__);}
#define SXER61(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1                        ); sxe_log_return(~0U,6,__FILE__,__LINE__);}
#define SXER62(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1,a2                     ); sxe_log_return(~0U,6,__FILE__,__LINE__);}
#define SXER63(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1,a2,a3                  ); sxe_log_return(~0U,6,__FILE__,__LINE__);}
#define SXER64(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1,a2,a3,a4               ); sxe_log_return(~0U,6,__FILE__,__LINE__);}
#define SXER65(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1,a2,a3,a4,a5            ); sxe_log_return(~0U,6,__FILE__,__LINE__);}
#define SXER66(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1,a2,a3,a4,a5,a6         ); sxe_log_return(~0U,6,__FILE__,__LINE__);}
#define SXER67(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1,a2,a3,a4,a5,a6,a7      ); sxe_log_return(~0U,6,__FILE__,__LINE__);}
#define SXER68(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8   ); sxe_log_return(~0U,6,__FILE__,__LINE__);}
#define SXER69(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (~0U,6,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9); sxe_log_return(~0U,6,__FILE__,__LINE__);}
#define SXEA60(con,fmt)                                 if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt);}
#define SXEA61(con,fmt,a1)                              if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1);}
#define SXEA62(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2);}
#define SXEA63(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3);}
#define SXEA64(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4);}
#define SXEA65(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5);}
#define SXEA66(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEA67(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEA68(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEA69(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEV60(func,con,fmt)                            if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV61(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV62(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV63(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV64(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV65(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV66(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV67(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV68(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV69(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXED60(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_dump_memory(~0U,6,ptr,len);}

#define SXEL70(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt                           );}
#define SXEL71(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1                        );}
#define SXEL72(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1,a2                     );}
#define SXEL73(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1,a2,a3                  );}
#define SXEL74(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1,a2,a3,a4               );}
#define SXEL75(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1,a2,a3,a4,a5            );}
#define SXEL76(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL77(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL78(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL79(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEE70(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(~0U,7,__FILE__,__LINE__,fmt                           );}
#define SXEE71(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(~0U,7,__FILE__,__LINE__,fmt,a1                        );}
#define SXEE72(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(~0U,7,__FILE__,__LINE__,fmt,a1,a2                     );}
#define SXEE73(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(~0U,7,__FILE__,__LINE__,fmt,a1,a2,a3                  );}
#define SXEE74(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(~0U,7,__FILE__,__LINE__,fmt,a1,a2,a3,a4               );}
#define SXEE75(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(~0U,7,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5            );}
#define SXEE76(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(~0U,7,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEE77(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(~0U,7,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEE78(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(~0U,7,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEE79(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(~0U,7,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXER70(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt                           ); sxe_log_return(~0U,7,__FILE__,__LINE__);}
#define SXER71(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1                        ); sxe_log_return(~0U,7,__FILE__,__LINE__);}
#define SXER72(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1,a2                     ); sxe_log_return(~0U,7,__FILE__,__LINE__);}
#define SXER73(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1,a2,a3                  ); sxe_log_return(~0U,7,__FILE__,__LINE__);}
#define SXER74(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1,a2,a3,a4               ); sxe_log_return(~0U,7,__FILE__,__LINE__);}
#define SXER75(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1,a2,a3,a4,a5            ); sxe_log_return(~0U,7,__FILE__,__LINE__);}
#define SXER76(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1,a2,a3,a4,a5,a6         ); sxe_log_return(~0U,7,__FILE__,__LINE__);}
#define SXER77(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1,a2,a3,a4,a5,a6,a7      ); sxe_log_return(~0U,7,__FILE__,__LINE__);}
#define SXER78(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8   ); sxe_log_return(~0U,7,__FILE__,__LINE__);}
#define SXER79(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (~0U,7,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9); sxe_log_return(~0U,7,__FILE__,__LINE__);}
#define SXEA70(con,fmt)                                 if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt);}
#define SXEA71(con,fmt,a1)                              if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1);}
#define SXEA72(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2);}
#define SXEA73(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3);}
#define SXEA74(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4);}
#define SXEA75(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5);}
#define SXEA76(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEA77(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEA78(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEA79(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEV70(func,con,fmt)                            if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV71(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV72(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV73(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV74(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV75(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV76(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV77(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV78(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV79(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXED70(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_dump_memory(~0U,7,ptr,len);}

#define SXEL80(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt                           );}
#define SXEL81(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1                        );}
#define SXEL82(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1,a2                     );}
#define SXEL83(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1,a2,a3                  );}
#define SXEL84(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1,a2,a3,a4               );}
#define SXEL85(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1,a2,a3,a4,a5            );}
#define SXEL86(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL87(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL88(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL89(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEE80(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(~0U,8,__FILE__,__LINE__,fmt                           );}
#define SXEE81(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(~0U,8,__FILE__,__LINE__,fmt,a1                        );}
#define SXEE82(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(~0U,8,__FILE__,__LINE__,fmt,a1,a2                     );}
#define SXEE83(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(~0U,8,__FILE__,__LINE__,fmt,a1,a2,a3                  );}
#define SXEE84(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(~0U,8,__FILE__,__LINE__,fmt,a1,a2,a3,a4               );}
#define SXEE85(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(~0U,8,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5            );}
#define SXEE86(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(~0U,8,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEE87(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(~0U,8,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEE88(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(~0U,8,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEE89(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(~0U,8,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXER80(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt                           ); sxe_log_return(~0U,8,__FILE__,__LINE__);}
#define SXER81(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1                        ); sxe_log_return(~0U,8,__FILE__,__LINE__);}
#define SXER82(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1,a2                     ); sxe_log_return(~0U,8,__FILE__,__LINE__);}
#define SXER83(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1,a2,a3                  ); sxe_log_return(~0U,8,__FILE__,__LINE__);}
#define SXER84(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1,a2,a3,a4               ); sxe_log_return(~0U,8,__FILE__,__LINE__);}
#define SXER85(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1,a2,a3,a4,a5            ); sxe_log_return(~0U,8,__FILE__,__LINE__);}
#define SXER86(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1,a2,a3,a4,a5,a6         ); sxe_log_return(~0U,8,__FILE__,__LINE__);}
#define SXER87(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1,a2,a3,a4,a5,a6,a7      ); sxe_log_return(~0U,8,__FILE__,__LINE__);}
#define SXER88(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8   ); sxe_log_return(~0U,8,__FILE__,__LINE__);}
#define SXER89(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (~0U,8,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9); sxe_log_return(~0U,8,__FILE__,__LINE__);}
#define SXEA80(con,fmt)                                 if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt);}
#define SXEA81(con,fmt,a1)                              if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1);}
#define SXEA82(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2);}
#define SXEA83(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3);}
#define SXEA84(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4);}
#define SXEA85(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5);}
#define SXEA86(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEA87(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEA88(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEA89(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEV80(func,con,fmt)                            if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV81(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV82(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV83(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV84(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV85(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV86(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV87(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV88(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV89(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXED80(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_dump_memory(~0U,8,ptr,len);}

#define SXEL90(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt                           );}
#define SXEL91(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1                        );}
#define SXEL92(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1,a2                     );}
#define SXEL93(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1,a2,a3                  );}
#define SXEL94(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1,a2,a3,a4               );}
#define SXEL95(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1,a2,a3,a4,a5            );}
#define SXEL96(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL97(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL98(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL99(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEE90(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(~0U,9,__FILE__,__LINE__,fmt                           );}
#define SXEE91(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(~0U,9,__FILE__,__LINE__,fmt,a1                        );}
#define SXEE92(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(~0U,9,__FILE__,__LINE__,fmt,a1,a2                     );}
#define SXEE93(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(~0U,9,__FILE__,__LINE__,fmt,a1,a2,a3                  );}
#define SXEE94(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(~0U,9,__FILE__,__LINE__,fmt,a1,a2,a3,a4               );}
#define SXEE95(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(~0U,9,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5            );}
#define SXEE96(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(~0U,9,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEE97(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(~0U,9,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEE98(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(~0U,9,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEE99(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(~0U,9,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXER90(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt                           ); sxe_log_return(~0U,9,__FILE__,__LINE__);}
#define SXER91(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1                        ); sxe_log_return(~0U,9,__FILE__,__LINE__);}
#define SXER92(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1,a2                     ); sxe_log_return(~0U,9,__FILE__,__LINE__);}
#define SXER93(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1,a2,a3                  ); sxe_log_return(~0U,9,__FILE__,__LINE__);}
#define SXER94(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1,a2,a3,a4               ); sxe_log_return(~0U,9,__FILE__,__LINE__);}
#define SXER95(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1,a2,a3,a4,a5            ); sxe_log_return(~0U,9,__FILE__,__LINE__);}
#define SXER96(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1,a2,a3,a4,a5,a6         ); sxe_log_return(~0U,9,__FILE__,__LINE__);}
#define SXER97(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1,a2,a3,a4,a5,a6,a7      ); sxe_log_return(~0U,9,__FILE__,__LINE__);}
#define SXER98(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8   ); sxe_log_return(~0U,9,__FILE__,__LINE__);}
#define SXER99(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (~0U,9,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9); sxe_log_return(~0U,9,__FILE__,__LINE__);}
#define SXEA90(con,fmt)                                 if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt);}
#define SXEA91(con,fmt,a1)                              if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1);}
#define SXEA92(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2);}
#define SXEA93(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3);}
#define SXEA94(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4);}
#define SXEA95(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5);}
#define SXEA96(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEA97(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEA98(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEA99(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEV90(func,con,fmt)                            if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV91(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV92(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV93(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV94(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV95(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV96(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV97(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV98(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV99(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(~0U,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXED90(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_dump_memory(~0U,9,ptr,len);}
#else
#define SXEL60(fmt)
#define SXEL61(fmt,a1)
#define SXEL62(fmt,a1,a2)
#define SXEL63(fmt,a1,a2,a3)
#define SXEL64(fmt,a1,a2,a3,a4)
#define SXEL65(fmt,a1,a2,a3,a4,a5)
#define SXEL66(fmt,a1,a2,a3,a4,a5,a6)
#define SXEL67(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEL68(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEL69(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEE60(fmt)
#define SXEE61(fmt,a1)
#define SXEE62(fmt,a1,a2)
#define SXEE63(fmt,a1,a2,a3)
#define SXEE64(fmt,a1,a2,a3,a4)
#define SXEE65(fmt,a1,a2,a3,a4,a5)
#define SXEE66(fmt,a1,a2,a3,a4,a5,a6)
#define SXEE67(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEE68(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEE69(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXER60(fmt)
#define SXER61(fmt,a1)
#define SXER62(fmt,a1,a2)
#define SXER63(fmt,a1,a2,a3)
#define SXER64(fmt,a1,a2,a3,a4)
#define SXER65(fmt,a1,a2,a3,a4,a5)
#define SXER66(fmt,a1,a2,a3,a4,a5,a6)
#define SXER67(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXER68(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXER69(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEA60(con,fmt)
#define SXEA61(con,fmt,a1)
#define SXEA62(con,fmt,a1,a2)
#define SXEA63(con,fmt,a1,a2,a3)
#define SXEA64(con,fmt,a1,a2,a3,a4)
#define SXEA65(con,fmt,a1,a2,a3,a4,a5)
#define SXEA66(con,fmt,a1,a2,a3,a4,a5,a6)
#define SXEA67(con,fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEA68(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEA69(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEV60(func,con,fmt)                            ((void)(func))
#define SXEV61(func,con,fmt,a1)                         ((void)(func))
#define SXEV62(func,con,fmt,a1,a2)                      ((void)(func))
#define SXEV63(func,con,fmt,a1,a2,a3)                   ((void)(func))
#define SXEV64(func,con,fmt,a1,a2,a3,a4)                ((void)(func))
#define SXEV65(func,con,fmt,a1,a2,a3,a4,a5)             ((void)(func))
#define SXEV66(func,con,fmt,a1,a2,a3,a4,a5,a6)          ((void)(func))
#define SXEV67(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       ((void)(func))
#define SXEV68(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    ((void)(func))
#define SXEV69(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) ((void)(func))
#define SXED60(ptr,len)

#define SXEL70(fmt)
#define SXEL71(fmt,a1)
#define SXEL72(fmt,a1,a2)
#define SXEL73(fmt,a1,a2,a3)
#define SXEL74(fmt,a1,a2,a3,a4)
#define SXEL75(fmt,a1,a2,a3,a4,a5)
#define SXEL76(fmt,a1,a2,a3,a4,a5,a6)
#define SXEL77(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEL78(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEL79(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEE70(fmt)
#define SXEE71(fmt,a1)
#define SXEE72(fmt,a1,a2)
#define SXEE73(fmt,a1,a2,a3)
#define SXEE74(fmt,a1,a2,a3,a4)
#define SXEE75(fmt,a1,a2,a3,a4,a5)
#define SXEE76(fmt,a1,a2,a3,a4,a5,a6)
#define SXEE77(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEE78(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEE79(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXER70(fmt)
#define SXER71(fmt,a1)
#define SXER72(fmt,a1,a2)
#define SXER73(fmt,a1,a2,a3)
#define SXER74(fmt,a1,a2,a3,a4)
#define SXER75(fmt,a1,a2,a3,a4,a5)
#define SXER76(fmt,a1,a2,a3,a4,a5,a6)
#define SXER77(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXER78(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXER79(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEA70(con,fmt)
#define SXEA71(con,fmt,a1)
#define SXEA72(con,fmt,a1,a2)
#define SXEA73(con,fmt,a1,a2,a3)
#define SXEA74(con,fmt,a1,a2,a3,a4)
#define SXEA75(con,fmt,a1,a2,a3,a4,a5)
#define SXEA76(con,fmt,a1,a2,a3,a4,a5,a6)
#define SXEA77(con,fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEA78(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEA79(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEV70(func,con,fmt)                            ((void)(func))
#define SXEV71(func,con,fmt,a1)                         ((void)(func))
#define SXEV72(func,con,fmt,a1,a2)                      ((void)(func))
#define SXEV73(func,con,fmt,a1,a2,a3)                   ((void)(func))
#define SXEV74(func,con,fmt,a1,a2,a3,a4)                ((void)(func))
#define SXEV75(func,con,fmt,a1,a2,a3,a4,a5)             ((void)(func))
#define SXEV76(func,con,fmt,a1,a2,a3,a4,a5,a6)          ((void)(func))
#define SXEV77(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       ((void)(func))
#define SXEV78(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    ((void)(func))
#define SXEV79(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) ((void)(func))
#define SXED70(ptr,len)

#define SXEL80(fmt)
#define SXEL81(fmt,a1)
#define SXEL82(fmt,a1,a2)
#define SXEL83(fmt,a1,a2,a3)
#define SXEL84(fmt,a1,a2,a3,a4)
#define SXEL85(fmt,a1,a2,a3,a4,a5)
#define SXEL86(fmt,a1,a2,a3,a4,a5,a6)
#define SXEL87(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEL88(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEL89(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEE80(fmt)
#define SXEE81(fmt,a1)
#define SXEE82(fmt,a1,a2)
#define SXEE83(fmt,a1,a2,a3)
#define SXEE84(fmt,a1,a2,a3,a4)
#define SXEE85(fmt,a1,a2,a3,a4,a5)
#define SXEE86(fmt,a1,a2,a3,a4,a5,a6)
#define SXEE87(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEE88(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEE89(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXER80(fmt)
#define SXER81(fmt,a1)
#define SXER82(fmt,a1,a2)
#define SXER83(fmt,a1,a2,a3)
#define SXER84(fmt,a1,a2,a3,a4)
#define SXER85(fmt,a1,a2,a3,a4,a5)
#define SXER86(fmt,a1,a2,a3,a4,a5,a6)
#define SXER87(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXER88(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXER89(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEA80(con,fmt)
#define SXEA81(con,fmt,a1)
#define SXEA82(con,fmt,a1,a2)
#define SXEA83(con,fmt,a1,a2,a3)
#define SXEA84(con,fmt,a1,a2,a3,a4)
#define SXEA85(con,fmt,a1,a2,a3,a4,a5)
#define SXEA86(con,fmt,a1,a2,a3,a4,a5,a6)
#define SXEA87(con,fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEA88(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEA89(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEV80(func,con,fmt)                             ((void)(func))
#define SXEV81(func,con,fmt,a1)                          ((void)(func))
#define SXEV82(func,con,fmt,a1,a2)                       ((void)(func))
#define SXEV83(func,con,fmt,a1,a2,a3)                    ((void)(func))
#define SXEV84(func,con,fmt,a1,a2,a3,a4)                 ((void)(func))
#define SXEV85(func,con,fmt,a1,a2,a3,a4,a5)              ((void)(func))
#define SXEV86(func,con,fmt,a1,a2,a3,a4,a5,a6)           ((void)(func))
#define SXEV87(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)        ((void)(func))
#define SXEV88(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)     ((void)(func))
#define SXEV89(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)  ((void)(func))
#define SXED80(ptr,len)

#define SXEL90(fmt)
#define SXEL91(fmt,a1)
#define SXEL92(fmt,a1,a2)
#define SXEL93(fmt,a1,a2,a3)
#define SXEL94(fmt,a1,a2,a3,a4)
#define SXEL95(fmt,a1,a2,a3,a4,a5)
#define SXEL96(fmt,a1,a2,a3,a4,a5,a6)
#define SXEL97(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEL98(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEL99(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEE90(fmt)
#define SXEE91(fmt,a1)
#define SXEE92(fmt,a1,a2)
#define SXEE93(fmt,a1,a2,a3)
#define SXEE94(fmt,a1,a2,a3,a4)
#define SXEE95(fmt,a1,a2,a3,a4,a5)
#define SXEE96(fmt,a1,a2,a3,a4,a5,a6)
#define SXEE97(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEE98(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEE99(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXER90(fmt)
#define SXER91(fmt,a1)
#define SXER92(fmt,a1,a2)
#define SXER93(fmt,a1,a2,a3)
#define SXER94(fmt,a1,a2,a3,a4)
#define SXER95(fmt,a1,a2,a3,a4,a5)
#define SXER96(fmt,a1,a2,a3,a4,a5,a6)
#define SXER97(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXER98(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXER99(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEA90(con,fmt)
#define SXEA91(con,fmt,a1)
#define SXEA92(con,fmt,a1,a2)
#define SXEA93(con,fmt,a1,a2,a3)
#define SXEA94(con,fmt,a1,a2,a3,a4)
#define SXEA95(con,fmt,a1,a2,a3,a4,a5)
#define SXEA96(con,fmt,a1,a2,a3,a4,a5,a6)
#define SXEA97(con,fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEA98(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEA99(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEV90(func,con,fmt)                             ((void)(func))
#define SXEV91(func,con,fmt,a1)                          ((void)(func))
#define SXEV92(func,con,fmt,a1,a2)                       ((void)(func))
#define SXEV93(func,con,fmt,a1,a2,a3)                    ((void)(func))
#define SXEV94(func,con,fmt,a1,a2,a3,a4)                 ((void)(func))
#define SXEV95(func,con,fmt,a1,a2,a3,a4,a5)              ((void)(func))
#define SXEV96(func,con,fmt,a1,a2,a3,a4,a5,a6)           ((void)(func))
#define SXEV97(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)        ((void)(func))
#define SXEV98(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)     ((void)(func))
#define SXEV99(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)  ((void)(func))
#define SXED90(ptr,len)
#endif

#define SXE_ID4LOG ((this) != NULL ? (this)->id : ~0U)

#define SXEL10I(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(SXE_ID4LOG,1,fmt                           );}
#define SXEL11I(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(SXE_ID4LOG,1,fmt,a1                        );}
#define SXEL12I(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(SXE_ID4LOG,1,fmt,a1,a2                     );}
#define SXEL13I(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(SXE_ID4LOG,1,fmt,a1,a2,a3                  );}
#define SXEL14I(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(SXE_ID4LOG,1,fmt,a1,a2,a3,a4               );}
#define SXEL15I(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(SXE_ID4LOG,1,fmt,a1,a2,a3,a4,a5            );}
#define SXEL16I(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(SXE_ID4LOG,1,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL17I(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(SXE_ID4LOG,1,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL18I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(SXE_ID4LOG,1,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL19I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log(SXE_ID4LOG,1,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEL20I(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(SXE_ID4LOG,2,fmt                           );}
#define SXEL21I(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(SXE_ID4LOG,2,fmt,a1                        );}
#define SXEL22I(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(SXE_ID4LOG,2,fmt,a1,a2                     );}
#define SXEL23I(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(SXE_ID4LOG,2,fmt,a1,a2,a3                  );}
#define SXEL24I(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(SXE_ID4LOG,2,fmt,a1,a2,a3,a4               );}
#define SXEL25I(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(SXE_ID4LOG,2,fmt,a1,a2,a3,a4,a5            );}
#define SXEL26I(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(SXE_ID4LOG,2,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL27I(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(SXE_ID4LOG,2,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL28I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(SXE_ID4LOG,2,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL29I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log(SXE_ID4LOG,2,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEL30I(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(SXE_ID4LOG,3,fmt                           );}
#define SXEL31I(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(SXE_ID4LOG,3,fmt,a1                        );}
#define SXEL32I(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(SXE_ID4LOG,3,fmt,a1,a2                     );}
#define SXEL33I(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(SXE_ID4LOG,3,fmt,a1,a2,a3                  );}
#define SXEL34I(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(SXE_ID4LOG,3,fmt,a1,a2,a3,a4               );}
#define SXEL35I(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(SXE_ID4LOG,3,fmt,a1,a2,a3,a4,a5            );}
#define SXEL36I(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(SXE_ID4LOG,3,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL37I(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(SXE_ID4LOG,3,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL38I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(SXE_ID4LOG,3,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL39I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log(SXE_ID4LOG,3,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEL40I(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(SXE_ID4LOG,4,fmt                           );}
#define SXEL41I(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(SXE_ID4LOG,4,fmt,a1                        );}
#define SXEL42I(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(SXE_ID4LOG,4,fmt,a1,a2                     );}
#define SXEL43I(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(SXE_ID4LOG,4,fmt,a1,a2,a3                  );}
#define SXEL44I(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(SXE_ID4LOG,4,fmt,a1,a2,a3,a4               );}
#define SXEL45I(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(SXE_ID4LOG,4,fmt,a1,a2,a3,a4,a5            );}
#define SXEL46I(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(SXE_ID4LOG,4,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL47I(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(SXE_ID4LOG,4,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL48I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(SXE_ID4LOG,4,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL49I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log(SXE_ID4LOG,4,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEL50I(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(SXE_ID4LOG,5,fmt                           );}
#define SXEL51I(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(SXE_ID4LOG,5,fmt,a1                        );}
#define SXEL52I(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(SXE_ID4LOG,5,fmt,a1,a2                     );}
#define SXEL53I(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(SXE_ID4LOG,5,fmt,a1,a2,a3                  );}
#define SXEL54I(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(SXE_ID4LOG,5,fmt,a1,a2,a3,a4               );}
#define SXEL55I(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(SXE_ID4LOG,5,fmt,a1,a2,a3,a4,a5            );}
#define SXEL56I(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(SXE_ID4LOG,5,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL57I(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(SXE_ID4LOG,5,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL58I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(SXE_ID4LOG,5,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL59I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log(SXE_ID4LOG,5,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}

#define SXEA10I(con,fmt)                                 if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt);}
#define SXEA11I(con,fmt,a1)                              if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1);}
#define SXEA12I(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2);}
#define SXEA13I(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3);}
#define SXEA14I(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4);}
#define SXEA15I(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5);}
#define SXEA16I(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEA17I(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEA18I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEA19I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEA20I(con,fmt)                                 if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt);}
#define SXEA21I(con,fmt,a1)                              if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1);}
#define SXEA22I(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2);}
#define SXEA23I(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3);}
#define SXEA24I(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4);}
#define SXEA25I(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5);}
#define SXEA26I(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEA27I(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEA28I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEA29I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEA30I(con,fmt)                                 if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt);}
#define SXEA31I(con,fmt,a1)                              if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1);}
#define SXEA32I(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2);}
#define SXEA33I(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3);}
#define SXEA34I(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4);}
#define SXEA35I(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5);}
#define SXEA36I(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEA37I(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEA38I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEA39I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEA40I(con,fmt)                                 if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt);}
#define SXEA41I(con,fmt,a1)                              if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1);}
#define SXEA42I(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2);}
#define SXEA43I(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3);}
#define SXEA44I(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4);}
#define SXEA45I(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5);}
#define SXEA46I(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEA47I(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEA48I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEA49I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEA50I(con,fmt)                                 if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt);}
#define SXEA51I(con,fmt,a1)                              if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1);}
#define SXEA52I(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2);}
#define SXEA53I(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3);}
#define SXEA54I(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4);}
#define SXEA55I(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5);}
#define SXEA56I(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEA57I(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEA58I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEA59I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}

#define SXEV10I(func,con,fmt)                            if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV11I(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV12I(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV13I(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV14I(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV15I(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV16I(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV17I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV18I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV19I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEV20I(func,con,fmt)                            if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV21I(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV22I(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV23I(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV24I(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV25I(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV26I(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV27I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV28I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV29I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEV30I(func,con,fmt)                            if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV31I(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV32I(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV33I(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV34I(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV35I(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV36I(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV37I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV38I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV39I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEV40I(func,con,fmt)                            if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV41I(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV42I(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV43I(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV44I(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV45I(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV46I(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV47I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV48I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV49I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEV50I(func,con,fmt)                            if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV51I(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV52I(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV53I(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV54I(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV55I(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV56I(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV57I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV58I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV59I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}

#define SXED10I(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(1) {sxe_log_dump_memory(SXE_ID4LOG,1,ptr,len);}
#define SXED20I(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(2) {sxe_log_dump_memory(SXE_ID4LOG,2,ptr,len);}
#define SXED30I(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(3) {sxe_log_dump_memory(SXE_ID4LOG,3,ptr,len);}
#define SXED40I(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(4) {sxe_log_dump_memory(SXE_ID4LOG,4,ptr,len);}
#define SXED50I(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(5) {sxe_log_dump_memory(SXE_ID4LOG,5,ptr,len);}

#if SXE_DEBUG
#define SXEL60I(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt                           );}
#define SXEL61I(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1                        );}
#define SXEL62I(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1,a2                     );}
#define SXEL63I(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1,a2,a3                  );}
#define SXEL64I(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1,a2,a3,a4               );}
#define SXEL65I(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1,a2,a3,a4,a5            );}
#define SXEL66I(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL67I(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL68I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL69I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEE60I(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(SXE_ID4LOG,6,__FILE__,__LINE__,fmt                           );}
#define SXEE61I(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(SXE_ID4LOG,6,__FILE__,__LINE__,fmt,a1                        );}
#define SXEE62I(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(SXE_ID4LOG,6,__FILE__,__LINE__,fmt,a1,a2                     );}
#define SXEE63I(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(SXE_ID4LOG,6,__FILE__,__LINE__,fmt,a1,a2,a3                  );}
#define SXEE64I(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(SXE_ID4LOG,6,__FILE__,__LINE__,fmt,a1,a2,a3,a4               );}
#define SXEE65I(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(SXE_ID4LOG,6,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5            );}
#define SXEE66I(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(SXE_ID4LOG,6,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEE67I(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(SXE_ID4LOG,6,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEE68I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(SXE_ID4LOG,6,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEE69I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_entry(SXE_ID4LOG,6,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXER60I(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt                           ); sxe_log_return(SXE_ID4LOG,6,__FILE__,__LINE__);}
#define SXER61I(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1                        ); sxe_log_return(SXE_ID4LOG,6,__FILE__,__LINE__);}
#define SXER62I(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1,a2                     ); sxe_log_return(SXE_ID4LOG,6,__FILE__,__LINE__);}
#define SXER63I(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1,a2,a3                  ); sxe_log_return(SXE_ID4LOG,6,__FILE__,__LINE__);}
#define SXER64I(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1,a2,a3,a4               ); sxe_log_return(SXE_ID4LOG,6,__FILE__,__LINE__);}
#define SXER65I(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1,a2,a3,a4,a5            ); sxe_log_return(SXE_ID4LOG,6,__FILE__,__LINE__);}
#define SXER66I(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1,a2,a3,a4,a5,a6         ); sxe_log_return(SXE_ID4LOG,6,__FILE__,__LINE__);}
#define SXER67I(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1,a2,a3,a4,a5,a6,a7      ); sxe_log_return(SXE_ID4LOG,6,__FILE__,__LINE__);}
#define SXER68I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8   ); sxe_log_return(SXE_ID4LOG,6,__FILE__,__LINE__);}
#define SXER69I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log      (SXE_ID4LOG,6,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9); sxe_log_return(SXE_ID4LOG,6,__FILE__,__LINE__);}
#define SXEA60I(con,fmt)                                 if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt);}
#define SXEA61I(con,fmt,a1)                              if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1);}
#define SXEA62I(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2);}
#define SXEA63I(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3);}
#define SXEA64I(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4);}
#define SXEA65I(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5);}
#define SXEA66I(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEA67I(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEA68I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEA69I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEV60I(func,con,fmt)                            if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV61I(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV62I(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV63I(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV64I(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV65I(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV66I(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV67I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV68I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV69I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXED60I(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(6) {sxe_log_dump_memory(SXE_ID4LOG,6,ptr,len);}

#define SXEL70I(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt                           );}
#define SXEL71I(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1                        );}
#define SXEL72I(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1,a2                     );}
#define SXEL73I(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1,a2,a3                  );}
#define SXEL74I(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1,a2,a3,a4               );}
#define SXEL75I(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1,a2,a3,a4,a5            );}
#define SXEL76I(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL77I(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL78I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL79I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEE70I(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(SXE_ID4LOG,7,__FILE__,__LINE__,fmt                           );}
#define SXEE71I(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(SXE_ID4LOG,7,__FILE__,__LINE__,fmt,a1                        );}
#define SXEE72I(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(SXE_ID4LOG,7,__FILE__,__LINE__,fmt,a1,a2                     );}
#define SXEE73I(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(SXE_ID4LOG,7,__FILE__,__LINE__,fmt,a1,a2,a3                  );}
#define SXEE74I(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(SXE_ID4LOG,7,__FILE__,__LINE__,fmt,a1,a2,a3,a4               );}
#define SXEE75I(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(SXE_ID4LOG,7,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5            );}
#define SXEE76I(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(SXE_ID4LOG,7,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEE77I(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(SXE_ID4LOG,7,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEE78I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(SXE_ID4LOG,7,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEE79I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_entry(SXE_ID4LOG,7,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXER70I(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt                           ); sxe_log_return(SXE_ID4LOG,7,__FILE__,__LINE__);}
#define SXER71I(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1                        ); sxe_log_return(SXE_ID4LOG,7,__FILE__,__LINE__);}
#define SXER72I(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1,a2                     ); sxe_log_return(SXE_ID4LOG,7,__FILE__,__LINE__);}
#define SXER73I(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1,a2,a3                  ); sxe_log_return(SXE_ID4LOG,7,__FILE__,__LINE__);}
#define SXER74I(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1,a2,a3,a4               ); sxe_log_return(SXE_ID4LOG,7,__FILE__,__LINE__);}
#define SXER75I(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1,a2,a3,a4,a5            ); sxe_log_return(SXE_ID4LOG,7,__FILE__,__LINE__);}
#define SXER76I(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1,a2,a3,a4,a5,a6         ); sxe_log_return(SXE_ID4LOG,7,__FILE__,__LINE__);}
#define SXER77I(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1,a2,a3,a4,a5,a6,a7      ); sxe_log_return(SXE_ID4LOG,7,__FILE__,__LINE__);}
#define SXER78I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8   ); sxe_log_return(SXE_ID4LOG,7,__FILE__,__LINE__);}
#define SXER79I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log      (SXE_ID4LOG,7,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9); sxe_log_return(SXE_ID4LOG,7,__FILE__,__LINE__);}
#define SXEA70I(con,fmt)                                 if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt);}
#define SXEA71I(con,fmt,a1)                              if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1);}
#define SXEA72I(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2);}
#define SXEA73I(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3);}
#define SXEA74I(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4);}
#define SXEA75I(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5);}
#define SXEA76I(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEA77I(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEA78I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEA79I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEV70I(func,con,fmt)                            if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV71I(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV72I(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV73I(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV74I(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV75I(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV76I(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV77I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV78I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV79I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXED70I(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(7) {sxe_log_dump_memory(SXE_ID4LOG,7,ptr,len);}

#define SXEL80I(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt                           );}
#define SXEL81I(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1                        );}
#define SXEL82I(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1,a2                     );}
#define SXEL83I(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1,a2,a3                  );}
#define SXEL84I(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1,a2,a3,a4               );}
#define SXEL85I(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1,a2,a3,a4,a5            );}
#define SXEL86I(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL87I(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL88I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL89I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEE80I(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(SXE_ID4LOG,8,__FILE__,__LINE__,fmt                           );}
#define SXEE81I(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(SXE_ID4LOG,8,__FILE__,__LINE__,fmt,a1                        );}
#define SXEE82I(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(SXE_ID4LOG,8,__FILE__,__LINE__,fmt,a1,a2                     );}
#define SXEE83I(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(SXE_ID4LOG,8,__FILE__,__LINE__,fmt,a1,a2,a3                  );}
#define SXEE84I(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(SXE_ID4LOG,8,__FILE__,__LINE__,fmt,a1,a2,a3,a4               );}
#define SXEE85I(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(SXE_ID4LOG,8,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5            );}
#define SXEE86I(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(SXE_ID4LOG,8,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEE87I(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(SXE_ID4LOG,8,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEE88I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(SXE_ID4LOG,8,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEE89I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_entry(SXE_ID4LOG,8,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXER80I(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt                           ); sxe_log_return(SXE_ID4LOG,8,__FILE__,__LINE__);}
#define SXER81I(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1                        ); sxe_log_return(SXE_ID4LOG,8,__FILE__,__LINE__);}
#define SXER82I(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1,a2                     ); sxe_log_return(SXE_ID4LOG,8,__FILE__,__LINE__);}
#define SXER83I(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1,a2,a3                  ); sxe_log_return(SXE_ID4LOG,8,__FILE__,__LINE__);}
#define SXER84I(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1,a2,a3,a4               ); sxe_log_return(SXE_ID4LOG,8,__FILE__,__LINE__);}
#define SXER85I(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1,a2,a3,a4,a5            ); sxe_log_return(SXE_ID4LOG,8,__FILE__,__LINE__);}
#define SXER86I(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1,a2,a3,a4,a5,a6         ); sxe_log_return(SXE_ID4LOG,8,__FILE__,__LINE__);}
#define SXER87I(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1,a2,a3,a4,a5,a6,a7      ); sxe_log_return(SXE_ID4LOG,8,__FILE__,__LINE__);}
#define SXER88I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8   ); sxe_log_return(SXE_ID4LOG,8,__FILE__,__LINE__);}
#define SXER89I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log      (SXE_ID4LOG,8,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9); sxe_log_return(SXE_ID4LOG,8,__FILE__,__LINE__);}
#define SXEA80I(con,fmt)                                 if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt);}
#define SXEA81I(con,fmt,a1)                              if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1);}
#define SXEA82I(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2);}
#define SXEA83I(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3);}
#define SXEA84I(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4);}
#define SXEA85I(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5);}
#define SXEA86I(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEA87I(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEA88I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEA89I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEV80I(func,con,fmt)                            if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV81I(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV82I(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV83I(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV84I(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV85I(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV86I(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV87I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV88I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV89I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXED80I(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(8) {sxe_log_dump_memory(SXE_ID4LOG,8,ptr,len);}

#define SXEL90I(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt                           );}
#define SXEL91I(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1                        );}
#define SXEL92I(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1,a2                     );}
#define SXEL93I(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1,a2,a3                  );}
#define SXEL94I(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1,a2,a3,a4               );}
#define SXEL95I(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1,a2,a3,a4,a5            );}
#define SXEL96I(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEL97I(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEL98I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEL99I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEE90I(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(SXE_ID4LOG,9,__FILE__,__LINE__,fmt                           );}
#define SXEE91I(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(SXE_ID4LOG,9,__FILE__,__LINE__,fmt,a1                        );}
#define SXEE92I(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(SXE_ID4LOG,9,__FILE__,__LINE__,fmt,a1,a2                     );}
#define SXEE93I(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(SXE_ID4LOG,9,__FILE__,__LINE__,fmt,a1,a2,a3                  );}
#define SXEE94I(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(SXE_ID4LOG,9,__FILE__,__LINE__,fmt,a1,a2,a3,a4               );}
#define SXEE95I(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(SXE_ID4LOG,9,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5            );}
#define SXEE96I(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(SXE_ID4LOG,9,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6         );}
#define SXEE97I(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(SXE_ID4LOG,9,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7      );}
#define SXEE98I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(SXE_ID4LOG,9,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7,a8   );}
#define SXEE99I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_entry(SXE_ID4LOG,9,__FILE__,__LINE__,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXER90I(fmt)                                     SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt                           ); sxe_log_return(SXE_ID4LOG,9,__FILE__,__LINE__);}
#define SXER91I(fmt,a1)                                  SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1                        ); sxe_log_return(SXE_ID4LOG,9,__FILE__,__LINE__);}
#define SXER92I(fmt,a1,a2)                               SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1,a2                     ); sxe_log_return(SXE_ID4LOG,9,__FILE__,__LINE__);}
#define SXER93I(fmt,a1,a2,a3)                            SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1,a2,a3                  ); sxe_log_return(SXE_ID4LOG,9,__FILE__,__LINE__);}
#define SXER94I(fmt,a1,a2,a3,a4)                         SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1,a2,a3,a4               ); sxe_log_return(SXE_ID4LOG,9,__FILE__,__LINE__);}
#define SXER95I(fmt,a1,a2,a3,a4,a5)                      SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1,a2,a3,a4,a5            ); sxe_log_return(SXE_ID4LOG,9,__FILE__,__LINE__);}
#define SXER96I(fmt,a1,a2,a3,a4,a5,a6)                   SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1,a2,a3,a4,a5,a6         ); sxe_log_return(SXE_ID4LOG,9,__FILE__,__LINE__);}
#define SXER97I(fmt,a1,a2,a3,a4,a5,a6,a7)                SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1,a2,a3,a4,a5,a6,a7      ); sxe_log_return(SXE_ID4LOG,9,__FILE__,__LINE__);}
#define SXER98I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)             SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8   ); sxe_log_return(SXE_ID4LOG,9,__FILE__,__LINE__);}
#define SXER99I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)          SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log      (SXE_ID4LOG,9,                  fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9); sxe_log_return(SXE_ID4LOG,9,__FILE__,__LINE__);}
#define SXEA90I(con,fmt)                                 if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt);}
#define SXEA91I(con,fmt,a1)                              if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1);}
#define SXEA92I(con,fmt,a1,a2)                           if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2);}
#define SXEA93I(con,fmt,a1,a2,a3)                        if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3);}
#define SXEA94I(con,fmt,a1,a2,a3,a4)                     if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4);}
#define SXEA95I(con,fmt,a1,a2,a3,a4,a5)                  if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5);}
#define SXEA96I(con,fmt,a1,a2,a3,a4,a5,a6)               if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEA97I(con,fmt,a1,a2,a3,a4,a5,a6,a7)            if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEA98I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)         if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEA99I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)      if((     con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXEV90I(func,con,fmt)                            if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt);}
#define SXEV91I(func,con,fmt,a1)                         if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1);}
#define SXEV92I(func,con,fmt,a1,a2)                      if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2);}
#define SXEV93I(func,con,fmt,a1,a2,a3)                   if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3);}
#define SXEV94I(func,con,fmt,a1,a2,a3,a4)                if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4);}
#define SXEV95I(func,con,fmt,a1,a2,a3,a4,a5)             if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5);}
#define SXEV96I(func,con,fmt,a1,a2,a3,a4,a5,a6)          if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6);}
#define SXEV97I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7);}
#define SXEV98I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8);}
#define SXEV99I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((func con)==0){sxe_log_assert(SXE_ID4LOG,__FILE__,__LINE__,#func " " #con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);}
#define SXED90I(ptr,len)                                 SXE_IF_VERBOSITY_AT_LEAST(9) {sxe_log_dump_memory(SXE_ID4LOG,9,ptr,len);}
#else
#define SXEL60I(fmt)
#define SXEL61I(fmt,a1)
#define SXEL62I(fmt,a1,a2)
#define SXEL63I(fmt,a1,a2,a3)
#define SXEL64I(fmt,a1,a2,a3,a4)
#define SXEL65I(fmt,a1,a2,a3,a4,a5)
#define SXEL66I(fmt,a1,a2,a3,a4,a5,a6)
#define SXEL67I(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEL68I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEL69I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEE60I(fmt)
#define SXEE61I(fmt,a1)
#define SXEE62I(fmt,a1,a2)
#define SXEE63I(fmt,a1,a2,a3)
#define SXEE64I(fmt,a1,a2,a3,a4)
#define SXEE65I(fmt,a1,a2,a3,a4,a5)
#define SXEE66I(fmt,a1,a2,a3,a4,a5,a6)
#define SXEE67I(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEE68I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEE69I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXER60I(fmt)
#define SXER61I(fmt,a1)
#define SXER62I(fmt,a1,a2)
#define SXER63I(fmt,a1,a2,a3)
#define SXER64I(fmt,a1,a2,a3,a4)
#define SXER65I(fmt,a1,a2,a3,a4,a5)
#define SXER66I(fmt,a1,a2,a3,a4,a5,a6)
#define SXER67I(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXER68I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXER69I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEA60I(con,fmt)
#define SXEA61I(con,fmt,a1)
#define SXEA62I(con,fmt,a1,a2)
#define SXEA63I(con,fmt,a1,a2,a3)
#define SXEA64I(con,fmt,a1,a2,a3,a4)
#define SXEA65I(con,fmt,a1,a2,a3,a4,a5)
#define SXEA66I(con,fmt,a1,a2,a3,a4,a5,a6)
#define SXEA67I(con,fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEA68I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEA69I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEV60I(func,con,fmt)                            ((void)(func))
#define SXEV61I(func,con,fmt,a1)                         ((void)(func))
#define SXEV62I(func,con,fmt,a1,a2)                      ((void)(func))
#define SXEV63I(func,con,fmt,a1,a2,a3)                   ((void)(func))
#define SXEV64I(func,con,fmt,a1,a2,a3,a4)                ((void)(func))
#define SXEV65I(func,con,fmt,a1,a2,a3,a4,a5)             ((void)(func))
#define SXEV66I(func,con,fmt,a1,a2,a3,a4,a5,a6)          ((void)(func))
#define SXEV67I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       ((void)(func))
#define SXEV68I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    ((void)(func))
#define SXEV69I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) ((void)(func))
#define SXED60I(ptr,len)

#define SXEL70I(fmt)
#define SXEL71I(fmt,a1)
#define SXEL72I(fmt,a1,a2)
#define SXEL73I(fmt,a1,a2,a3)
#define SXEL74I(fmt,a1,a2,a3,a4)
#define SXEL75I(fmt,a1,a2,a3,a4,a5)
#define SXEL76I(fmt,a1,a2,a3,a4,a5,a6)
#define SXEL77I(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEL78I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEL79I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEE70I(fmt)
#define SXEE71I(fmt,a1)
#define SXEE72I(fmt,a1,a2)
#define SXEE73I(fmt,a1,a2,a3)
#define SXEE74I(fmt,a1,a2,a3,a4)
#define SXEE75I(fmt,a1,a2,a3,a4,a5)
#define SXEE76I(fmt,a1,a2,a3,a4,a5,a6)
#define SXEE77I(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEE78I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEE79I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXER70I(fmt)
#define SXER71I(fmt,a1)
#define SXER72I(fmt,a1,a2)
#define SXER73I(fmt,a1,a2,a3)
#define SXER74I(fmt,a1,a2,a3,a4)
#define SXER75I(fmt,a1,a2,a3,a4,a5)
#define SXER76I(fmt,a1,a2,a3,a4,a5,a6)
#define SXER77I(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXER78I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXER79I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEA70I(con,fmt)
#define SXEA71I(con,fmt,a1)
#define SXEA72I(con,fmt,a1,a2)
#define SXEA73I(con,fmt,a1,a2,a3)
#define SXEA74I(con,fmt,a1,a2,a3,a4)
#define SXEA75I(con,fmt,a1,a2,a3,a4,a5)
#define SXEA76I(con,fmt,a1,a2,a3,a4,a5,a6)
#define SXEA77I(con,fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEA78I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEA79I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEV70I(func,con,fmt)                            ((void)(func))
#define SXEV71I(func,con,fmt,a1)                         ((void)(func))
#define SXEV72I(func,con,fmt,a1,a2)                      ((void)(func))
#define SXEV73I(func,con,fmt,a1,a2,a3)                   ((void)(func))
#define SXEV74I(func,con,fmt,a1,a2,a3,a4)                ((void)(func))
#define SXEV75I(func,con,fmt,a1,a2,a3,a4,a5)             ((void)(func))
#define SXEV76I(func,con,fmt,a1,a2,a3,a4,a5,a6)          ((void)(func))
#define SXEV77I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       ((void)(func))
#define SXEV78I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    ((void)(func))
#define SXEV79I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) ((void)(func))
#define SXED70I(ptr,len)

#define SXEL80I(fmt)
#define SXEL81I(fmt,a1)
#define SXEL82I(fmt,a1,a2)
#define SXEL83I(fmt,a1,a2,a3)
#define SXEL84I(fmt,a1,a2,a3,a4)
#define SXEL85I(fmt,a1,a2,a3,a4,a5)
#define SXEL86I(fmt,a1,a2,a3,a4,a5,a6)
#define SXEL87I(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEL88I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEL89I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEE80I(fmt)
#define SXEE81I(fmt,a1)
#define SXEE82I(fmt,a1,a2)
#define SXEE83I(fmt,a1,a2,a3)
#define SXEE84I(fmt,a1,a2,a3,a4)
#define SXEE85I(fmt,a1,a2,a3,a4,a5)
#define SXEE86I(fmt,a1,a2,a3,a4,a5,a6)
#define SXEE87I(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEE88I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEE89I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXER80I(fmt)
#define SXER81I(fmt,a1)
#define SXER82I(fmt,a1,a2)
#define SXER83I(fmt,a1,a2,a3)
#define SXER84I(fmt,a1,a2,a3,a4)
#define SXER85I(fmt,a1,a2,a3,a4,a5)
#define SXER86I(fmt,a1,a2,a3,a4,a5,a6)
#define SXER87I(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXER88I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXER89I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEA80I(con,fmt)
#define SXEA81I(con,fmt,a1)
#define SXEA82I(con,fmt,a1,a2)
#define SXEA83I(con,fmt,a1,a2,a3)
#define SXEA84I(con,fmt,a1,a2,a3,a4)
#define SXEA85I(con,fmt,a1,a2,a3,a4,a5)
#define SXEA86I(con,fmt,a1,a2,a3,a4,a5,a6)
#define SXEA87I(con,fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEA88I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEA89I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEV80I(func,con,fmt)                            ((void)(func))
#define SXEV81I(func,con,fmt,a1)                         ((void)(func))
#define SXEV82I(func,con,fmt,a1,a2)                      ((void)(func))
#define SXEV83I(func,con,fmt,a1,a2,a3)                   ((void)(func))
#define SXEV84I(func,con,fmt,a1,a2,a3,a4)                ((void)(func))
#define SXEV85I(func,con,fmt,a1,a2,a3,a4,a5)             ((void)(func))
#define SXEV86I(func,con,fmt,a1,a2,a3,a4,a5,a6)          ((void)(func))
#define SXEV87I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       ((void)(func))
#define SXEV88I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    ((void)(func))
#define SXEV89I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) ((void)(func))
#define SXED80I(ptr,len)

#define SXEL90I(fmt)
#define SXEL91I(fmt,a1)
#define SXEL92I(fmt,a1,a2)
#define SXEL93I(fmt,a1,a2,a3)
#define SXEL94I(fmt,a1,a2,a3,a4)
#define SXEL95I(fmt,a1,a2,a3,a4,a5)
#define SXEL96I(fmt,a1,a2,a3,a4,a5,a6)
#define SXEL97I(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEL98I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEL99I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEE90I(fmt)
#define SXEE91I(fmt,a1)
#define SXEE92I(fmt,a1,a2)
#define SXEE93I(fmt,a1,a2,a3)
#define SXEE94I(fmt,a1,a2,a3,a4)
#define SXEE95I(fmt,a1,a2,a3,a4,a5)
#define SXEE96I(fmt,a1,a2,a3,a4,a5,a6)
#define SXEE97I(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEE98I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEE99I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXER90I(fmt)
#define SXER91I(fmt,a1)
#define SXER92I(fmt,a1,a2)
#define SXER93I(fmt,a1,a2,a3)
#define SXER94I(fmt,a1,a2,a3,a4)
#define SXER95I(fmt,a1,a2,a3,a4,a5)
#define SXER96I(fmt,a1,a2,a3,a4,a5,a6)
#define SXER97I(fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXER98I(fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXER99I(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEA90I(con,fmt)
#define SXEA91I(con,fmt,a1)
#define SXEA92I(con,fmt,a1,a2)
#define SXEA93I(con,fmt,a1,a2,a3)
#define SXEA94I(con,fmt,a1,a2,a3,a4)
#define SXEA95I(con,fmt,a1,a2,a3,a4,a5)
#define SXEA96I(con,fmt,a1,a2,a3,a4,a5,a6)
#define SXEA97I(con,fmt,a1,a2,a3,a4,a5,a6,a7)
#define SXEA98I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)
#define SXEA99I(con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define SXEV90I(func,con,fmt)                            ((void)(func))
#define SXEV91I(func,con,fmt,a1)                         ((void)(func))
#define SXEV92I(func,con,fmt,a1,a2)                      ((void)(func))
#define SXEV93I(func,con,fmt,a1,a2,a3)                   ((void)(func))
#define SXEV94I(func,con,fmt,a1,a2,a3,a4)                ((void)(func))
#define SXEV95I(func,con,fmt,a1,a2,a3,a4,a5)             ((void)(func))
#define SXEV96I(func,con,fmt,a1,a2,a3,a4,a5,a6)          ((void)(func))
#define SXEV97I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7)       ((void)(func))
#define SXEV98I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8)    ((void)(func))
#define SXEV99I(func,con,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9) ((void)(func))
#define SXED90I(ptr,len)
#endif

#include "sxe-log-proto.h"
#include "sxe-str-encode-proto.h"
#include "sxe-time-proto.h"

#endif /* __SXE_LOG_H__ */
