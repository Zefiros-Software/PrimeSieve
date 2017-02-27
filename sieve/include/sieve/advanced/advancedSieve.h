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
#ifndef __PRIMESIEVE_ADVANCEDSIEVE_H__
#define __PRIMESIEVE_ADVANCEDSIEVE_H__

#include "sieve/advanced/bitValueHelpers.h"
#include "sieve/advanced/memoryPool.h"
#include "sieve/helper.h"

#include <algorithm>
#include <xutility>

template< uint8_t tPrimeMod30 >
class PrimeCrosser
{
public:

    template< uint64_t tMultiple >
    __forceinline static void CrossOff( uint8_t *segmentIterator, uint64_t &primeDiv30 )
    {
        constexpr uint64_t m = tMultiple;
        constexpr uint8_t r = tPrimeMod30;
        constexpr uint8_t rest = GetRest( r, m );

        segmentIterator[GetOffset<m>( primeDiv30 )] &= OffsetMask<rest>();
    }

    static uint64_t GetTotalOffset( uint64_t &primeDiv30 )
    {
        return primeDiv30 * 30 + tPrimeMod30;
    }

    template< uint64_t tMultiple1, uint64_t tMultiple2 >
    static uint64_t GetOffsetBetween( uint64_t &primeDiv30 )
    {
        constexpr uint64_t m1 = tMultiple1;
        constexpr uint64_t m2 = tMultiple2;
        constexpr uint64_t r = tPrimeMod30;
        const uint64_t &k = primeDiv30;

        return k * ( m2 - m1 ) + ( r * m2 - GetRest( r, m2 ) ) / 30 - ( r * m1 - GetRest( r, m1 ) ) / 30;
    }

    template< uint64_t tMultiple >
    static uint64_t GetOffset( uint64_t &primeDiv30 )
    {
        constexpr uint64_t m = tMultiple;
        constexpr uint64_t r = tPrimeMod30;
        constexpr uint8_t rest = GetRest( r, m );

        return primeDiv30 * ( m - 1 ) + ( r * m - rest ) / 30;
    }
};

template<>
class PrimeCrosser<31>
{
public:

    template< uint64_t tMultiple >
    static void CrossOff( uint8_t *segmentIterator, uint64_t &primeDiv30 )
    {
        constexpr uint64_t m = tMultiple;
        constexpr uint8_t r = 31;
        constexpr uint8_t rest = GetRest( r, m );

        segmentIterator[GetOffset<m>( primeDiv30 )] &= OffsetMask<rest>();
    }

    static uint64_t GetTotalOffset( uint64_t &primeDiv30 )
    {
        return primeDiv30 * 30 + 1;
    }

    template< uint64_t tMultiple1, uint64_t tMultiple2 >
    static uint64_t GetOffsetBetween( uint64_t &primeDiv30 )
    {
        constexpr uint64_t m1 = tMultiple1;
        constexpr uint64_t m2 = tMultiple2;
        const uint64_t &k = primeDiv30;

        return k * ( m2 - m1 ) + 1 - MaybeAddOne( m1 );
    }

    template< uint64_t tMultiple >
    static uint64_t GetOffset( uint64_t &primeDiv30 )
    {
        constexpr uint64_t m = tMultiple;
        const uint64_t &k = primeDiv30;

        return k * ( m - 1 ) + MaybeAddOne( m );
    }

private:

    static constexpr uint8_t MaybeAddOne( uint8_t m )
    {
        return ( m - 1 ) > 0 ? 1 : 0;
    }
};

template< uint8_t tPrimeMod30 >
class Prime
{
public:

    Prime( uint64_t prime, uint64_t segmentStart )
    {
        Init( prime, segmentStart );
    }

    Prime()
        : mPrimeDiv30( 0 ),
          mNextMultipleIndex( 0 ),
          mWheelIndex( 0 )
    {
    }

