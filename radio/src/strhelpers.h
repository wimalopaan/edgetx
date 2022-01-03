/*
 * Copyright (C) EdgeTX
 *
 * Based on code named
 *   opentx - https://github.com/opentx/opentx
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _STRHELPERS_H_
#define _STRHELPERS_H_

#include "definitions.h"
#include "opentx_types.h"

#include <array>
#include <cassert>

#define SHOW_TIME  0x1
#define SHOW_TIMER 0x0
#define SHOW_TIMER_UPPER_CASE   0x2
#define SHOW_TIMER_HM_FORMAT    0x5

PACK(typedef struct {
  uint8_t showTime:1;
  uint8_t upperCase:1;
  uint8_t numDigitGroups:3;  
  uint8_t hmFormat:1;
  uint8_t reserved:2;
}) TimerDisplayOptions;

typedef union  {
  uint8_t options;
  TimerDisplayOptions displayOptions;
} TimerOptions;

char hex2zchar(uint8_t hex);
char hex2char(uint8_t hex);
char zchar2char(int8_t idx);
char char2lower(char c);
int8_t char2zchar(char c);
void str2zchar(char *dest, const char *src, int size);
int zchar2str(char *dest, const char *src, int size);
int strnlen(const char *src, int max_size);
unsigned int effectiveLen(const char * str, unsigned int size);

char *strAppend(char *dest, const char *source, int len = 0);
char *strAppendUnsigned(char *dest, uint32_t value, uint8_t digits = 0,
                        uint8_t radix = 10);
char *strAppendSigned(char *dest, int32_t value, uint8_t digits = 0,
                      uint8_t radix = 10);
char *strSetCursor(char *dest, int position);
char *strAppendDate(char *str, bool time = false);
char *strAppendFilename(char *dest, const char *filename, const int size);

#if !defined(BOOT)
char *getStringAtIndex(char *dest, const char *s, int idx);
char *strAppendStringWithIndex(char *dest, const char *s, int idx);
#define LEN_TIMER_STRING 10  // "-00:00:00"
char *getTimerString(char *dest, int32_t tme, TimerOptions timerOptions = {.options = 0});
char *getFormattedTimerString(char *dest, int32_t tme, TimerOptions timerOptions);
char *getCurveString(char *dest, int idx);
char *getGVarString(char *dest, int idx);
char *getGVarString(int idx);
char *getSwitchPositionName(char *dest, swsrc_t idx);
char *getSwitchName(char *dest, swsrc_t idx);
char *getSourceString(char *dest, mixsrc_t idx);
int  getRawSwitchIdx(char sw);
char getRawSwitchFromIdx(int sw);
#endif

char *getFlightModeString(char *dest, int8_t idx);
#define SWITCH_WARNING_STR_SIZE 3
// char *getSwitchWarningString(char *dest, swsrc_t idx);

char *getSourceString(mixsrc_t idx);
char *getSwitchPositionName(swsrc_t idx);
// char *getSwitchWarningString(swsrc_t idx);
char *getCurveString(int idx);
char *getTimerString(int32_t tme, TimerOptions timerOptions = {.options = 0});
void splitTimer(char *s0, char *s1, char *s2, char *s3, int tme,
                bool bLowercase = true);

#if (__cplusplus >= 201707L)

template<typename C, size_t L>
struct LString {
    consteval LString(const C* const p) : ptr{p} {} // immdiate-function: only for string-literals
    constexpr C operator[](const size_t i) const {
        assert(i < L);
        return *(ptr + i);
    }
private:
    const C* ptr{};
};

namespace detail {
    template<typename C>
    struct size;
    template<typename E, size_t N>
    struct size<E[N]> {
        static inline constexpr size_t value = N;
    };
    template<typename C, size_t N>
    struct size<LString<C, N>> {
        static inline constexpr size_t value = N;            
    };
    
    template<typename T, typename... TT>
    struct front {
        using type = T;
    };
    
    template<typename C>
    struct item_type;
    template<typename E, size_t N>
    struct item_type<E[N]> {
        using type = E;   
    };
    template<typename E, size_t N>
    struct item_type<LString<E, N>> {
        using type = E;   
    };        
}    
template<typename C>
constexpr auto size_v = detail::size<C>::value;

template<typename C>
using item_type_t = detail::item_type<C>::type;

template<typename... TT>
using front = detail::front<TT...>::type;

struct AlwaysNullTerminated {
    static inline constexpr size_t addToLength{1};
};
struct MaybeNullTerminated {
    static inline constexpr size_t addToLength{0};    
};

template<typename Term, typename... C>
constexpr auto concat(Term, const C&...  c) {
    constexpr size_t length = (size_v<C> + ...) + Term::addToLength;
    
    using item_type = std::remove_cvref_t<item_type_t<front<C...>>>;
    static_assert((std::is_same_v<item_type, std::remove_cvref_t<item_type_t<C>>> && ...));
    
    using result_t = std::array<item_type, length>;
    result_t result;
   
    size_t index{0};
    auto insert = [&]<typename CC>(const CC& c) {
                  for(size_t i = 0; i < size_v<CC>; ++i, ++index) {
                    if (c[i] != '\0') {
                        assert(index < length);
                        result[index] = c[i];
                    }
                    else {
                        break;
                    }
                }
            };
     (insert(c), ...);        
     if (index < length) {
          result[index] = '\0';
     }
     return result;
}    

template<typename E, size_t N>
consteval auto lstring(const E(&a)[N]) { // immediate function at compile-time
    static_assert(N >= 1);
    return LString<E, N - 1>(a);
}
#define LStr(s) lstring(s)


#else
template<typename C, size_t L>
struct LString {
    constexpr LString(const C* const p) : ptr{p} {} // should be consteval
    constexpr C operator[](const size_t i) const {
        return *(ptr + i);
    }
private:
    const C* const ptr{};
};

template<typename C>
struct size;
template<typename E, size_t N>
struct size<E[N]> {
    static constexpr size_t value = N;
};
template<typename C, size_t N>
struct size<LString<C, N>> {
    static constexpr size_t value = N;            
};

template<typename C, typename... CC>
struct totalsize {
    static constexpr size_t value = size<C>::value + totalsize<CC...>::value;  
};
template<typename C>
struct totalsize<C> {
    static constexpr size_t value = size<C>::value;  
};

template<typename E, size_t N>
constexpr LString<E, N - 1> lstring(const E(&a)[N]) { // should be consteval
    static_assert(N >= 1);
    return LString<E, N - 1>(a);
}

#define LStr(s) lstring(s)

template<typename O, typename C>
void insert(O& out, size_t& index, const C& c) {
    for(size_t i = 0; i < size<C>::value; ++i, ++index) {
        if (c[i] != '\0') {
            out[index] = c[i];
        }
        else {
            break;
        }
    }    
}
template<typename O, typename C, typename... CC>
void insert(O& out, size_t& index, const C& c, const CC&... cc) {
    insert(out, index, c);    
    insert(out, index, cc...);    
}

struct AlwaysNullTerminated {};
struct MaybeNullTerminated {};

template<typename... C>
constexpr auto concat(MaybeNullTerminated, const C& ... c) -> std::array<char, totalsize<C...>::value> {
    constexpr size_t length = totalsize<C...>::value;
    std::array<char, length> result; 
    size_t index{0};
    insert(result, index, c...);
    if (index < length) {
        result[index] = '\0';
    }
    return result;
}

template<typename... C>
constexpr auto concat(AlwaysNullTerminated, const C& ... c) -> std::array<char, totalsize<C...>::value + 1> {
    constexpr size_t length = totalsize<C...>::value + 1;
    std::array<char, length> result; 
    size_t index{0};
    insert(result, index, c...);
    result[index] = '\0';
    return result;
}
#endif

#endif  // _STRHELPERS_H_
