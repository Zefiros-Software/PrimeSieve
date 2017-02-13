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

#include "sieve/denseBitArray.h"
#include "sieve/helper.h"

#include <assert.h>
#include <algorithm>
#include "vectorWrapper.h"
#include <unordered_map>

template< typename tContainer >
class AbstractPrimeSieve
{
public:

    explicit AbstractPrimeSieve( size_t size )
        : mIsPrime( size + 1 ),
          mSize( size + 1 ),
          mProcessed( false )
    {
        assert( size > 1 );
    }

    virtual ~AbstractPrimeSieve()
    {}

    virtual void ExecuteSieve() = 0;

    virtual bool IsPrime( size_t q ) const = 0;

    void Reset()
    {
        mProcessed = false;
        mIsPrime.Reset();
    }

protected:

    tContainer mIsPrime;

    size_t mSize;
    bool mProcessed;

};

template< typename tContainer >
class PrimeSieve
    : public AbstractPrimeSieve<tContainer>
{
public:

    typedef AbstractPrimeSieve<tContainer> tParent;

    explicit PrimeSieve( size_t size )
        : tParent( size )
    {
    }

    void ExecuteSieve() override
    {
        tParent::mIsPrime.SetFalse( 0 );
        tParent::mIsPrime.SetFalse( 1 );

        for ( size_t p = 2; p < tParent::mSize; ++p )
        {
            if ( tParent::mIsPrime.Get( p ) )
            {
                SievePrime( p );
            }
        }

        tParent::mProcessed = true;
    }

    bool IsPrime( size_t q ) const override
    {
        assert( mProcessed && "You should first execute the sieve before querying" );

        return tParent::mIsPrime.Get( q );
    }

protected:

    void SievePrime( size_t p )
    {
        for ( size_t q = 2 * p; q < tParent::mSize; q += p )
        {
            tParent::mIsPrime.SetFalse( q );
        }
    }
};

template< typename tContainer >
class PrimeSieve2
    : public PrimeSieve< tContainer >
{
public:

    typedef PrimeSieve<tContainer> tParent;

    explicit PrimeSieve2( size_t size )
        : tParent( size )
    {
    }

    void ExecuteSieve() override
    {
        tParent::mIsPrime.SetFalse( 0 );
        tParent::mIsPrime.SetFalse( 1 );

        // Sieve the only even prime number
        tParent::SievePrime( 2 );

        // Only check whether odd numbers are prime
        for ( size_t p = 3, sqrtP = CeilSqrt( tParent::mSize ); p < sqrtP; p += 2 )
        {
            if ( tParent::mIsPrime.Get( p ) )
            {
                tParent::SievePrime( p );
            }
        }

        tParent::mProcessed = true;
    }
};

