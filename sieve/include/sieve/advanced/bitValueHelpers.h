/**
 * Copyright (c) 2017 Zefiros Software.
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
#pragma once
#ifndef __PRIMESIEVE_BITVALUEHELPERS_H__
#define __PRIMESIEVE_BITVALUEHELPERS_H__

#include <stdint.h>
#include <assert.h>


template< uint64_t tOffsetMod30 >
constexpr static uint8_t GetBitIndex()
{
    static_assert( false, "This should not be called" );
    throw;
}

#define SIEVE_BITINDEX_OVERRIDE( byteValue, bitIndex )  \
template<>                                              \
constexpr uint8_t GetBitIndex<byteValue>()              \
{                                                       \
    return bitIndex;                                    \
}

SIEVE_BITINDEX_OVERRIDE( 7, 0 );
SIEVE_BITINDEX_OVERRIDE( 11, 1 );
SIEVE_BITINDEX_OVERRIDE( 13, 2 );
SIEVE_BITINDEX_OVERRIDE( 17, 3 );
SIEVE_BITINDEX_OVERRIDE( 19, 4 );
SIEVE_BITINDEX_OVERRIDE( 23, 5 );
SIEVE_BITINDEX_OVERRIDE( 29, 6 );
SIEVE_BITINDEX_OVERRIDE( 31, 7 );

#define SIEVE_ALL_BITVALUES_BASE( templateFunction, ... )   \
templateFunction< 7>( __VA_ARGS__ );                        \
templateFunction<11>( __VA_ARGS__ );                        \
templateFunction<13>( __VA_ARGS__ );                        \
templateFunction<17>( __VA_ARGS__ );                        \
templateFunction<19>( __VA_ARGS__ );                        \
templateFunction<23>( __VA_ARGS__ );                        \
templateFunction<29>( __VA_ARGS__ );

#define SIEVE_ALL_BITVALUES( templateFunction, ... )        \
SIEVE_ALL_BITVALUES_BASE( templateFunction, __VA_ARGS__ );  \
templateFunction<31>( __VA_ARGS__ );

#define SIEVE_ALL_BITVALUES_MOD( templateFunction, ... )    \
templateFunction<1>( __VA_ARGS__ );                         \
SIEVE_ALL_BITVALUES_BASE( templateFunction, __VA_ARGS__ );

constexpr static uint8_t GetBitValue( uint8_t bitIndex )
{
    return bitIndex == 0 ? 7 : (
               bitIndex == 1 ? 11 : (
                   bitIndex == 2 ? 13 : (
                       bitIndex == 3 ? 17 : (
                           bitIndex == 4 ? 19 : (
                               bitIndex == 5 ? 23 : (
                                   bitIndex == 6 ? 29 : 31 ) ) ) ) ) );
}

constexpr static uint8_t GetWheelIndex( uint8_t offsetMod30 )
{
    return offsetMod30 == 7 ? 1 : (
               offsetMod30 == 11 ? 2 : (
                   offsetMod30 == 13 ? 3 : (
                       offsetMod30 == 17 ? 4 : (
                           offsetMod30 == 19 ? 5 : (
                               offsetMod30 == 23 ? 6 : (
                                   offsetMod30 == 29 ? 7 : 0 ) ) ) ) ) );
}

template< uint64_t tOffsetMod30 >
__forceinline constexpr static uint8_t GetBitMask()
{
    return 1 << GetBitIndex<tOffsetMod30>();
}

template< uint64_t tOffsetMod30 >
__forceinline constexpr static uint8_t OffsetMask()
{
    return ~( GetBitMask<tOffsetMod30>() );
}

__forceinline constexpr uint8_t GetRest( uint8_t r, uint64_t m )
{
    return ( ( r * m ) % 30 ) == 1 ? 31 : ( ( r * m ) % 30 );
}

#endif
