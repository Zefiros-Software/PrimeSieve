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

class DenseBitArray
{
public:

    explicit DenseBitArray();

    std::vector< uint8_t > &GetRaw();

    void SetFalse( uint64_t i );

    bool Get( uint64_t i ) const;

    void Reset();

    void Reset( uint64_t size );

    uint64_t Size() const;

    static constexpr uint64_t GetNumbersPerByte()
    {
        return mBitsInBase;
    }

private:

    static constexpr uint64_t mBitsInBase = sizeof( uint8_t ) * 8;
    static constexpr uint8_t mBit = 0x1;

    std::vector< uint8_t > mBits;
    uint64_t mSize;

};

#endif
