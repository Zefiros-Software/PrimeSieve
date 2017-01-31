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

std::stringstream gSs;

template< typename tSieve >
void TestSieve()
{
    tSieve sieve( 999983 );
    sieve.ExecuteSieve();

    std::stringstream ss;

    for ( size_t p = 0; p <= 999983; ++p )
    {
        if ( sieve.IsPrime( p ) )
        {
            ss << p << "\n";
        }
    }

    //     std::ofstream( "../../test/testPrimes.txt", std::ofstream::trunc | std::ofstream::binary ) << ss.str();
    //
    //     system( "pause" );

    ExpectEqual( gSs.str(), ss.str() );
}

#define SIEVE_TEST( tSieve )                    \
TEST( P( Sieve ), tSieve )                      \
{                                               \
    TestSieve<tSieve>();                        \
}

SIEVE_TEST( NaivePrimeSieve );
SIEVE_TEST( NaivePrimeSieve2 );
SIEVE_TEST( NaivePrimeSieve3 );
SIEVE_TEST( NaivePrimeSieve4 );

int main( int argc, char **argv )
{

#ifdef _WIN32
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDERR );
    //_crtBreakAlloc = 742;
#endif

    std::ifstream stream( PRIMES_FILE );

    std::string s;

    while ( std::getline( stream, s ) )
    {
        gSs << s << "\n";
    }

    ::testing::InitGoogleTest( &argc, argv );

    int32_t result = ::testing::UnitTest::GetInstance()->Run();

    return result;
}