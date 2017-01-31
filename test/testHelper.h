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
#ifndef __PRIMESIEVE_TESTHELPER_H__
#define __PRIMESIEVE_TESTHELPER_H__

#include "gtest/gtest.h"

#define CONCATEXT( a, b ) a##b
#define CONCAT( a, b ) CONCATEXT( a, b )
#define P( prefix ) CONCAT( PREFIX, prefix )

#ifndef PRIMES_FILE
#ifndef _MSC_VER
#   define PRIMES_FILE "test/primes.txt"
#else
#   define PRIMES_FILE "../../test/primes.txt"
#endif
#endif

template< typename tT >
inline void ExpectEqual( const tT &t1, const tT &t2, const std::string &message = "" )
{
    EXPECT_EQ( t1, t2 ) << message;
}

template<>
inline void ExpectEqual< float >( const float &f1, const float &f2, const std::string &message )
{
    EXPECT_FLOAT_EQ( f1, f2 ) << message;
}

template<>
inline void ExpectEqual< double >( const double &d1, const double &d2, const std::string &message )
{
    EXPECT_DOUBLE_EQ( d1, d2 ) << message;
}

inline void ExpectEqual( const char *c1, const char *c2, const std::string &message = "" )
{
    std::string s1( c1 ), s2( c2 );
    EXPECT_EQ( s1, s2 ) << message;
}

#endif