    void Init( uint64_t prime, uint64_t segmentStart )
    {
        uint64_t pSquare = prime * prime;

        if ( segmentStart > pSquare )
        {
            throw;
        }

        mPrimeDiv30 = static_cast<uint32_t>( prime / 30 );
        mNextMultipleIndex = static_cast<uint32_t>( ( pSquare - ( ( pSquare % 30 ) == 1 ? 31 : 0 ) ) / 30 - segmentStart / 30 );
        mWheelIndex = GetWheelIndex( prime % 30 );
    }

    __forceinline void SieveSegment( uint8_t *segment, uint32_t segmentSize )
    {
        uint64_t primeDiv30 = mPrimeDiv30;

        uint8_t *q = segment + mNextMultipleIndex;
        uint8_t *sieveEnd = segment + segmentSize / 30;
        uint8_t *loopEnd = sieveEnd - tPrimeCrosser::GetOffset<29>( primeDiv30 );
        uint64_t loopOffset = tPrimeCrosser::GetTotalOffset( primeDiv30 );;

        switch ( mWheelIndex )
        {
            while ( true )
            {
            case 0:
                {
                    for ( ; q < loopEnd; q += loopOffset )
                    {
                        SIEVE_ALL_BITVALUES_MOD( tPrimeCrosser::template CrossOff, q, primeDiv30 );
                    }

                    if ( !HandleCase<0, 1>( q, sieveEnd ) )
                    {
                        break;
                    }

                    q += tPrimeCrosser::template GetOffset<7>( primeDiv30 );
                }

#define SIEVE_CROSSER_CASE_BODY( thisCase, thisPrime, nextPrime )                                       \
            case thisCase:                                                                              \
                {                                                                                       \
                    if ( !HandleCase<thisCase, thisPrime>( q, sieveEnd ) )                              \
                    {                                                                                   \
                        break;                                                                          \
                    }                                                                                   \
                                                                                                        \
                    q += tPrimeCrosser::template GetOffsetBetween<thisPrime, nextPrime>( primeDiv30 );  \
                }

                SIEVE_CROSSER_CASE_BODY( 1,  7, 11 );
                SIEVE_CROSSER_CASE_BODY( 2, 11, 13 );
                SIEVE_CROSSER_CASE_BODY( 3, 13, 17 );
                SIEVE_CROSSER_CASE_BODY( 4, 17, 19 );
                SIEVE_CROSSER_CASE_BODY( 5, 19, 23 );
                SIEVE_CROSSER_CASE_BODY( 6, 23, 29 );
                SIEVE_CROSSER_CASE_BODY( 7, 29, 31 );

#undef SIEVE_CROSSER_CASE_BODY
            }

            mNextMultipleIndex = static_cast<uint32_t>( q - sieveEnd );
        }
    }

    uint8_t GetPrimeMod30() const
    {
        return tPrimeMod30;
    }

private:

    typedef PrimeCrosser< tPrimeMod30 > tPrimeCrosser;

    uint32_t mPrimeDiv30;
    uint32_t mNextMultipleIndex;
    uint8_t mWheelIndex;

    template< uint8_t tWheelIndex, uint8_t tMultiple >
    bool HandleCase( uint8_t *q, uint8_t *qEnd )
    {
        if ( q >= qEnd )
        {
            mWheelIndex = tWheelIndex;
            return false;
        }

        constexpr uint8_t m = tMultiple;
        constexpr uint64_t r = tPrimeMod30;

        *q &= OffsetMask< GetRest( r, m ) >();

        return true;
    }
};

template< uint8_t tPrimeMod30 >
class PrimeStorageBase
{
public:

    template< typename tFactory >
    void ClearPrimes( tFactory &factory )
    {
        for ( Prime<tPrimeMod30> *prime : mPrimes )
        {
            factory.Release( prime );
        }

        mPrimes.clear();
    }

    std::vector<Prime<tPrimeMod30> *> &GetStorage()
    {
        return mPrimes;
    }

private:

    std::vector<Prime<tPrimeMod30 > *> mPrimes;
};

