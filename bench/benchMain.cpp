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

template< typename tSieve, uint64_t tNumbers = 0 >
void BenchSieve( benchmark::State &state )
{
    const size_t nNumbers = tNumbers ? tNumbers : state.range( 0 );
    tSieve sieve;
    VoidOutput vo;

    while ( state.KeepRunning() )
    {
        TimeManual( state, [&]()
        {
            sieve.ExecuteSieve( nNumbers, vo );
        } );
    }

    state.SetItemsProcessed( state.iterations() * nNumbers );
}

template< typename tSieve, uint64_t tNumbers = 0 >
void BenchAdvancedSieve( benchmark::State &state )
{
    const size_t nNumbers = tNumbers ? tNumbers : state.range( 0 ) * 10ull;

    typename tSieve::tFactoryType factory;
    std::vector<uint8_t> segment( 32 * 1024 );

    tSieve sieve( factory, segment );
    VoidOutput vo;

    while ( state.KeepRunning() )
    {
        TimeManual( state, [&]()
        {
            sieve.ExecuteSieve( nNumbers, vo );
        } );
    }

    state.SetComplexityN( static_cast<int>( nNumbers ) );
    state.SetItemsProcessed( state.iterations() * nNumbers );
}

typedef VectorWrapper< uint8_t > Vector;

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve< Vector > )->Arg( 2000000 )->MinTime( 2.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve< Vector > )->Arg( 1000 )->MinTime( 2.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2< Vector > )->Arg( 2000000 )->MinTime( 2.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2< Vector > )->Arg( 1000 )->MinTime( 2.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3< Vector > )->Arg( 2000000 )->MinTime( 2.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3< Vector > )->Arg( 1000 )->MinTime( 2.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4< Vector > )->Arg( 2000000 )->MinTime( 2.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4< Vector > )->Arg( 1000 )->MinTime( 2.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5< Vector > )->Arg( 2000000 )->MinTime( 2.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5< Vector > )->Arg( 1000 )->MinTime( 2.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve< DenseBitArray > )->Arg( 2000000 )->MinTime( 2.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve< DenseBitArray > )->Arg( 1000 )->MinTime( 2.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2< DenseBitArray > )->Arg( 2000000 )->MinTime( 2.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2< DenseBitArray > )->Arg( 1000 )->MinTime( 2.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3< DenseBitArray > )->Arg( 2000000 )->MinTime( 2.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3< DenseBitArray > )->Arg( 1000 )->MinTime( 2.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4< DenseBitArray > )->Arg( 2000000 )->MinTime( 2.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4< DenseBitArray > )->Arg( 1000 )->MinTime( 2.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5< DenseBitArray > )->Arg( 2000000 )->MinTime( 2.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5< DenseBitArray > )->Arg( 1000 )->MinTime( 2.0 );

BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 2000000 )->MinTime( 2.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 1000 )->MinTime( 2.0 );

BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 2000000 )->MinTime( 2.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 1000 )->MinTime( 2.0 );

BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5< DenseBitArray >, 10000000000 )->MinTime( 2.0 );

BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 10000000000 )->MinTime( 10.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 10000000000 )->MinTime( 10.0 );
// BENCHMARK_TEMPLATE( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled> )->RangeMultiplier( 2 )
// ->Range( 2000000, 256000000 )->Complexity();

int main( int argc, char **argv )
{
    ::benchmark::Initialize( &argc, argv );

    if ( ::benchmark::ReportUnrecognizedArguments( argc, argv ) )
    {
        return 1;
    }

    ::benchmark::RunSpecifiedBenchmarks();
};