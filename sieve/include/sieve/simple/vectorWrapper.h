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
#ifndef __PRIMESIEVE_VECTORWRAPPER_H__
#define __PRIMESIEVE_VECTORWRAPPER_H__

#include <vector>

template< typename tBase >
class VectorWrapper
{
public:

    static constexpr tBase tTrue = 0x1;
    static constexpr tBase tFalse = 0x0;

    explicit VectorWrapper()
        : mSize( 0 )
    {
    }

    void SetFalse( uint64_t i )
    {
        mBooleans[i] ^= mBooleans[i];
    }

    bool Get( uint64_t i ) const
    {
        return mBooleans[i] == tTrue;
    }

    static constexpr uint64_t GetNumbersPerByte()
    {
        return 1;
    }

    void Reset()
    {
        std::fill( mBooleans.begin(), mBooleans.end(), tTrue );
    }

    void Reset( uint64_t size )
    {
        mBooleans.resize( size );
        Reset();
    }

    uint64_t Size() const
    {
        return mSize;
    }

private:

    std::vector< tBase > mBooleans;
    uint64_t mSize;

};

#endif