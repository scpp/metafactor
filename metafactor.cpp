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

// Benchmark cases from the article on scpp.eu
//static const int_t N = 65519;  // max. prime unsigned int
//static const int_t N = 4294967291;  // max. prime unsigned int
//static const int_t N = 18446744073709551615;  // max. ulong


// More complicated cases: test your compilers and hardware (g++ will require a lot of RAM)
//static const int_t N = 10000600009;    // = 100003^2 
//static const int_t N = 90004200049;  // = 300007^2

//static const int_t N = 429496729091; 
//static const int_t N = 184467440737095497;
//static const int_t N = 9223372036854775807;  // max long
//static const int_t N = 9223372036854775806; 
//static const int_t N = 18446744073709551613;

#include "sint.h"

// This is a workaround for compilers that could not pass such a big integer as a preprocessor definition
#if !defined(NUM) || NUM == 0 
static const int_t N = 18446744073709551615;
#else
static const int_t N = NUM;
#endif

#ifndef MODE
#define MODE 3
#endif

#if MODE == 1
static const int_t StartQ = 2*3;
static const int_t LastQPrime = 3;
#elif MODE == 2
static const int_t StartQ = 2*3*5;
static const int_t LastQPrime = 5;
#elif MODE == 3
static const int_t StartQ = 2*3*5*7;
static const int_t LastQPrime = 7;
#elif MODE == 4
static const int_t StartQ = 2*3*5*7*11;
static const int_t LastQPrime = 11;
#endif


#if !defined(CPP) || CPP != 11

#include "metafactor.h"
typedef Factorization<sint<N> >::Result FList;

#else  // c++11

#include "metafactor11.h"
typedef factorization<sint<N> >::type FList;

#endif


//typedef ListOfPrimes<7,2*3*5*7>::Result PList;

int main(int argc, char *argv[])
{
    //std::cout << std::numeric_limits<unsigned __int64>::max() << std::endl;
    //std::cout << std::numeric_limits<unsigned long>::max() << std::endl;
  
    typelist_out<FList>::print();
    std::cout << N << std::endl << Check<FList>::get() << std::endl;

//     typedef TYPELIST_7(sint<7>, sint<11>, sint<13>, sint<17>, sint<19>, sint<23>, sint<29>) RList;
//     typedef GenPrimes<210,30,RList> GP;
//     typedef GenPrimes<210*11,210,GP::NextRList> GP2;
//     //typelist_out<GP2::Result>::print();
//     typelist_out<GP2::NextRList>::print();
    //std::cout << Loki::TL::Length<Reminders>::value << std::endl;
	//std::cin.get();
    return 0;
}
