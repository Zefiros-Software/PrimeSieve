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
#ifndef __PRIMESIEVE_SIMPLESIEVE_H__
#define __PRIMESIEVE_SIMPLESIEVE_H__

#include "sieve/helper.h"

#include <algorithm>
#include <stdint.h>
#include <vector>

constexpr uint8_t tFalse = 0x0;
constexpr uint8_t tTrue = 0x1;

template< typename tContainer >
class PrimeSieve
{
public:

    template< typename tOutput >
    void ExecuteSieve( uint64_t limit, tOutput &output )
    {
        mIsPrime.Reset( limit );

        // 0 and 1 are not prime
        mIsPrime.SetFalse( 0 );
        mIsPrime.SetFalse( 1 );

        for ( uint64_t p = 2; p < limit; ++p )
        {
            if ( mIsPrime.Get( p ) )
            {
                output << p;
                SievePrime( p, limit );
            }
        }
    }

private:

    tContainer mIsPrime;

    void SievePrime( uint64_t p, uint64_t limit )
    {
        for ( uint64_t q = 2 * p; q < limit; q += p )
        {
            mIsPrime.SetFalse( q );
        }
    }
};

template< typename tContainer >
class PrimeSieve2
{
public:

    template< typename tOutput >
    void ExecuteSieve( uint64_t limit, tOutput &output )
    {
        mIsPrime.Reset( limit );

        mIsPrime.SetFalse( 0 );
        mIsPrime.SetFalse( 1 );

        // Sieve the only even prime number
        output << 2;
        SievePrimeOld( 2, limit );

        uint64_t p = 3;

        // Only check whether odd numbers are prime
        // Stop at floor( sqrt( limit ) )
        for ( uint64_t sqrtLimit = Sqrt( limit ); p < sqrtLimit; p += 2 )
        {
            if ( mIsPrime.Get( p ) )
            {
                output << p;
                SievePrime( p, limit );
            }
        }

        output.MaybeCall( [&]()
        {
            // Output the remaining primes
            for ( ; p < limit; p += 2 )
            {
                if ( mIsPrime.Get( p ) )
                {
                    output << p;
                }
            };
        } );
    }

private:

    tContainer mIsPrime;

    void SievePrime( uint64_t p, uint64_t limit )
    {
        for ( uint64_t q = p * p, skip = 2 * p; q < limit; q += skip )
        {
            mIsPrime.SetFalse( q );
        }
    }

    // This is still needed for p = 2
    void SievePrimeOld( uint64_t p, uint64_t limit )
    {
        for ( uint64_t q = p * p; q < limit; q += p )
        {
            mIsPrime.SetFalse( q );
        }
    }
};

template< typename tContainer >
class PrimeSieve3
{
public:

    template< typename tOutput >
    void ExecuteSieve( uint64_t limit, tOutput &output )
    {
        mIsPrime.Reset( limit );

        // Since we know 0 and 1 are not prime,
        // and since we know 2 is the only even prime number,
        // we can skip setting 0 and 1 to false and we can skip sieving 2
        output << 2;

        uint64_t p = 3;

        // Only check whether odd numbers are prime
        // Stop at floor( sqrt( limit ) )
        for ( uint64_t sqrtLimit = Sqrt( limit ); p < sqrtLimit; p += 2 )
        {
            if ( mIsPrime.Get( p ) )
            {
                output << p;
                SievePrime( p, limit );
            }
        }

        output.MaybeCall( [&]()
        {
            // Output the remaining primes
            for ( ; p < limit; p += 2 )
            {
                if ( mIsPrime.Get( p ) )
                {
                    output << p;
                }
            };
        } );
    }

private:

    tContainer mIsPrime;

    void SievePrime( uint64_t p, uint64_t limit )
    {
        for ( uint64_t q = p * p, skip = 2 * p; q < limit; q += skip )
        {
            mIsPrime.SetFalse( q );
        }
    }
};

// 1 --> 0
// 3 --> 1
// 5 --> 2
// ...
template< typename tContainer >
class PrimeSieve4
{
public:

    template< typename tOutput >
    void ExecuteSieve( uint64_t limit, tOutput &output )
    {
        const uint64_t posLimit = NumToPos( limit + 1 );
        mIsPrime.Reset( posLimit );

        output << 2;

        uint64_t pStar = NumToPos( 3 );

        // Only check whether odd numbers are prime
        for ( uint64_t sqrtPosLimit = NumToPos( Sqrt( limit ) ); pStar < sqrtPosLimit; ++pStar )
        {
            if ( mIsPrime.Get( pStar ) )
            {
                const uint64_t p = PosToNum( pStar );

                output << p;
                SievePrime( p, posLimit );
            }
        }

        output.MaybeCall( [&]()
        {
            // Output the remaining primes
            for ( ; pStar < posLimit; ++pStar )
            {
                if ( mIsPrime.Get( pStar ) )
                {
                    output << PosToNum( pStar );
                }
            };
        } );
    }

protected:

    tContainer mIsPrime;

