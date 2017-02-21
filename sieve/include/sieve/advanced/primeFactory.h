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
class SmallPrime;


class SmallPrimeFactoryPooled
{
public:

    template<uint64_t tPrimeMod30>
    SmallPrime<tPrimeMod30> *Init( uint64_t /*prime*/, uint64_t /*segmentStart*/ )
    {
        static_assert( false, "This should not be called" );
        return nullptr;
    }

    template<uint64_t tPrimeMod30>
    void Release( SmallPrime<tPrimeMod30> * /*p*/ )
    {
        static_assert( false, "This should not be called" );
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

#define SIEVE_FACTORY_POOLED_INIT( bitValue, factory )                                                          \
    template<>                                                                                                  \
    SmallPrime<bitValue> *SmallPrimeFactoryPooled::Init<bitValue>( uint64_t prime, uint64_t segmentStart )      \
    {                                                                                                           \
        SmallPrime<bitValue> *p = factory.Create();                                                             \
        p->Init( prime, segmentStart );                                                                         \
        return p;                                                                                               \
    }

SIEVE_FACTORY_POOLED_INIT( 7, mFactory7 );
SIEVE_FACTORY_POOLED_INIT( 11, mFactory11 );
SIEVE_FACTORY_POOLED_INIT( 13, mFactory13 );
SIEVE_FACTORY_POOLED_INIT( 17, mFactory17 );
SIEVE_FACTORY_POOLED_INIT( 19, mFactory19 );
SIEVE_FACTORY_POOLED_INIT( 23, mFactory23 );
SIEVE_FACTORY_POOLED_INIT( 29, mFactory29 );
SIEVE_FACTORY_POOLED_INIT( 31, mFactory31 );

#undef SIEVE_FACTORY_POOLED_INIT

#define SIEVE_FACTORY_POOLED_RELEASE( bitValue, factory )                   \
    template<>                                                              \
    void SmallPrimeFactoryPooled::Release( SmallPrime<bitValue> *p )        \
    {                                                                       \
        factory.Destroy( reinterpret_cast<SmallPrime<bitValue>  *>( p ) );  \
    }

SIEVE_FACTORY_POOLED_RELEASE( 7, mFactory7 );
SIEVE_FACTORY_POOLED_RELEASE( 11, mFactory11 );
SIEVE_FACTORY_POOLED_RELEASE( 13, mFactory13 );
SIEVE_FACTORY_POOLED_RELEASE( 17, mFactory17 );
SIEVE_FACTORY_POOLED_RELEASE( 19, mFactory19 );
SIEVE_FACTORY_POOLED_RELEASE( 23, mFactory23 );
SIEVE_FACTORY_POOLED_RELEASE( 29, mFactory29 );
SIEVE_FACTORY_POOLED_RELEASE( 31, mFactory31 );

#undef SIEVE_FACTORY_POOLED_RELEASE


class SmallPrimeFactory
{
public:

    template<uint64_t tPrimeMod30>
    inline SmallPrime<tPrimeMod30> *Init( uint64_t prime, uint64_t segmentStart )
    {
        return new SmallPrime<tPrimeMod30>( prime, segmentStart );
    }

    template<uint64_t tPrimeMod30>
    inline void Release( SmallPrime<tPrimeMod30> *p )
    {
        delete p;
    }
};

#endif