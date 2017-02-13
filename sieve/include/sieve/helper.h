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
#ifndef __PRIMESIEVE_HELPER_H__
#define __PRIMESIEVE_HELPER_H__

#include <complex>

template< typename tT >
tT CeilSqrt( tT val )
{
    return static_cast<tT>( std::ceil( std::sqrt( val ) ) );
}

template< typename tT >
tT Sqrt( tT val )
{
    return static_cast<tT>( std::sqrt( val ) );
}


template< typename tT, tT tN1, tT tN2, tT tN3 >
class Wheel
{
public:

    Wheel()
        : mWheel1( 0x0 ),
          mWheel2( 0x0 )
    {
        ExtendWheel<tN1>();
        ExtendWheel<tN2>();
        ExtendWheel<tN3>();
    }

    tT GetMask( size_t i )
    {
        size_t boundary = i * tSize;
        boundary -= ( boundary / tCircumference ) * tCircumference;

        if ( boundary == 0 )
        {
            return mWheel1;
        }

        if ( boundary < tSize )
        {
            return ( ( mWheel1 >> boundary ) | ( mWheel2 << ( tSize - boundary ) ) );
        }

        if ( boundary == tSize )
        {
            return mWheel2;
        }
        else
        {
            boundary -= tSize;
            return ( ( mWheel2 >> boundary ) | ( mWheel1 << ( tCircumference - tSize - boundary ) ) );
        }
    }

    static constexpr size_t GetCircumference()
    {
        return tCircumference;
    }

private:

    static constexpr size_t tCircumference = tN1 * tN2 * tN3;
    static constexpr size_t tSize = sizeof( tT ) * 8;

    tT mWheel1;
    tT mWheel2;

    template< tT tN >
    void ExtendWheel()
    {
        constexpr tT bit = 0x1;

        size_t i;

        for ( i = ( tN >> 1 ); i < 64; i += tN )
        {
            mWheel1 |= bit << i;
        }

        for ( i -= 64; i < 64; i += tN )
        {
            mWheel2 |= bit << i;
        }
    }
};

#endif