    void SievePrime( uint64_t p, uint64_t posLimit )
    {
        for ( uint64_t qStar = NumToPos( p * p ); qStar < posLimit; qStar += p )
        {
            mIsPrime.SetFalse( qStar );
        }
    }

    constexpr static uint64_t NumToPos( uint64_t num )
    {
        return num >> 1;
    }

    constexpr static uint64_t PosToNum( uint64_t pos )
    {
        return ( pos << 1 ) + 1;
    }
};

template< typename tContainer >
class PrimeSieve5
{
public:

    template< typename tOutput >
    void ExecuteSieve( uint64_t limit, tOutput &output )
    {
        const uint64_t segmentSize = std::min( limit, 1024 * 32 * mIsPrime.GetNumbersPerByte() * 2 );
        const uint64_t segmentPosSize = NumToPos( segmentSize );
        mIsPrime.Reset( segmentSize );
        mSievePrimes.clear();

        output << 2;


        uint64_t pStar = NumToPos( 3 );

        // Only check whether odd numbers are prime
        for ( uint64_t sqrtSegmentPosLimit = NumToPos( Sqrt( segmentSize ) + 1 ); pStar < sqrtSegmentPosLimit; ++pStar )
        {
            if ( mIsPrime.Get( pStar ) )
            {
                const uint64_t p = PosToNum( pStar );

                output << p;

                // Append primes that are below the square root of the segment size
                mSievePrimes.push_back( {p, NumToPos( p * p )} );

                // Sieve the prime we just appended
                SievePrime( mSievePrimes.back(), segmentPosSize );
            }
        }

        for ( uint64_t sqrtPosLimit = NumToPos( Sqrt( limit ) ); pStar < sqrtPosLimit; ++pStar )
        {
            if ( mIsPrime.Get( pStar ) )
            {
                const uint64_t p = PosToNum( pStar );

                output << p;

                // Append primes still left below the square root of the limit
                // We know that if such a prime exits, it must lie in the next segment, so we have to subtract the size of the
                // segment to fix the initial position
                mSievePrimes.push_back( { p, NumToPos( p * p ) - segmentPosSize } );
            }
        }

        output.MaybeCall( [&]()
        {
            // Output the remaining primes
            for ( ; pStar < segmentPosSize; ++pStar )
            {
                if ( mIsPrime.Get( pStar ) )
                {
                    const uint64_t p = PosToNum( pStar );

                    output << p;
                }
            };
        } );

        uint64_t pBase = segmentSize;
        uint64_t sqrtPosLimit = NumToPos( Sqrt( limit ) );

        // There may still be primes left we need to sieve
        for ( pStar = segmentPosSize; pStar < sqrtPosLimit; pStar += segmentPosSize, pBase += segmentSize )
        {
            mIsPrime.Reset();
            const uint64_t segmentLimit = std::min( segmentPosSize, NumToPos( limit - pBase ) );

            for ( std::pair<uint64_t, uint64_t> &prime : mSievePrimes )
            {
                SievePrime( prime, segmentLimit );
            }

            uint64_t qStar = 0;

            for ( ; qStar < sqrtPosLimit; ++qStar )
            {
                if ( mIsPrime.Get( qStar ) )
                {
                    const uint64_t p = pBase + PosToNum( qStar );

                    output << p;

                    // We do not need to sieve this prime, since
                    // segmentStart * segmentStart >= segmentSize * segmentStart > segmentStart + segmentSize = segmentEnd
                    mSievePrimes.push_back( { p, NumToPos( p * p ) - pStar } );
                }
            }

            output.MaybeCall( [&]()
            {
                for ( ; qStar < segmentLimit; ++qStar )
                {
                    if ( mIsPrime.Get( qStar ) )
                    {
                        output << ( pBase + PosToNum( qStar ) );
                    }
                }
            } );
        }

        // All primes we need to sieve have been collected
        // Sieve them and output the remaining primes
        for ( ; pBase < limit; pBase += segmentSize )
        {
            mIsPrime.Reset();
            const uint64_t segmentLimit = std::min( segmentPosSize, NumToPos( limit - pBase ) );

            for ( std::pair<uint64_t, uint64_t> &prime : mSievePrimes )
            {
                SievePrime( prime, segmentLimit );
            }

            uint64_t qStar = 0;

            output.MaybeCall( [&]()
            {
                for ( ; qStar < segmentLimit; ++qStar )
                {
                    if ( mIsPrime.Get( qStar ) )
                    {
                        output << ( pBase + PosToNum( qStar ) );
                    }
                }
            } );
        }
    }

protected:

    tContainer mIsPrime;

    std::vector< std::pair< uint64_t, uint64_t > > mSievePrimes;

    void SievePrime( std::pair<uint64_t, uint64_t> &prime, uint64_t posLimit )
    {
        uint64_t qStar = prime.second;

        for ( ; qStar < posLimit; qStar += prime.first )
        {
            mIsPrime.SetFalse( qStar );
        }

        prime.second = qStar - posLimit;
    }

    constexpr static uint64_t NumToPos( uint64_t num )
    {
        return num >> 1;
    }

    constexpr static uint64_t PosToNum( uint64_t pos )
    {
        return ( pos << 1 ) + 1;
    }
};



#endif