class PrimeStorage
    : PrimeStorageBase< 7>, PrimeStorageBase<11>, PrimeStorageBase<13>, PrimeStorageBase<17>,
      PrimeStorageBase<19>, PrimeStorageBase<23>, PrimeStorageBase<29>, PrimeStorageBase<31>
{
public:

    template< typename tFactory >
    void ClearPrimes( tFactory &factory )
    {
        PrimeStorageBase<7>::ClearPrimes( factory );
        PrimeStorageBase<11>::ClearPrimes( factory );
        PrimeStorageBase<13>::ClearPrimes( factory );
        PrimeStorageBase<17>::ClearPrimes( factory );
        PrimeStorageBase<19>::ClearPrimes( factory );
        PrimeStorageBase<23>::ClearPrimes( factory );
        PrimeStorageBase<29>::ClearPrimes( factory );
        PrimeStorageBase<31>::ClearPrimes( factory );
    }

    template<uint8_t tPrimeMod30>
    __forceinline std::vector<Prime<tPrimeMod30> *> &GetStorage()
    {
        return PrimeStorageBase<tPrimeMod30>::GetStorage();
    }
};

template< typename tFactory >
class AdvancedSieve
{
public:

    typedef tFactory tFactoryType;

    explicit AdvancedSieve( tFactory &factory, std::vector<uint8_t> &segment )
        : mSegment( segment ),
          mFactory( factory )
    {
    }

    ~AdvancedSieve()
    {
        mPrimeStorage.ClearPrimes( mFactory );
    }

    template< typename tOutput >
    void ExecuteSieve( uint64_t limit, tOutput &output )
    {
        mPrimeStorage.ClearPrimes( mFactory );

        std::fill( mSegment.begin(), mSegment.end(), static_cast<uint8_t>( ~0 ) );


        output << 2;
        output << 3;
        output << 5;

        uint64_t p = 0;

        // Every byte contains the numbers:
        //   - i * 30 + 7
        //   - i * 30 + 11
        //   - i * 30 + 13
        //   - i * 30 + 17
        //   - i * 30 + 19
        //   - i * 30 + 23
        //   - i * 30 + 29
        //   - i * 30 + 31
        //
        // The numbers that fit in a segment are the size in bytes * 30
        uint32_t segmentSize = static_cast<uint32_t>( mSegment.size() ) * 30;
        uint64_t pLimit = limit / 30 * 30;

        {
            uint8_t *q = mSegment.data();

            // For each block of numbers that is below the square root of the segment length and the square root of the limit:
            //     If a bit position is still marked prime:
            //         - Cross off multiples of the corresponding number
            //         - Append it to a list of primes we will cross off in the next segment
            //         - Output the number
            for ( uint8_t *end = q + ( std::min<uint64_t>( Sqrt( segmentSize + 7 ), Sqrt( limit ) ) + 29 ) / 30; q < end;
                    ++q, p += 30 )
            {
                SIEVE_ALL_BITVALUES( ProcessBlockPrime, *q, p, 0, segmentSize, output );
            }

            // For each block of numbers that is below the square root of the limit:
            //     If a bit position is still marked prime:
            //         - Append it to a list of primes we will cross off in the next segment
            //         - Output the corresponding number
            for ( uint8_t *end = mSegment.data() + ( Sqrt( limit ) + 29 ) / 30; q < end; ++q, p += 30 )
            {
                SIEVE_ALL_BITVALUES( AppendIfPrime, *q, p, segmentSize, output );
            }

            // For the remaining blocks in the segment:
            //     If a bit position is still marked prime:
            //         - Output the corresponding number
            output.MaybeCall( [&]()
            {
                uint64_t qLimit = std::min<uint64_t>( mSegment.size(), limit );

                uint8_t *end = mSegment.data() + qLimit;

                OutputRemainingPrimesInSegment( q, end, p, pLimit, limit, output );
            } );
        }


        for ( uint64_t segmentStart = segmentSize; segmentStart < limit; segmentStart += segmentSize )
        {
            std::fill( mSegment.begin(), mSegment.end(), ~0x0 );

            // Sieve all the necessary primes we found below sqrt( limit ) from this segment
#define SIEVE_SIEVE_PRIMES( bitValue )                                                  \
            for ( Prime<bitValue> *prime : mPrimeStorage.GetStorage<bitValue>() )       \
            {                                                                           \
                prime->SieveSegment( mSegment.data(), segmentSize );                    \
            }

            SIEVE_SIEVE_PRIMES( 7 );
            SIEVE_SIEVE_PRIMES( 11 );
            SIEVE_SIEVE_PRIMES( 13 );
            SIEVE_SIEVE_PRIMES( 17 );
            SIEVE_SIEVE_PRIMES( 19 );
            SIEVE_SIEVE_PRIMES( 23 );
            SIEVE_SIEVE_PRIMES( 29 );
            SIEVE_SIEVE_PRIMES( 31 );

#undef SIEVE_SIEVE_PRIMES

            // For every block in the segment:
            //     If a bit position is still marked prime:
            //         - Output the corresponding number
            output.MaybeCall( [&]()
            {
                uint64_t qLimit = std::min<uint64_t>( mSegment.size(), limit - segmentStart );

                uint8_t *q = mSegment.data();
                uint8_t *end = q + qLimit;

                OutputRemainingPrimesInSegment( q, end, p, pLimit, limit, output );
            } );
        }
    }

private:

