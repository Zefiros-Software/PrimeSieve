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
void BenchNaiveSieve( benchmark::State &state )
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

// BENCHMARK_TEMPLATE( BenchNaiveSieve, NaivePrimeSieve )->Arg( 999983 )->MinTime( 4.0 );
// BENCHMARK_TEMPLATE( BenchNaiveSieve, NaivePrimeSieve )->Arg( 1000 )->MinTime( 4.0 );
//
// BENCHMARK_TEMPLATE( BenchNaiveSieve, NaivePrimeSieve2 )->Arg( 999983 )->MinTime( 4.0 );
// BENCHMARK_TEMPLATE( BenchNaiveSieve, NaivePrimeSieve2 )->Arg( 1000 )->MinTime( 4.0 );
//
// BENCHMARK_TEMPLATE( BenchNaiveSieve, NaivePrimeSieve3 )->Arg( 999983 )->MinTime( 4.0 );
// BENCHMARK_TEMPLATE( BenchNaiveSieve, NaivePrimeSieve3 )->Arg( 1000 )->MinTime( 4.0 );
//
BENCHMARK_TEMPLATE( BenchNaiveSieve, NaivePrimeSieve4 )->Arg( 999983 )->MinTime( 4.0 );
// BENCHMARK_TEMPLATE( BenchNaiveSieve, NaivePrimeSieve4 )->Arg( 1000 )->MinTime( 4.0 );

int main( int argc, char **argv )
{
    ::benchmark::Initialize( &argc, argv );

    if ( ::benchmark::ReportUnrecognizedArguments( argc, argv ) )
    {
        return 1;
    }

    ::benchmark::RunSpecifiedBenchmarks();
};