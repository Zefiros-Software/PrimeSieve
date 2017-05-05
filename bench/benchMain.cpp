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

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<Vector> )->Arg( 2000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<Vector> )->Arg( 1500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<Vector> )->Arg( 1000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<Vector> )->Arg( 750000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<Vector> )->Arg( 500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<Vector> )->Arg( 400000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<Vector> )->Arg( 300000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<Vector> )->Arg( 200000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<Vector> )->Arg( 100000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<Vector> )->Arg( 75000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<Vector> )->Arg( 50000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<Vector> )->Arg( 37500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<Vector> )->Arg( 25000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<Vector> )->Arg( 12500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<Vector> )->Arg( 1000 )->MinTime( 4.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<Vector> )->Arg( 2000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<Vector> )->Arg( 1500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<Vector> )->Arg( 1000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<Vector> )->Arg( 750000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<Vector> )->Arg( 500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<Vector> )->Arg( 400000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<Vector> )->Arg( 300000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<Vector> )->Arg( 200000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<Vector> )->Arg( 100000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<Vector> )->Arg( 75000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<Vector> )->Arg( 50000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<Vector> )->Arg( 37500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<Vector> )->Arg( 25000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<Vector> )->Arg( 12500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<Vector> )->Arg( 1000 )->MinTime( 4.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<Vector> )->Arg( 2000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<Vector> )->Arg( 1500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<Vector> )->Arg( 1000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<Vector> )->Arg( 750000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<Vector> )->Arg( 500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<Vector> )->Arg( 400000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<Vector> )->Arg( 300000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<Vector> )->Arg( 200000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<Vector> )->Arg( 100000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<Vector> )->Arg( 75000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<Vector> )->Arg( 50000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<Vector> )->Arg( 37500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<Vector> )->Arg( 25000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<Vector> )->Arg( 12500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<Vector> )->Arg( 1000 )->MinTime( 4.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<Vector> )->Arg( 2000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<Vector> )->Arg( 1500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<Vector> )->Arg( 1000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<Vector> )->Arg( 750000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<Vector> )->Arg( 500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<Vector> )->Arg( 400000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<Vector> )->Arg( 300000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<Vector> )->Arg( 200000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<Vector> )->Arg( 100000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<Vector> )->Arg( 75000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<Vector> )->Arg( 50000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<Vector> )->Arg( 37500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<Vector> )->Arg( 25000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<Vector> )->Arg( 12500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<Vector> )->Arg( 1000 )->MinTime( 4.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<Vector> )->Arg( 2000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<Vector> )->Arg( 1500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<Vector> )->Arg( 1000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<Vector> )->Arg( 750000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<Vector> )->Arg( 500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<Vector> )->Arg( 400000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<Vector> )->Arg( 300000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<Vector> )->Arg( 200000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<Vector> )->Arg( 100000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<Vector> )->Arg( 75000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<Vector> )->Arg( 50000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<Vector> )->Arg( 37500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<Vector> )->Arg( 25000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<Vector> )->Arg( 12500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<Vector> )->Arg( 1000 )->MinTime( 4.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<DenseBitArray> )->Arg( 2000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<DenseBitArray> )->Arg( 1500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<DenseBitArray> )->Arg( 1000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<DenseBitArray> )->Arg( 750000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<DenseBitArray> )->Arg( 500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<DenseBitArray> )->Arg( 400000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<DenseBitArray> )->Arg( 300000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<DenseBitArray> )->Arg( 200000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<DenseBitArray> )->Arg( 100000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<DenseBitArray> )->Arg( 75000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<DenseBitArray> )->Arg( 50000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<DenseBitArray> )->Arg( 37500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<DenseBitArray> )->Arg( 25000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<DenseBitArray> )->Arg( 12500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve<DenseBitArray> )->Arg( 1000 )->MinTime( 4.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<DenseBitArray> )->Arg( 2000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<DenseBitArray> )->Arg( 1500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<DenseBitArray> )->Arg( 1000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<DenseBitArray> )->Arg( 750000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<DenseBitArray> )->Arg( 500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<DenseBitArray> )->Arg( 400000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<DenseBitArray> )->Arg( 300000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<DenseBitArray> )->Arg( 200000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<DenseBitArray> )->Arg( 100000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<DenseBitArray> )->Arg( 75000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<DenseBitArray> )->Arg( 50000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<DenseBitArray> )->Arg( 37500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<DenseBitArray> )->Arg( 25000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<DenseBitArray> )->Arg( 12500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve2<DenseBitArray> )->Arg( 1000 )->MinTime( 4.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<DenseBitArray> )->Arg( 2000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<DenseBitArray> )->Arg( 1500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<DenseBitArray> )->Arg( 1000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<DenseBitArray> )->Arg( 750000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<DenseBitArray> )->Arg( 500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<DenseBitArray> )->Arg( 400000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<DenseBitArray> )->Arg( 300000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<DenseBitArray> )->Arg( 200000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<DenseBitArray> )->Arg( 100000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<DenseBitArray> )->Arg( 75000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<DenseBitArray> )->Arg( 50000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<DenseBitArray> )->Arg( 37500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<DenseBitArray> )->Arg( 25000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<DenseBitArray> )->Arg( 12500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve3<DenseBitArray> )->Arg( 1000 )->MinTime( 4.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<DenseBitArray> )->Arg( 2000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<DenseBitArray> )->Arg( 1500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<DenseBitArray> )->Arg( 1000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<DenseBitArray> )->Arg( 750000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<DenseBitArray> )->Arg( 500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<DenseBitArray> )->Arg( 400000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<DenseBitArray> )->Arg( 300000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<DenseBitArray> )->Arg( 200000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<DenseBitArray> )->Arg( 100000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<DenseBitArray> )->Arg( 75000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<DenseBitArray> )->Arg( 50000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<DenseBitArray> )->Arg( 37500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<DenseBitArray> )->Arg( 25000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<DenseBitArray> )->Arg( 12500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve4<DenseBitArray> )->Arg( 1000 )->MinTime( 4.0 );

BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<DenseBitArray> )->Arg( 2000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<DenseBitArray> )->Arg( 1500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<DenseBitArray> )->Arg( 1000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<DenseBitArray> )->Arg( 750000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<DenseBitArray> )->Arg( 500000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<DenseBitArray> )->Arg( 400000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<DenseBitArray> )->Arg( 300000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<DenseBitArray> )->Arg( 200000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<DenseBitArray> )->Arg( 100000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<DenseBitArray> )->Arg( 75000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<DenseBitArray> )->Arg( 50000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<DenseBitArray> )->Arg( 37500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<DenseBitArray> )->Arg( 25000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<DenseBitArray> )->Arg( 12500 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE( BenchSieve, PrimeSieve5<DenseBitArray> )->Arg( 1000 )->MinTime( 4.0 );

BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 10000000000 )->MinTime( 30.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 7500000000 )->MinTime( 30.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 5000000000 )->MinTime( 30.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 2500000000 )->MinTime( 15.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 1000000000 )->MinTime( 15.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 20000000 )->MinTime( 10.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 17500000 )->MinTime( 10.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 15000000 )->MinTime( 5.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 12500000 )->MinTime( 5.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 10000000 )->MinTime( 5.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 8000000 )->MinTime( 5.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 6000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 4000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 2000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 1000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactory>, 1000 )->MinTime( 4.0 );

BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 10000000000 )->MinTime( 30.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 7500000000 )->MinTime( 30.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 5000000000 )->MinTime( 30.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 2500000000 )->MinTime( 15.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 1000000000 )->MinTime( 15.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 20000000 )->MinTime( 10.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 17500000 )->MinTime( 10.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 15000000 )->MinTime( 5.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 12500000 )->MinTime( 5.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 10000000 )->MinTime( 5.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 8000000 )->MinTime( 5.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 6000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 4000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 2000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 1000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE2( BenchAdvancedSieve, AdvancedSieve<PrimeFactoryPooled>, 1000 )->MinTime( 4.0 );

BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<Vector>, 10000000000 )->MinTime( 30.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<Vector>, 7500000000 )->MinTime( 30.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<Vector>, 5000000000 )->MinTime( 30.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<Vector>, 2500000000 )->MinTime( 15.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<Vector>, 1000000000 )->MinTime( 15.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<Vector>, 20000000 )->MinTime( 10.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<Vector>, 17500000 )->MinTime( 10.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<Vector>, 15000000 )->MinTime( 5.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<Vector>, 12500000 )->MinTime( 5.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<Vector>, 10000000 )->MinTime( 5.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<Vector>, 8000000 )->MinTime( 5.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<Vector>, 6000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<Vector>, 4000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<Vector>, 2000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<Vector>, 1000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<Vector>, 1000 )->MinTime( 4.0 );

BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<DenseBitArray>, 10000000000 )->MinTime( 30.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<DenseBitArray>, 7500000000 )->MinTime( 30.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<DenseBitArray>, 5000000000 )->MinTime( 30.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<DenseBitArray>, 2500000000 )->MinTime( 15.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<DenseBitArray>, 1000000000 )->MinTime( 15.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<DenseBitArray>, 20000000 )->MinTime( 10.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<DenseBitArray>, 17500000 )->MinTime( 10.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<DenseBitArray>, 15000000 )->MinTime( 5.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<DenseBitArray>, 12500000 )->MinTime( 5.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<DenseBitArray>, 10000000 )->MinTime( 5.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<DenseBitArray>, 8000000 )->MinTime( 5.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<DenseBitArray>, 6000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<DenseBitArray>, 4000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<DenseBitArray>, 2000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<DenseBitArray>, 1000000 )->MinTime( 4.0 );
BENCHMARK_TEMPLATE2( BenchSieve, PrimeSieve5<DenseBitArray>, 1000 )->MinTime( 4.0 );

int main( int argc, char **argv )
{
    ::benchmark::Initialize( &argc, argv );

    if ( ::benchmark::ReportUnrecognizedArguments( argc, argv ) )
    {
        return 1;
    }

    ::benchmark::RunSpecifiedBenchmarks();
};