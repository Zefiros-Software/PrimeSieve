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
#include "sieve/sieve.h"

#include "testHelper.h"

#include "gtest/gtest.h"

#include <fstream>
#include <chrono>

std::stringstream gSs;

template< typename tSieve >
void TestSieve()
{
    tSieve sieve;
    std::stringstream ss;
    StreamOutput output( ss );
    sieve.ExecuteSieve( 2000000, output );

    std::ofstream( "../../test/testPrimes.txt", std::ofstream::trunc | std::ofstream::binary ) << ss.str();

    ExpectEqual( ss.str(), gSs.str() );
}

template< typename tSieve >
void TestSieveAdvanced()
{
    typename tSieve::tFactoryType factory;
    std::vector<uint8_t> segment( 32 * 1024 );
    tSieve sieve( factory, segment );
    std::stringstream ss;
    StreamOutput output( ss );
    sieve.ExecuteSieve( 2000000, output );

    std::ofstream( "../../test/testPrimes.txt", std::ofstream::trunc | std::ofstream::binary ) << ss.str();

    ExpectEqual( ss.str(), gSs.str() );
}

#define SIEVE_TEST( tSieve, sieve ) \
TEST( P( Sieve ), sieve )           \
{                                   \
    TestSieve<tSieve>();            \
}

#define SIEVE_TEST_ADVANCED( tSieve, sieve )    \
TEST( P( Sieve ), sieve )                       \
{                                               \
    TestSieveAdvanced<tSieve>();                \
}

typedef VectorWrapper< uint8_t > Vector;

SIEVE_TEST( PrimeSieve< Vector >, CONCAT( PrimeSieve, Vector ) );
SIEVE_TEST( PrimeSieve2< Vector >, CONCAT( PrimeSieve2, Vector ) );
SIEVE_TEST( PrimeSieve3< Vector >, CONCAT( PrimeSieve3, Vector ) );
SIEVE_TEST( PrimeSieve4< Vector >, CONCAT( PrimeSieve4, Vector ) );
SIEVE_TEST( PrimeSieve5< Vector >, CONCAT( PrimeSieve5, Vector ) );
SIEVE_TEST( PrimeSieve< DenseBitArray >, CONCAT( PrimeSieve, DenseBitArray ) );
SIEVE_TEST( PrimeSieve2< DenseBitArray >, CONCAT( PrimeSieve2, DenseBitArray ) );
SIEVE_TEST( PrimeSieve3< DenseBitArray >, CONCAT( PrimeSieve3, DenseBitArray ) );
SIEVE_TEST( PrimeSieve4< DenseBitArray >, CONCAT( PrimeSieve4, DenseBitArray ) );
SIEVE_TEST( PrimeSieve5< DenseBitArray >, CONCAT( PrimeSieve5, DenseBitArray ) );

SIEVE_TEST_ADVANCED( AdvancedSieve<PrimeFactory>, CONCAT( AdvancedSieve, NewDelete ) );
SIEVE_TEST_ADVANCED( AdvancedSieve<PrimeFactoryPooled>, CONCAT( AdvancedSieve, Pooled ) );

int main( int argc, char **argv )
{

#ifdef _WIN32
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDERR );
    //_crtBreakAlloc =;
#endif

    std::ifstream stream( PRIMES_FILE );

    std::string s;

    while ( std::getline( stream, s ) )
    {
        gSs << s << "\n";
    }

    ::testing::InitGoogleTest( &argc, argv );

    int32_t result = ::testing::UnitTest::GetInstance()->Run();

    gSs.str( "" );
    gSs.clear();
    gSs.~basic_stringstream();

    return result;
}
