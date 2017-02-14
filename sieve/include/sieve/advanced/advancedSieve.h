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

#include "sieve/advanced/memoryPool.h"
#include "sieve/helper.h"

#include <iostream>
#include <stdint.h>
#include <xutility>
#include <fstream>
#include <algorithm>

template< uint64_t tOffsetMod30 >
constexpr static uint8_t GetBitIndex()
{
    assert( false && "This value should not be tried" );
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

class IPrime
{
public:

    virtual ~IPrime() = default;

    virtual void SieveSegment( uint8_t *segment, uint32_t segmentSize ) = 0;
};

class ISmallPrime
    : public IPrime
{
public:

    virtual uint8_t GetPrimeMod30() const = 0;

    virtual ~ISmallPrime() override = default;
};

template< uint8_t tPrimeMod30 >
class SmallPrime
    : public ISmallPrime
{
public:

    SmallPrime( uint64_t prime, uint64_t segmentStart )
    {
        Init( prime, segmentStart );
    }

    SmallPrime()
    {}

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

    void SieveSegment( uint8_t *segment, uint32_t segmentSize ) override
    {

        //         if ( mNextMultipleIndex > segmentSize / 30 )
        //         {
        //             mNextMultipleIndex -= segmentSize / 30;
        //             return;
        //         }

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

            case 1:
                {
                    if ( !HandleCase<1, 7>( q, sieveEnd ) )
                    {
                        break;
                    }

                    q += tPrimeCrosser::template GetOffsetBetween<7, 11>( primeDiv30 );
                }

            case 2:
                {
                    if ( !HandleCase<2, 11>( q, sieveEnd ) )
                    {
                        break;
                    }

                    q += tPrimeCrosser::template GetOffsetBetween<11, 13>( primeDiv30 );
                }

            case 3:
                {
                    if ( !HandleCase<3, 13>( q, sieveEnd ) )
                    {
                        break;
                    }

                    q += tPrimeCrosser::template GetOffsetBetween<13, 17>( primeDiv30 );
                }

            case 4:
                {
                    if ( !HandleCase<4, 17>( q, sieveEnd ) )
                    {
                        break;
                    }

                    q += tPrimeCrosser::template GetOffsetBetween<17, 19>( primeDiv30 );
                }

            case 5:
                {
                    if ( !HandleCase<5, 19>( q, sieveEnd ) )
                    {
                        break;
                    }

                    q += tPrimeCrosser::template GetOffsetBetween<19, 23>( primeDiv30 );
                }

            case 6:
                {
                    if ( !HandleCase<6, 23>( q, sieveEnd ) )
                    {
                        break;
                    }

                    q += tPrimeCrosser::template GetOffsetBetween<23, 29>( primeDiv30 );
                }

            case 7:
                {
                    if ( !HandleCase<7, 29>( q, sieveEnd ) )
                    {
                        break;
                    }

                    q += tPrimeCrosser::template GetOffsetBetween<29, 31>( primeDiv30 );
                }
            }

            mNextMultipleIndex = static_cast<uint32_t>( q - sieveEnd );
        }
    }

    uint8_t GetPrimeMod30() const override
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

class SmallPrimeFactoryPooled
{
public:

    __forceinline ISmallPrime *Init( uint64_t prime, uint64_t segmentStart )
    {
        switch ( prime % 30 )
        {
        case 7:
            {
                SmallPrime<7> *p = mFactory7.Create();
                p->Init( prime, segmentStart );
                return p;
            }

        case 11:
            {
                SmallPrime<11> *p = mFactory11.Create();
                p->Init( prime, segmentStart );
                return p;
            }

        case 13:
            {
                SmallPrime<13> *p = mFactory13.Create();
                p->Init( prime, segmentStart );
                return p;
            }

        case 17:
            {
                SmallPrime<17> *p = mFactory17.Create();
                p->Init( prime, segmentStart );
                return p;
            }

        case 19:
            {
                SmallPrime<19> *p = mFactory19.Create();
                p->Init( prime, segmentStart );
                return p;
            }

        case 23:
            {
                SmallPrime<23> *p = mFactory23.Create();
                p->Init( prime, segmentStart );
                return p;
            }

        case 29:
            {
                SmallPrime<29> *p = mFactory29.Create();
                p->Init( prime, segmentStart );
                return p;
            }

        default:
            {
                SmallPrime<31> *p = mFactory31.Create();
                p->Init( prime, segmentStart );
                return p;
            }
        }
    }