template< typename tContainer >
class PrimeSieve3
    : public AbstractPrimeSieve<tContainer>
{
public:

    typedef AbstractPrimeSieve<tContainer> tParent;

    explicit PrimeSieve3( size_t size )
        : tParent( size )
    {
    }

    void ExecuteSieve() override
    {
        tParent::mIsPrime.SetFalse( 1 );

        // Only check whether odd numbers are prime
        for ( size_t p = 3, sqrtP = CeilSqrt( tParent::mSize ); p < sqrtP; p += 2 )
        {
            if ( tParent::mIsPrime.Get( p ) )
            {
                SievePrime( p );
            }
        }

        tParent::mProcessed = true;
    }

    bool IsPrime( size_t q ) const override
    {
        assert( mProcessed && "You should first execute the sieve before querying" );

        return ( ( q & 0x1 ) && tParent::mIsPrime.Get( q ) ) || q == 2;
    }

protected:

    void SievePrime( size_t p )
    {
        for ( size_t q = p * p, p2 = 2 * p; q < tParent::mSize; q += p2 )
        {
            tParent::mIsPrime.SetFalse( q );
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

    explicit PrimeSieve4( size_t size )
        : mIsPrime( ( size + 1 ) >> 1 ),
          mSize( size + 1 ),
          mPosSize( NumToPos( size + 1 ) ),
          mSqrtSize( NumToPos( CeilSqrt( size + 1 ) ) ),
          mProcessed( false )
    {
    }

    void ExecuteSieve()
    {
        // 1 is not a prime number
        mIsPrime.SetFalse( 0 );

        // Only check whether odd numbers are prime
        for ( size_t pStar = NumToPos( 3 ), sqrtPStar = mSqrtSize; pStar < sqrtPStar; ++pStar )
        {
            if ( mIsPrime.Get( pStar ) )
            {
                SievePrime( pStar );
            }
        }

        mProcessed = true;
    }

    bool IsPrime( size_t q ) const
    {
        assert( mProcessed && "You should first execute the sieve before querying" );

        return ( ( q & 0x1 ) && mIsPrime.Get( NumToPos( q ) ) ) || q == 2;
    }

    void Reset()
    {
        mProcessed = false;
        mIsPrime.Reset();
    }

protected:

    tContainer mIsPrime;

    size_t mSize;
    size_t mPosSize;
    size_t mSqrtSize;
    bool mProcessed;

    void SievePrime( size_t pStar )
    {
        for ( size_t p = PosToNum( pStar ), q = NumToPos( p * p ); q < mPosSize; q += p )
        {
            mIsPrime.SetFalse( q );
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

// 1 --> 0
// 3 --> 1
// 5 --> 2
// ...
template< size_t tSize = 0 >
class PrimeSieve5
{
public:

    explicit PrimeSieve5( size_t size )
        : mIsPrime( ( std::max( size, tSize ) + 1 ) >> 1 ),
          mSize( std::max( size, tSize ) + 1 ),
          mPosSize( NumToPos( std::max( size, tSize ) + 1 ) ),
          mSqrtSize( NumToPos( CeilSqrt( std::max( size, tSize ) + 1 ) ) ),
          mProcessed( false )
    {
    }

    virtual ~PrimeSieve5()
    {}

    void ExecuteSieve()
    {
        // 1 is not a prime number
        mIsPrime.SetFalse( 0 );

        RollWheel();

        // Start at 9, since we assumed 3, 5, 7 are prime
        for ( size_t pStar = GetStartPrime(); pStar < mSqrtSize; ++pStar )
        {
            if ( mIsPrime.Get( pStar ) )
            {
                SievePrime( pStar );
            }
        }

        mProcessed = true;
    }

    bool IsPrime( size_t q ) const
    {
        assert( mProcessed && "You should first execute the sieve before querying" );

        return ( ( q & 0x1 ) && mIsPrime.Get( NumToPos( q ) ) ) || q == 2;
    }

    bool FastIsPrime( size_t q ) const
    {
        return mIsPrime.Get( NumToPos( q ) );
    }

    bool PosIsPrime( size_t pStar ) const
    {
        return mIsPrime.Get( pStar );
    }

    void Reset()
    {
        mProcessed = false;
        mIsPrime.Reset();
    }

protected:

    DenseBitArray<uint64_t> mIsPrime;
    Wheel<uint64_t, 3, 5, 7> mWheel;

    size_t mSize;
    size_t mPosSize;
    size_t mSqrtSize;
    bool mProcessed;

    void SievePrime( size_t pStar )
    {
        for ( size_t p = PosToNum( pStar ), q = NumToPos( p * p ); q < mPosSize; q += p )
        {
            mIsPrime.SetFalse( q );
        }
    }

    void Reset357()
    {
        // Reset 3, 5 and 7 that have been crossed out
        mIsPrime.GetRaw()[0] |= ( 0x1 << NumToPos( 3 ) ) | ( 0x1 << NumToPos( 5 ) ) | ( 0x1 << NumToPos( 7 ) );
    }

    virtual void RollWheel()
    {
        size_t i = 0;

        for ( uint64_t &block : mIsPrime.GetRaw() )
        {
            block &= ~mWheel.GetMask( i++ );
        }

        Reset357();
    }

    virtual size_t GetStartPrime() const
    {
        return NumToPos( 9 );
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

template< size_t tSize = 0, size_t tMaxDensePrime = 16 >
class PrimeSieve6
    : public PrimeSieve5<tSize>
{
public:

    typedef PrimeSieve5<tSize> tParent;

    explicit PrimeSieve6( size_t size )
        : tParent( std::max( size,  tSize ) )
    {
    }

    //     const std::vector<uint64_t> &GetBlocks() const
    //     {
    //         return tParent::mIsPrime.GetRaw();
    //     }

    //*
    template< typename tOutput >
    void OutputPrimes( tOutput &output )
    {

        output << 2;

        for ( uint64_t q = 3; q < tParent::mSize; q += 2 )
        {
            if ( tParent::IsPrime( q ) )
            {
                output << q;
            }
        }
    }

private:

    void RollWheel() override
    {
        size_t i = 0;

        uint64_t wheel = tParent::mWheel.GetMask( 0 );

        std::vector< std::pair< size_t, size_t > > lowPrimes;
        DenseBitArray<uint64_t> wheelWrapper( 64 );
        wheelWrapper.GetRaw()[0] = wheel;

        for ( size_t j = tParent::NumToPos( 11 ); j < tParent::NumToPos( tMaxDensePrime ); ++j )
        {
            if ( !wheelWrapper.Get( j ) )
            {
                size_t p = tParent::PosToNum( j );
                lowPrimes.emplace_back( std::make_pair( p, tParent::NumToPos( p * p ) ) );
            }
        }

        for ( uint64_t &block : tParent::mIsPrime.GetRaw() )
        {
            for ( auto &pp : lowPrimes )
            {
                size_t pos = pp.second;

                for ( size_t pStar = pp.first; pos < 64; pos += pStar )
                {
                    wheel |= 1ull << pos;
                }

                pp.second = pos - 64;
            }

            block &= ~wheel;
            wheel = tParent::mWheel.GetMask( ++i );
        }

        tParent::Reset357();
    }

    size_t GetStartPrime() const override
    {
        return tParent::NumToPos( tMaxDensePrime + 1 );
    }
};

template< size_t tSize = 0 >
class PrimeSieve7
{
public:

    explicit PrimeSieve7( size_t size )
        : mSegment( 32 * 1024 * 4 ),
          mInitialSieve( ( CeilSqrt( std::max( size, tSize ) ) + 127 ) / 128 * 128 ),
          mInitialPosSize( NumToPos( ( CeilSqrt( std::max( size, tSize ) ) + 127 ) / 128 * 128 ) ),
          mSize( std::max( size, tSize ) + 1 ),
          mPosSize( NumToPos( std::max( size, tSize ) + 1 ) )
    {

    }

    /*
    template< typename tOutput >
    void ExecuteSieve( tOutput &output )
    /*/
    void ExecuteSieve()
    /**/
    {
        mInitialSieve.ExecuteSieve();

        for ( size_t p = 9; p < 16; p += 2 )
        {
            if ( mInitialSieve.IsPrime( p ) )
            {
                mLowPrimes.push_back( p );
            }
        }

        mLowPrimePositions.resize( mLowPrimes.size() );

        {
            //output << 2;
            //output << 3;
            //output << 5;
            //output << 7;
            //output << 11;
            //output << 13;

            size_t p = 17;

            for ( size_t end = PosToNum( mInitialPosSize );  p < end; p += 2 )
            {
                if ( mInitialSieve.FastIsPrime( p ) )
                {
                    //output << p;
                    mInitialPrimes.push_back( p );
                    mInitialPrimeSquaresPos.push_back( NumToPos( p * p ) );
                }
            }
        }

        for ( size_t pos = mInitialPosSize; pos < mPosSize; pos += mSegment.Size() )
        {
            mSegment.Reset();
            SieveSegment( mSegment, pos );
            //OutputSegmentPrimes( output, mSegment, pos );
        }
    }

    void Reset()
    {
        mInitialSieve.Reset();
    }

private:

    DenseBitArray<uint64_t> mSegment;
    PrimeSieve6<> mInitialSieve;
    Wheel< uint64_t, 3, 5, 7 > mWheel;

    std::vector<uint64_t> mLowPrimes;
    std::vector<uint64_t> mLowPrimePositions;
    std::vector<uint64_t> mInitialPrimes;
    std::vector<uint64_t> mInitialPrimeSquaresPos;

    size_t mInitialPosSize;
    size_t mSize;
    size_t mPosSize;

    //  void SieveSegment(DenseBitArray<uint64_t> &segment, size_t segmentStart)
    //  {
    //      RollWheel(segment, segmentStart);
    //
    //      size_t segmentEnd = segmentStart + segment.Size();
    //
    //      size_t i = 0;
    //
    //      for (auto &pSquarePos : mInitialPrimeSquaresPos)
    //      {
    //          if (pSquarePos > segmentEnd)
    //          {
    //              break;
    //          }
    //
    //          SieveSegmentPrime(segment, segmentStart, mInitialPrimes[i++], pSquarePos);
    //      }
    //  }

    void SieveSegment( DenseBitArray<uint64_t> &segment, size_t segmentStart )
    {
        RollWheel( segment, segmentStart );

        size_t segmentEnd = segmentStart + segment.Size();

        size_t i = 0;

        for ( size_t end = mInitialPrimes.size() / 4 * 4; i < end; i += 4 )
        {
            if ( mInitialPrimeSquaresPos[i + 1] > segmentEnd )
            {
                break;
            }

            SieveSegmentPrime4( segment, segmentStart, mInitialPrimes[i], mInitialPrimes[i + 1], mInitialPrimes[i + 2],
                                mInitialPrimes[i + 3], mInitialPrimeSquaresPos[i], mInitialPrimeSquaresPos[i + 1], mInitialPrimeSquaresPos[i + 2],
                                mInitialPrimeSquaresPos[i + 3] );
        }

        for ( size_t end = mInitialPrimes.size(); i < end; ++i )
        {
            if ( mInitialPrimeSquaresPos[i] > segmentEnd )
            {
                break;
            }

            SieveSegmentPrime( segment, segmentStart, mInitialPrimes[i], mInitialPrimeSquaresPos[i] );
        }
    }

    template< typename tOutput >
    void OutputSegmentPrimes( tOutput &output, DenseBitArray<uint64_t> &segment, size_t segmentStart )
    {
        size_t q = PosToNum( segmentStart );

        for ( size_t pStar = 0; pStar < segment.Size() && pStar + segmentStart < mPosSize; ++pStar, q += 2 )
        {
            if ( segment.Get( pStar ) )
            {
                output << q;
            }
        }
    }

    void SieveSegmentPrime4( DenseBitArray<uint64_t> &segment, size_t segmentStart, size_t p1, size_t p2, size_t p3,
                             size_t p4, size_t pSquarePos1, size_t pSquarePos2, size_t pSquarePos3, size_t pSquarePos4 )
    {
        size_t segmentStartNum = PosToNum( segmentStart );

        size_t q1 = FirstPosInSegment( p1, segmentStartNum, pSquarePos1 ) - segmentStart;
        size_t q2 = FirstPosInSegment( p2, segmentStartNum, pSquarePos2 ) - segmentStart;
        size_t q3 = FirstPosInSegment( p3, segmentStartNum, pSquarePos3 ) - segmentStart;
        size_t q4 = FirstPosInSegment( p4, segmentStartNum, pSquarePos4 ) - segmentStart;
        size_t end = segment.Size();

        for ( size_t end4 = std::min( std::max( {q1, q2, q3} ), end ); q4 < end4; q4 += p4 )
        {
            segment.SetFalse( q4 );
        }

        for ( ; q4 < end; q1 += p1, q2 += p2, q3 += p3, q4 += p4 )
        {
            segment.SetFalse( q1 );
            segment.SetFalse( q2 );
            segment.SetFalse( q3 );
            segment.SetFalse( q4 );
        }

        for ( size_t end3 = std::min( std::max( { q1, q2 } ), end ); q3 < end3; q3 += p3 )
        {
            segment.SetFalse( q3 );
        }

        for ( ; q3 < end; q1 += p1, q2 += p2, q3 += p3 )
        {
            segment.SetFalse( q1 );
            segment.SetFalse( q2 );
            segment.SetFalse( q3 );
        }

        for ( size_t end2 = std::min( q1, end ); q2 < end2; q2 += p2 )
        {
            segment.SetFalse( q2 );
        }

        for ( ; q2 < end; q1 += p1, q2 += p2 )
        {
            segment.SetFalse( q1 );
            segment.SetFalse( q2 );
        }

        for ( ; q1 < end; q1 += p1 )
        {
            segment.SetFalse( q1 );
        }
    }

    void SieveSegmentPrime( DenseBitArray<uint64_t> &segment, size_t segmentStart, size_t p, size_t pSquarePos )
    {
        size_t segmentStartNum = PosToNum( segmentStart );

        for ( size_t q = FirstPosInSegment( p, segmentStartNum, pSquarePos ) - segmentStart, end = segment.Size(); q < end;
                q += p )
        {
            segment.SetFalse( q );
        }
    }

    size_t FirstPosInSegment( size_t p, size_t segmentStartNum, size_t pSquarePos ) const
    {
        return std::max( NumToPos( ( ( ( segmentStartNum + p - 1 ) / p ) | 0x1 ) * p ), pSquarePos );
    }

    void RollWheel( DenseBitArray< uint64_t > &segment, size_t segmentStart )
    {
        size_t i = segmentStart / 64;

        {
            size_t j = 0;

            for ( auto &p : mLowPrimes )
            {
                mLowPrimePositions[j++] = FirstPosInSegment( p, PosToNum( segmentStart ), NumToPos( p * p ) ) - segmentStart;
            }
        }

        for ( uint64_t &block : segment.GetRaw() )
        {
            uint64_t wheel = mWheel.GetMask( i++ );

            size_t j = 0;

            for ( auto &p : mLowPrimes )
            {
                uint64_t &pos = mLowPrimePositions[j++];

                for ( ; pos < 64; pos += p )
                {
                    wheel |= 1ull << pos;
                }

                pos -= 64;
            }

            block &= ~wheel;
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

template< size_t tSize = 0 >
class PrimeSieve8
{
public:

    explicit PrimeSieve8( size_t size )
        : mSegment( 32 * 1024 * 8 ),
          mInitialSieve( ( CeilSqrt( std::max( size, tSize ) ) + 127 ) / 128 * 128 ),
          mInitialPosSize( NumToPos( ( CeilSqrt( std::max( size, tSize ) ) + 127 ) / 128 * 128 ) ),
          mSize( std::max( size, tSize ) + 1 ),
          mPosSize( NumToPos( std::max( size, tSize ) + 1 ) )
    {

    }

    /*
    template< typename tOutput >
    void ExecuteSieve( tOutput &output )
    /*/
    void ExecuteSieve()
    /**/
    {
        mInitialSieve.ExecuteSieve();

        for ( size_t p = 9; p < 16; p += 2 )
        {
            if ( mInitialSieve.IsPrime( p ) )
            {
                mLowPrimes.push_back( p );
            }
        }

        mLowPrimePositions.resize( mLowPrimes.size() );

        {
            //output << 2;
            //output << 3;
            //output << 5;
            //output << 7;
            //output << 11;
            //output << 13;

            size_t p = 17;

            for ( size_t end = PosToNum( mInitialPosSize );  p < end; p += 2 )
            {
                if ( mInitialSieve.FastIsPrime( p ) )
                {
                    //output << p;
                    mInitialPrimes.push_back( p );
                    mInitialPrimeSquaresPos.push_back( NumToPos( p * p ) );
                }
            }
        }

        for ( size_t pos = mInitialPosSize; pos < mPosSize; pos += mSegment.Size() )
        {
            mSegment.Reset();
            SieveSegment( mSegment, pos );
            //OutputSegmentPrimes( output, mSegment, pos );
        }
    }

    void Reset()
    {
        mInitialSieve.Reset();
    }

private:

    VectorWrapper<uint8_t> mSegment;
    PrimeSieve6<> mInitialSieve;
    Wheel< uint64_t, 3, 5, 7 > mWheel;

    std::vector<uint64_t> mLowPrimes;
    std::vector<uint64_t> mLowPrimePositions;
    std::vector<uint64_t> mInitialPrimes;
    std::vector<uint64_t> mInitialPrimeSquaresPos;

    size_t mInitialPosSize;
    size_t mSize;
    size_t mPosSize;

    //  void SieveSegment(DenseBitArray<uint64_t> &segment, size_t segmentStart)
    //  {
    //      RollWheel(segment, segmentStart);
    //
    //      size_t segmentEnd = segmentStart + segment.Size();
    //
    //      size_t i = 0;
    //
    //      for (auto &pSquarePos : mInitialPrimeSquaresPos)
    //      {
    //          if (pSquarePos > segmentEnd)
    //          {
    //              break;
    //          }
    //
    //          SieveSegmentPrime(segment, segmentStart, mInitialPrimes[i++], pSquarePos);
    //      }
    //  }

    void SieveSegment( VectorWrapper<uint8_t> &segment, size_t segmentStart )
    {
        size_t segmentEnd = segmentStart + segment.Size();

        size_t i = 0;

        for ( size_t end = mInitialPrimes.size() / 4 * 4; i < end; i += 4 )
        {
            if ( mInitialPrimeSquaresPos[i + 1] > segmentEnd )
            {
                break;
            }

            SieveSegmentPrime4( segment, segmentStart, mInitialPrimes[i], mInitialPrimes[i + 1], mInitialPrimes[i + 2],
                                mInitialPrimes[i + 3], mInitialPrimeSquaresPos[i], mInitialPrimeSquaresPos[i + 1], mInitialPrimeSquaresPos[i + 2],
                                mInitialPrimeSquaresPos[i + 3] );
        }

        for ( size_t end = mInitialPrimes.size(); i < end; ++i )
        {
            if ( mInitialPrimeSquaresPos[i] > segmentEnd )
            {
                break;
            }

            SieveSegmentPrime( segment, segmentStart, mInitialPrimes[i], mInitialPrimeSquaresPos[i] );
        }
    }

    template< typename tOutput >
    void OutputSegmentPrimes( tOutput &output, VectorWrapper<uint8_t> &segment, size_t segmentStart )
    {
        size_t q = PosToNum( segmentStart );

        for ( size_t pStar = 0; pStar < segment.Size() && pStar + segmentStart < mPosSize; ++pStar, q += 2 )
        {
            if ( segment.Get( pStar ) )
            {
                output << q;
            }
        }
    }

    void SieveSegmentPrime4( VectorWrapper<uint8_t> &segment, size_t segmentStart, size_t p1, size_t p2, size_t p3,
                             size_t p4, size_t pSquarePos1, size_t pSquarePos2, size_t pSquarePos3, size_t pSquarePos4 )
    {
        size_t segmentStartNum = PosToNum( segmentStart );

        size_t q1 = FirstPosInSegment( p1, segmentStartNum, pSquarePos1 ) - segmentStart;
        size_t q2 = FirstPosInSegment( p2, segmentStartNum, pSquarePos2 ) - segmentStart;
        size_t q3 = FirstPosInSegment( p3, segmentStartNum, pSquarePos3 ) - segmentStart;
        size_t q4 = FirstPosInSegment( p4, segmentStartNum, pSquarePos4 ) - segmentStart;
        size_t end = segment.Size();

        for ( size_t end4 = std::min( std::max( {q1, q2, q3} ), end ); q4 < end4; q4 += p4 )
        {
            segment.SetFalse( q4 );
        }

        for ( ; q4 < end; q1 += p1, q2 += p2, q3 += p3, q4 += p4 )
        {
            segment.SetFalse( q1 );
            segment.SetFalse( q2 );
            segment.SetFalse( q3 );
            segment.SetFalse( q4 );
        }

        for ( size_t end3 = std::min( std::max( { q1, q2 } ), end ); q3 < end3; q3 += p3 )
        {
            segment.SetFalse( q3 );
        }

        for ( ; q3 < end; q1 += p1, q2 += p2, q3 += p3 )
        {
            segment.SetFalse( q1 );
            segment.SetFalse( q2 );
            segment.SetFalse( q3 );
        }

        for ( size_t end2 = std::min( q1, end ); q2 < end2; q2 += p2 )
        {
            segment.SetFalse( q2 );
        }

        for ( ; q2 < end; q1 += p1, q2 += p2 )
        {
            segment.SetFalse( q1 );
            segment.SetFalse( q2 );
        }

        for ( ; q1 < end; q1 += p1 )
        {
            segment.SetFalse( q1 );
        }
    }

    void SieveSegmentPrime( VectorWrapper<uint8_t> &segment, size_t segmentStart, size_t p, size_t pSquarePos )
    {
        size_t segmentStartNum = PosToNum( segmentStart );

        for ( size_t q = FirstPosInSegment( p, segmentStartNum, pSquarePos ) - segmentStart, end = segment.Size(); q < end;
                q += p )
        {
            segment.SetFalse( q );
        }
    }

    size_t FirstPosInSegment( size_t p, size_t segmentStartNum, size_t pSquarePos ) const
    {
        return std::max( NumToPos( ( ( ( segmentStartNum + p - 1 ) / p ) | 0x1 ) * p ), pSquarePos );
    }

    void RollWheel( DenseBitArray< uint64_t > &segment, size_t segmentStart )
    {
        size_t i = segmentStart / 64;

        {
            size_t j = 0;

            for ( auto &p : mLowPrimes )
            {
                mLowPrimePositions[j++] = FirstPosInSegment( p, PosToNum( segmentStart ), NumToPos( p * p ) ) - segmentStart;
            }
        }

        for ( uint64_t &block : segment.GetRaw() )
        {
            uint64_t wheel = mWheel.GetMask( i++ );

            size_t j = 0;

            for ( auto &p : mLowPrimes )
            {
                uint64_t &pos = mLowPrimePositions[j++];

                for ( ; pos < 64; pos += p )
                {
                    wheel |= 1ull << pos;
                }

                pos -= 64;
            }

            block &= ~wheel;
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

template< size_t tSize = 0 >
class PrimeSieve9
{
public:

    explicit PrimeSieve9( size_t size )
        : mSegment( 32 * 1024 * 6 ),
          mInitialSieve( ( CeilSqrt( std::max( size, tSize ) ) + 127 ) / 128 * 128 ),
          mInitialPosSize( NumToPos( ( CeilSqrt( std::max( size, tSize ) ) + 127 ) / 128 * 128 ) ),
          mSize( std::max( size, tSize ) + 1 ),
          mPosSize( NumToPos( std::max( size, tSize ) + 1 ) )
    {

    }

    /*
    template< typename tOutput >
    void ExecuteSieve( tOutput &output )
    /*/
    void ExecuteSieve()
    /**/
    {
        mInitialSieve.ExecuteSieve();

        {
            uint8_t step = 1;

            uint64_t wheel = mWheel.GetMask( 0 ) >> 1;

            std::vector<uint8_t> posToWheel;

            for ( uint8_t i = 0; i < 63; ++i, ++step, wheel >>= 1 )
            {
                posToWheel.push_back( static_cast<uint8_t>( mWheelSkippingDistances.size() ) );

                if ( !( wheel & 0x1 ) )
                {
                    mWheelSkippingDistances.push_back( step );
                    step = 0;
                }
            }

            wheel = mWheel.GetMask( 1 );


            for ( uint8_t i = 0; i < ( 105 - 64 ); ++i, ++step, wheel >>= 1 )
            {
                posToWheel.push_back( static_cast<uint8_t>( mWheelSkippingDistances.size() ) );

                if ( !( wheel & 0x1 ) )
                {
                    mWheelSkippingDistances.push_back( step );
                    step = 0;
                }
            }

            posToWheel.push_back( static_cast<uint8_t>( mWheelSkippingDistances.size() ) );
            mWheelSkippingDistances.push_back( step );

            //output << 2;
            //output << 3;
            //output << 5;
            //output << 7;

            size_t p = 11;

            for ( size_t end = PosToNum( mInitialPosSize );  p < end; p += 2 )
            {
                if ( mInitialSieve.FastIsPrime( p ) )
                {
                    //output << p;
                    mInitialPrimes.push_back( p );
                    size_t q = FirstPosInSegment( p, PosToNum( mInitialPosSize ), p * p );
                    mInitialPrimeSquaresPos.push_back( q - mInitialPosSize );
                    mWheelIndices.push_back( posToWheel[NumToPos( ( PosToNum( q ) / p ) % 210 )] );
                }
            }
        }

        for ( size_t pos = mInitialPosSize; pos < mPosSize; pos += mSegment.Size() )
        {
            mSegment.Reset();
            SieveSegment( mSegment, pos );
            //OutputSegmentPrimes( output, mSegment, pos );
        }
    }

    void Reset()
    {
        mInitialSieve.Reset();
    }

private:

    DenseBitArray<uint64_t> mSegment;
    PrimeSieve6<> mInitialSieve;
    Wheel< uint64_t, 3, 5, 7 > mWheel;

    std::vector<uint64_t> mInitialPrimes;
    std::vector<uint64_t> mInitialPrimeSquaresPos;
    std::vector<uint8_t> mWheelSkippingDistances;
    std::vector<uint8_t> mWheelIndices;

    size_t mInitialPosSize;
    size_t mSize;
    size_t mPosSize;

    size_t FirstPosInSegment( size_t p, size_t segmentStartNum, size_t pSquare ) const
    {
        size_t q = std::max( ( ( ( segmentStartNum + p - 1 ) / p ) | 0x1 ) * p, pSquare );

        while ( q / 3 * 3 == q || q / 5 * 5 == q || q / 7 * 7 == q )
        {
            q += 2 * p;
        }

        return NumToPos( q );
    }

    void SieveSegment( DenseBitArray<uint64_t> &segment, size_t segmentStart )
    {
        RollWheel( segment, segmentStart );

        size_t i = 0;
        size_t end = mInitialPrimes.size();

        for ( size_t end1 = end - 1; i < end1; i += 2 )
        {
            SieveSegmentPrime2( segment, i );
        }

        for ( ; i < end; ++i )
        {
            SieveSegmentPrime( segment, i );
        }
    }

    void SieveSegmentPrime( DenseBitArray<uint64_t> &segment, size_t i )
    {
        size_t q = mInitialPrimeSquaresPos[i];
        size_t p = mInitialPrimes[i];
        uint8_t w = mWheelIndices[i];
        size_t end = segment.Size();

        for ( ; q < end; )
        {
            for ( ; q < end && w < 48; q += mWheelSkippingDistances[w] * p, ++w )
            {
                segment.SetFalse( q );
            }

            if ( w == 48 )
            {
                w = 0;
            }
        }

        mInitialPrimeSquaresPos[i] = q - end;
        mWheelIndices[i] = w;
    }

    void SieveSegmentPrime2( DenseBitArray<uint64_t> &segment, size_t i )
    {
        size_t q1 = mInitialPrimeSquaresPos[i];
        size_t q2 = mInitialPrimeSquaresPos[i + 1];
        size_t p1 = mInitialPrimes[i];
        size_t p2 = mInitialPrimes[i + 1];
        uint8_t w1 = mWheelIndices[i];
        uint8_t w2 = mWheelIndices[i + 1];
        size_t end = segment.Size();

        for ( ; q1 < end && q2 < end; )
        {
            for ( ; q1 < end && q2 < end && w1 < 48 &&
                    w2 < 48; q1 += mWheelSkippingDistances[w1] * p1, ++w1, q2 += mWheelSkippingDistances[w2] * p2, ++w2 )
            {
                segment.SetFalse( q1 );
                segment.SetFalse( q2 );
            }

            if ( w1 == 48 )
            {
                w1 = 0;
            }

            if ( w2 == 48 )
            {
                w2 = 0;
            }
        }

        for ( ; q1 < end; )
        {
            for ( ; q1 < end && w1 < 48; q1 += mWheelSkippingDistances[w1] * p1, ++w1 )
            {
                segment.SetFalse( q1 );
            }

            if ( w1 == 48 )
            {
                w1 = 0;
            }
        }

        for ( ; q2 < end; )
        {
            for ( ; q2 < end && w2 < 48; q2 += mWheelSkippingDistances[w2] * p2, ++w2 )
            {
                segment.SetFalse( q2 );
            }

            if ( w2 == 48 )
            {
                w2 = 0;
            }
        }

        mInitialPrimeSquaresPos[i] = q1 - end;
        mInitialPrimeSquaresPos[i + 1] = q2 - end;
        mWheelIndices[i] = w1;
        mWheelIndices[i + 1] = w2;
    }

    void RollWheel( DenseBitArray< uint64_t > &segment, size_t segmentStart )
    {
        size_t i = segmentStart / 64;


        for ( uint64_t &block : segment.GetRaw() )
        {
            uint64_t wheel = mWheel.GetMask( i++ );
            block &= ~wheel;
        }
    }

    template< typename tOutput >
    void OutputSegmentPrimes( tOutput &output, DenseBitArray<uint64_t> &segment, size_t segmentStart )
    {
        size_t q = PosToNum( segmentStart );

        for ( size_t pStar = 0; pStar < segment.Size() && pStar + segmentStart < mPosSize; ++pStar, q += 2 )
        {
            if ( segment.Get( pStar ) )
            {
                output << q;
            }
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