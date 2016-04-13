/***************************************************************************
 *   Copyright (C) 2015 by Vladimir Mirnyy, blog.scpp.eu                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the MIT License                                 *
 ***************************************************************************/

/** \file
    \brief Program to demonstrate compile-time factorization
    
*/

#include <iostream>
#include <limits>

#include "sint.h"

// Benchmark cases from the article on scpp.eu
//static const int_t N = 65521;  // max. prime unsigned int
//static const int_t N = 4294967291;  // max. prime unsigned int
//static const int_t N = 18446744073709551615;  // max. ulong

// More complicated cases: test your compilers and hardware (g++ will require a lot of RAM)
//static const int_t N = 9223372036854775807;  // max long  = 7^2×73×127×337×92737×649657
//static const int_t N = 10000600009;    // = 100003^2 
//static const int_t N = 90004200049;  // = 300007^2
//static const int_t N = 429496729091; // a prime above unsigned int range

// The cases I couldn't compile with g++ 4.8.3
//static const int_t N = 184467440737095497;  // a prime
//static const int_t N = 9223372036854775806; // = 2*3*715827883*2147483647
//static const int_t N = 18446744073709551613; // = 13*3889*364870227143809

//This is a workaround for compilers that could not pass such a long integer as a preprocessor definition
#if !defined(NUM) || NUM == 0 
static const uint_t N = 18446744073709551615;
#else
static const uint_t N = NUM;
#endif
//static const int_t N = 4294967291;  // max long  = 7^2×73×127×337×92737×649657

#ifndef MODE
#define MODE 3
#endif


#if !defined(CPP) || CPP != 11

#include "metafactor.h"
typedef Factorization<sint<N> >::Result FList;

#else  // c++11

#include "metafactor11.h"
typedef factorization<sint<N> >::type FList;

#endif


int main(int argc, char *argv[])
{
    //std::cout << std::numeric_limits<unsigned __int64>::max() << std::endl;
    //std::cout << std::numeric_limits<unsigned long>::max() << std::endl;
  
//     typelist_out<InitialPrimesList>::print();
//     typelist_out<Reminders>::print();
    
    typelist_out<FList>::print();
    std::cout << N << std::endl << Check<FList>::get() << std::endl;

	//std::cin.get();
    return 0;
}