    __forceinline void Release( ISmallPrime *p )
    {
        switch ( p->GetPrimeMod30() )
        {
        case 7:
            mFactory7.Destroy( reinterpret_cast< SmallPrime<7>  *>( p ) );
            break;

        case 11:
            mFactory11.Destroy( reinterpret_cast< SmallPrime<11> * >( p ) );
            break;

        case 13:
            mFactory13.Destroy( reinterpret_cast< SmallPrime<13> * >( p ) );
            break;

        case 17:
            mFactory17.Destroy( reinterpret_cast< SmallPrime<17> * >( p ) );
            break;

        case 19:
            mFactory19.Destroy( reinterpret_cast< SmallPrime<19> * >( p ) );
            break;

        case 23:
            mFactory23.Destroy( reinterpret_cast< SmallPrime<23> * >( p ) );
            break;

        case 29:
            mFactory29.Destroy( reinterpret_cast< SmallPrime<29> * >( p ) );
            break;

        case 31:
            mFactory31.Destroy( reinterpret_cast< SmallPrime<31> * >( p ) );
            break;
        }
    }

private:

    UnsychronisedMemoryPoolInstantiator< SmallPrime< 7 > > mFactory7;
    UnsychronisedMemoryPoolInstantiator< SmallPrime< 11 > > mFactory11;
    UnsychronisedMemoryPoolInstantiator< SmallPrime< 13 > > mFactory13;
    UnsychronisedMemoryPoolInstantiator< SmallPrime< 17 > > mFactory17;
    UnsychronisedMemoryPoolInstantiator< SmallPrime< 19 > > mFactory19;
    UnsychronisedMemoryPoolInstantiator< SmallPrime< 23 > > mFactory23;
    UnsychronisedMemoryPoolInstantiator< SmallPrime< 29 > > mFactory29;
    UnsychronisedMemoryPoolInstantiator< SmallPrime< 31 > > mFactory31;
};

class SmallPrimeFactory
{
public:

    static inline ISmallPrime *Init( uint32_t prime, uint64_t segmentStart )
    {
        switch ( prime % 30 )
        {
        case 7:
            return new SmallPrime<7>( prime, segmentStart );

        case 11:
            return new SmallPrime<11>( prime, segmentStart );

        case 13:
            return new SmallPrime<13>( prime, segmentStart );

        case 17:
            return new SmallPrime<17>( prime, segmentStart );

        case 19:
            return new SmallPrime<19>( prime, segmentStart );

        case 23:
            return new SmallPrime<23>( prime, segmentStart );

        case 29:
            return new SmallPrime<29>( prime, segmentStart );

        default:
            return new SmallPrime<31>( prime, segmentStart );

        }
    }

    static inline void Release( ISmallPrime *p )
    {
        delete p;
    }
};


template< typename tFactory >
class AdvancedSieve
{
public:

    explicit AdvancedSieve( tFactory &factory, std::vector<uint8_t> &segment )
        : mSegment( segment ),
          mFactory( factory )
    {
        std::fill( segment.begin(), segment.end(), static_cast<uint8_t>( ~0 ) );
    }

    ~AdvancedSieve()
    {
        for ( ISmallPrime *prime : mPrimes )
        {
            mFactory.Release( prime );
        }

        mPrimes.clear();
    }

    template< typename tOutput >
    void ExecuteSieve( uint64_t limit, tOutput &output )
    {
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
            for ( uint8_t *end = q + ( std::min<uint64_t>( Sqrt( segmentSize + 7 ), Sqrt( limit ) ) + 29 ) / 30; q < end; ++q, p += 30 )
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
            for ( ISmallPrime *prime : mPrimes )
            {
                prime->SieveSegment( mSegment.data(), segmentSize );
            }

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
    std::vector<ISmallPrime *> mPrimes;

    tFactory &mFactory;

    template< uint64_t tOffsetMod30, typename tOutput >
    void ProcessBlockPrime( const uint8_t &q, uint64_t p, uint64_t segmentStart, uint64_t segmentEnd, tOutput &output )
    {
        if ( q & GetBitMask<tOffsetMod30>() )
        {
            ISmallPrime *prime = mFactory.Init( p + tOffsetMod30, segmentStart );
            mPrimes.push_back( prime );
            prime->SieveSegment( mSegment.data(), static_cast<uint32_t>( segmentEnd ) );
            output << p + tOffsetMod30;
        }
    }

    template< uint64_t tOffsetMod30, typename tOutput >
    void AppendIfPrime( const uint8_t &q, uint64_t p, uint64_t segmentStart, tOutput &output )
    {
        if ( q & GetBitMask<tOffsetMod30>() )
        {
            ISmallPrime *prime = mFactory.Init( p + tOffsetMod30, segmentStart );
            mPrimes.push_back( prime );
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
    void OutputRemainingPrimesInSegment( uint8_t *&q, uint8_t *end, uint64_t &p, uint64_t pLimit, uint64_t limit, tOutput &output )
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