    std::vector<uint8_t> &mSegment;

    PrimeStorage mPrimeStorage;

    tFactory &mFactory;

    template< uint8_t tOffsetMod30, typename tOutput >
    void ProcessBlockPrime( const uint8_t &q, uint64_t p, uint64_t segmentStart, uint64_t segmentEnd, tOutput &output )
    {
        if ( q & GetBitMask<tOffsetMod30>() )
        {
            Prime<tOffsetMod30> *prime = mFactory.Init<tOffsetMod30>( p + tOffsetMod30, segmentStart );
            mPrimeStorage.GetStorage<tOffsetMod30>().push_back( prime );
            prime->SieveSegment( mSegment.data(), static_cast<uint32_t>( segmentEnd ) );
            output << p + tOffsetMod30;
        }
    }

    template< uint64_t tOffsetMod30, typename tOutput >
    void AppendIfPrime( const uint8_t &q, uint64_t p, uint64_t segmentStart, tOutput &output )
    {
        if ( q & GetBitMask<tOffsetMod30>() )
        {
            Prime<tOffsetMod30> *prime = mFactory.Init<tOffsetMod30>( p + tOffsetMod30, segmentStart );
            mPrimeStorage.GetStorage<tOffsetMod30>().push_back( prime );

            output << p + tOffsetMod30;
        }
    }

    template< uint64_t tOffsetMod30, typename tOutput >
    void OutputIfPrime( const uint8_t &q, uint64_t p, tOutput &output )
    {
        if ( q & GetBitMask<tOffsetMod30>() )
        {
            output << p + tOffsetMod30;
        }
    }

    template< uint64_t tOffsetMod30, typename tOutput >
    void OutputIfPrimeBelowLimit( const uint8_t &q, uint64_t p, uint64_t limit, tOutput &output )
    {
        if ( p + tOffsetMod30 < limit )
        {
            OutputIfPrime<tOffsetMod30>( q, p, output );
        }
    }

    template< typename tOutput >
    void OutputRemainingPrimesInSegment( uint8_t *&q, uint8_t *end, uint64_t &p, uint64_t pLimit, uint64_t limit,
                                         tOutput &output )
    {
        for ( ; q < end && p < pLimit; ++q, p += 30 )
        {
            SIEVE_ALL_BITVALUES( OutputIfPrime, *q, p, output );
        }

        for ( ; q < end && p < limit; ++q, p += 30 )
        {
            SIEVE_ALL_BITVALUES( OutputIfPrimeBelowLimit, *q, p, limit, output );
        }
    }
};

#endif
