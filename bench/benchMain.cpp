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

#define BENCHMARK_HAS_CXX11
#include "benchmark/benchmark.h"

#include <iostream>
#include <chrono>

template< typename tFunc >
static inline void TimeManual( benchmark::State &state, const tFunc &func )
{
    auto start = std::chrono::high_resolution_clock::now();

    func();

    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>( end - start );

    state.SetIterationTime( elapsed_seconds.count() );
}

template< typename tSieve >
void BenchSieve( benchmark::State &state )
{
    const size_t nNumbers = state.range( 0 );
    tSieve sieve( nNumbers );

    while ( state.KeepRunning() )
    {
        sieve.Reset();

        TimeManual( state, [&]()
        {
            sieve.ExecuteSieve();
        } );
    }

    state.SetItemsProcessed( state.iterations() * nNumbers );
}

//BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve< Vector > )->Arg( 1999993 )->MinTime( 2.0 );
//BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve< Vector > )->Arg( 1000 )->MinTime( 2.0 );

//BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2< Vector > )->Arg( 1999993 )->MinTime( 2.0 );
//BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2< Vector > )->Arg( 1000 )->MinTime( 2.0 );

//BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3< Vector > )->Arg( 1999993 )->MinTime( 2.0 );
//BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3< Vector > )->Arg( 1000 )->MinTime( 2.0 );

//BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4< Vector  > )->Arg( 1999993 )->MinTime( 2.0 );
//BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4< Vector  > )->Arg( 1000 )->MinTime( 2.0 );

//BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve< BitArray > )->Arg( 1999993 )->MinTime( 2.0 );
//BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve< BitArray > )->Arg( 1000 )->MinTime( 2.0 );

//BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2< BitArray > )->Arg( 1999993 )->MinTime( 2.0 );
//BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2< BitArray > )->Arg( 1000 )->MinTime( 2.0 );

//BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3< BitArray > )->Arg( 1999993 )->MinTime( 2.0 );
//BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3< BitArray > )->Arg( 1000 )->MinTime( 2.0 );

//BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4< BitArray > )->Arg( 1999993 )->MinTime( 2.0 );
//BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4< BitArray > )->Arg( 1000 )->MinTime( 2.0 );

// BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<> )->Arg( 1999993 )->MinTime( 2.0 );
// BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve6<> )->Arg( 1999993 )->MinTime( 2.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve9< 10000000000 > )->Arg( 10000000000 )->MinTime( 2.0 );
//BENCHMARK_TEMPLATE( BenchNaiveSieve, PrimeSieve5 )->Arg( 1000 )->MinTime( 2.0 );

int main( int argc, char **argv )
{
    ::benchmark::Initialize( &argc, argv );

    if ( ::benchmark::ReportUnrecognizedArguments( argc, argv ) )
    {
        return 1;
    }

    //     Wheel<uint64_t, 3, 5, 7> wheel;
    //
    //     for ( uint32_t j = 0; j < 5; ++j )
    //     {
    //         uint64_t mask = wheel.GetMask( j );
    //
    //         for ( uint32_t i = 0; i < 64; ++i )
    //         {
    //             if ( ( ( mask ) >> i ) & 0x1 )
    //             {
    //                 std::cout << 1ul;
    //             }
    //             else
    //             {
    //                 std::cout << 0ul;
    //             }
    //
    //             if ( ( ( j * 64 + i + 1 ) % 105 ) == 0 )
    //             {
    //                 std::cout << std::endl;
    //             }
    //         }
    //
    //         std::cout << " ";
    //     }
    //
    //     for ( uint32_t j = 0; j < 5; ++j )
    //     {
    //         uint64_t mask = wheel.GetMask( j );
    //
    //         for ( uint32_t i = 0; i < 64; ++i )
    //         {
    //             if ( ( ( mask ) >> i ) & 0x1 )
    //             {
    //                 std::cout << ( ( ( i + 64 * j ) << 1 ) + 1 ) << std::endl;
    //             }
    //         }
    //     }

    ::benchmark::RunSpecifiedBenchmarks();
};