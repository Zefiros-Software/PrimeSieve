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
#include "sieve/simple/denseBitArray.h"

DenseBitArray::DenseBitArray() : mSize( 0 )
{

}

std::vector< uint8_t > &DenseBitArray::GetRaw()
{
    return mBits;
}

void DenseBitArray::SetFalse( uint64_t i )
{
    const uint64_t iDiv = i / mBitsInBase;

    const uint8_t bit = mBits[iDiv] & ( mBit << ( i % mBitsInBase ) );
    mBits[iDiv] ^= bit;
}

bool DenseBitArray::Get( uint64_t i ) const
{
    return ( mBits[i / mBitsInBase] & ( mBit << ( i % mBitsInBase ) ) ) > 0;
}

void DenseBitArray::Reset()
{
    std::fill( mBits.begin(), mBits.end(), ~0x0 );
}

void DenseBitArray::Reset( uint64_t size )
{
    mBits.resize( ( size + mBitsInBase - 1 ) / mBitsInBase );
    mSize = size;
    std::fill( mBits.begin(), mBits.end(), ~0x0 );
}

uint64_t DenseBitArray::Size() const
{
    return mSize;
}
