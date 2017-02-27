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
#ifndef __PRIMESIEVE_PRIMEFACTORY_H__
#define __PRIMESIEVE_PRIMEFACTORY_H__

#include <stdint.h>

class ISmallPrime;

template< uint8_t tPrimeMod30 >
class Prime;

template< uint8_t tPrimeMod30 >
class PrimeFactoryPooledBase
{
public:

    Prime<tPrimeMod30> *Init( uint64_t prime, uint64_t segmentStart )
    {
        Prime<tPrimeMod30> *p = mPool.Create();
        p->Init( prime, segmentStart );
        return p;
    }

    void Release( Prime<tPrimeMod30> *p )
    {
        mPool.Destroy( p );
    }

private:

    UnsychronisedMemoryPoolInstantiator<Prime<tPrimeMod30>> mPool;
};


class PrimeFactoryPooled
    : PrimeFactoryPooledBase<7>,
      PrimeFactoryPooledBase<11>,
      PrimeFactoryPooledBase<13>,
      PrimeFactoryPooledBase<17>,
      PrimeFactoryPooledBase<19>,
      PrimeFactoryPooledBase<23>,
      PrimeFactoryPooledBase<29>,
      PrimeFactoryPooledBase<31>
{
public:

    template<uint64_t tPrimeMod30>
    __forceinline Prime<tPrimeMod30> *Init( uint64_t prime, uint64_t segmentStart )
    {
        return PrimeFactoryPooledBase<tPrimeMod30>::Init( prime, segmentStart );
    }

    template<uint64_t tPrimeMod30>
    void Release( Prime<tPrimeMod30> *p )
    {
        PrimeFactoryPooledBase<tPrimeMod30>::Release( p );
    }
};


class PrimeFactory
{
public:

    template<uint64_t tPrimeMod30>
    inline Prime<tPrimeMod30> *Init( uint64_t prime, uint64_t segmentStart )
    {
        return new Prime<tPrimeMod30>( prime, segmentStart );
    }

    template<uint64_t tPrimeMod30>
    inline void Release( Prime<tPrimeMod30> *p )
    {
        delete p;
    }
};

#endif
