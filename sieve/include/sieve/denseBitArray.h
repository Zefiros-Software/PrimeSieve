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
#ifndef __PRIMESIEVE_DENSEBITARRAY_H__
#define __PRIMESIEVE_DENSEBITARRAY_H__

#include <vector>

template< typename tBase >
class DenseBitArray
{
public:

    explicit DenseBitArray( size_t size )
        : mBits( ( size + mBitsInBase - 1 ) / mBitsInBase, ~( mBit ^ mBit ) ),
          mSize( size ),
          mCursor( 0 )
    {
    }

    std::vector< tBase > &GetRaw()
    {
        return mBits;
    }

    void SetFalse( size_t i )
    {
        const size_t iDiv = i / mBitsInBase;

        const tBase bit = mBits[iDiv] & ( mBit << ( i % mBitsInBase ) );
        mBits[iDiv] ^= bit;
    }

    bool Get( size_t i ) const
    {
        return ( mBits[i / mBitsInBase] & ( mBit << ( i % mBitsInBase ) ) ) > 0;
    }

    void Reset()
    {
        std::fill( mBits.begin(), mBits.end(), ~0x0 );
    }

    size_t Size() const
    {
        return mSize;
    }

private:

    static constexpr size_t mBitsInBase = sizeof( tBase ) * 8;
    static constexpr tBase mBit = 0x1;

    std::vector< tBase > mBits;
    size_t mSize;
    size_t mCursor;

};

#endif
