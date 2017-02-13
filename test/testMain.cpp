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
#include "sieve/advancedSieve.h"
#include "sieve/sieve.h"

//#include "testHelper.h"

//#include "gtest/gtest.h"

#include <iostream>
#include <fstream>
#include <chrono>

std::stringstream gSs;

template< typename tSieve >
void TestSieve()
{
    tSieve sieve( 1999993 );
    sieve.ExecuteSieve();

    std::stringstream ss;

    for ( size_t p = 0; p <= 1999993; ++p )
    {
        if ( sieve.IsPrime( p ) )
        {
            ss << p << "\n";
        }
    }

    std::ofstream( "../../test/testPrimes.txt", std::ofstream::trunc | std::ofstream::binary ) << ss.str();
    //
    //     system( "pause" );

    //ExpectEqual( gSs.str(), ss.str() );
}

#define SIEVE_TEST( tSieve, sieve ) \
TEST( P( Sieve ), sieve )          \
{                                   \
    TestSieve<tSieve>();            \
}

// SIEVE_TEST( PrimeSieve< Vector >, CONCAT( PrimeSieve, Vector ) );
// SIEVE_TEST( PrimeSieve2< Vector >, CONCAT( PrimeSieve2, Vector ) );
// SIEVE_TEST( PrimeSieve3< Vector >, CONCAT( PrimeSieve3, Vector ) );
// SIEVE_TEST( PrimeSieve4< Vector >, CONCAT( PrimeSieve4, Vector ) );
//
// SIEVE_TEST( PrimeSieve< BitArray >, CONCAT( PrimeSieve, BitArray ) );
// SIEVE_TEST( PrimeSieve2< BitArray >, CONCAT( PrimeSieve2, BitArray ) );
// SIEVE_TEST( PrimeSieve3< BitArray >, CONCAT( PrimeSieve3, BitArray ) );
// SIEVE_TEST( PrimeSieve4< BitArray >, CONCAT( PrimeSieve4, BitArray ) );
//
// SIEVE_TEST( PrimeSieve5<>, PrimeSieve5 );
// SIEVE_TEST( PrimeSieve6<>, PrimeSieve6 );

class StreamOutput
{
public:

    explicit StreamOutput( std::ostream &stream )
        : mStream( stream )
    {

    }

    StreamOutput &operator <<( const uint64_t &o )
    {
        mStream << o << "\n";
        return *this;
    }

    template< typename tLambda >
    void MaybeCall( const tLambda &lambda )
    {
        lambda();
    }

private:

    std::ostream &mStream;
};

class VoidOutput
{
public:

    VoidOutput &operator <<( const uint64_t & )
    {
        return *this;
    }

    template< typename tLambda >
    void MaybeCall( const tLambda &/*lambda*/ )
    {

    }
};

// TEST( P( Sieve ), PrimeSieve9 )
// {
//     PrimeSieve9<> sieve( 1999993 );
//     std::stringstream ss;
//     StreamOutput sO( ss );
//     //sieve.ExecuteSieve( sO );
//
//
//     //     for ( size_t p = 0; p <= 1999993; ++p )
//     //     {
//     //         if ( sieve.IsPrime( p ) )
//     //         {
//     //             ss << p << "\n";
//     //         }
//     //     }
//
//     std::ofstream( "../../test/testPrimes.txt", std::ofstream::trunc | std::ofstream::binary ) << ss.str();
//     //
//     //     system( "pause" );
//
//     ExpectEqual( gSs.str(), ss.str() );
// };

int main( int argc, char **argv )
{

#ifdef _WIN32
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDERR );
    //_crtBreakAlloc = 742;
#endif

    //     std::ifstream stream( PRIMES_FILE );
    //
    //     std::string s;
    //
    //     while ( std::getline( stream, s ) )
    //     {
    //         gSs << s << "\n";
    //     }

    //     ::testing::InitGoogleTest( &argc, argv );
    //
    //     int32_t result = ::testing::UnitTest::GetInstance()->Run();
    //
    //    return result;

    SmallPrimeFactoryPooled factory;
    std::vector<uint8_t> segment( 32 * 1024 );

    {
        std::ofstream file( "../../test/testPrimes.txt", std::ofstream::trunc | std::ofstream::binary );
        StreamOutput output( file );

        AdvancedSieve<SmallPrimeFactoryPooled> sieve( factory, segment );
        sieve.ExecuteSieve( 2000000, output );
    }


    {
        auto start = std::chrono::high_resolution_clock::now();
        uint64_t i = 0;

        {
            VoidOutput vo;

            for ( ; i < 10; ++i )
            {
                AdvancedSieve<SmallPrimeFactoryPooled> sieve( factory, segment );
                sieve.ExecuteSieve( 10000000000, vo );
            }
        }

        auto end = std::chrono::high_resolution_clock::now();

        std::cout << ( std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count() / i ) << std::endl;
    }

    //     PrimeCrosser<31> pc( 31 );


    //     uint8_t test[500];
    //     pc.CrossOff<1>( test );
    //     pc.CrossOff<29>( test );

    return 0;
}
