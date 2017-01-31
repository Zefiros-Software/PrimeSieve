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
#ifndef __PRIMESIEVE_SIEVE1_H__
#define __PRIMESIEVE_SIEVE1_H__

#include "sieve/helper.h"

#include <assert.h>
#include <vector>

class AbstractNaivePrimeSieve
{
public:

    explicit AbstractNaivePrimeSieve( size_t size )
        : mIsPrime( size + 1, true ),
          mSize( size + 1 ),
          mProcessed( false )
    {
        assert( size > 1 );
    }

    virtual ~AbstractNaivePrimeSieve()
    {}

    virtual void ExecuteSieve() = 0;

    virtual bool IsPrime( size_t q ) const = 0;

    void Reset()
    {
        mProcessed = false;
        std::fill( mIsPrime.begin(), mIsPrime.end(), true );
    }

protected:

    std::vector<bool> mIsPrime;

    size_t mSize;
    bool mProcessed;

};

class NaivePrimeSieve
    : public AbstractNaivePrimeSieve
{
public:

    explicit NaivePrimeSieve( size_t size )
        : AbstractNaivePrimeSieve( size )
    {
    }

    void ExecuteSieve() override
    {
        mIsPrime[0] = false;
        mIsPrime[1] = false;

        for ( size_t p = 2; p < mSize; ++p )
        {
            if ( mIsPrime[p] )
            {
                SievePrime( p );
            }
        }

        mProcessed = true;
    }

    bool IsPrime( size_t q ) const override
    {
        assert( mProcessed && "You should first execute the sieve before querying" );

        return mIsPrime[q];
    }

protected:

    void SievePrime( size_t p )
    {
        for ( size_t q = 2 * p; q < mSize; q += p )
        {
            mIsPrime[q] = false;
        }
    }
};

class NaivePrimeSieve2
    : public NaivePrimeSieve
{
public:
    explicit NaivePrimeSieve2( size_t size )
        : NaivePrimeSieve( size )
    {
    }

    void ExecuteSieve() override
    {
        mIsPrime[0] = false;
        mIsPrime[1] = false;

        // Sieve the only even prime number
        SievePrime( 2 );

        // Only check whether odd numbers are prime
        for ( size_t p = 3, sqrtP = CeilSqrt( mSize ); p < sqrtP; p += 2 )
        {
            if ( mIsPrime[p] )
            {
                SievePrime( p );
            }
        }

        mProcessed = true;
    }
};

class NaivePrimeSieve3
    : public AbstractNaivePrimeSieve
{
public:
    explicit NaivePrimeSieve3( size_t size )
        : AbstractNaivePrimeSieve( size )
    {
    }

    void ExecuteSieve() override
    {
        mIsPrime[0] = false;
        mIsPrime[1] = false;

        // Only check whether odd numbers are prime
        for ( size_t p = 3, sqrtP = CeilSqrt( mSize ); p < sqrtP; p += 2 )
        {
            if ( mIsPrime[p] )
            {
                SievePrime( p );
            }
        }

        mProcessed = true;
    }

    bool IsPrime( size_t q ) const override
    {
        assert( mProcessed && "You should first execute the sieve before querying" );

        return ( ( q & 0x1 ) && mIsPrime[q] ) || q == 2;
    }

protected:

    void SievePrime( size_t p )
    {
        for ( size_t q = 3 * p, skip = 2 * p; q < mSize; q += skip )
        {
            mIsPrime[q] = false;
        }
    }
};

// 1 --> 0
// 3 --> 1
// 5 --> 2
// ...
class NaivePrimeSieve4
{
public:
    explicit NaivePrimeSieve4( size_t size )
        : mIsPrime( ( size + 1 ) >> 1, true ),
          mSize( size + 1 ),
          mPosSize( NumToPos( size + 1 ) ),
          mSqrtSize( NumToPos( CeilSqrt( size + 1 ) ) ),
          mProcessed( false )
    {
    }

    void ExecuteSieve()
    {
        // 1 is not a prime number
        mIsPrime[0] = false;

        // Only check whether odd numbers are prime
        for ( size_t pStar = NumToPos( 3 ); pStar < mSqrtSize; ++pStar )
        {
            if ( mIsPrime[pStar] )
            {
                SievePrime( pStar );
            }
        }

        mProcessed = true;
    }

    bool IsPrime( size_t q ) const
    {
        assert( mProcessed && "You should first execute the sieve before querying" );

        return ( ( q & 0x1 ) && mIsPrime[NumToPos( q )] ) || q == 2;
    }

    void Reset()
    {
        mProcessed = false;
        std::fill( mIsPrime.begin(), mIsPrime.end(), true );
    }

protected:

    std::vector<bool> mIsPrime;

    size_t mSize;
    size_t mPosSize;
    size_t mSqrtSize;
    bool mProcessed;

    void SievePrime( size_t pStar )
    {
        for ( size_t skip = PosToNum( pStar ), q = pStar + skip; q < mPosSize; q += skip )
        {
            mIsPrime[q] = false;
        }
    }

    constexpr static size_t NumToPos( size_t num )
    {
        return num >> 1;
    }

    constexpr static size_t PosToNum( size_t pos )
    {
        return ( pos << 1 ) + 1;
    }
};

#endif