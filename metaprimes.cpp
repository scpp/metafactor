/***************************************************************************
 *   Copyright (C) 2015 by Vladimir Mirnyy, blog.scpp.eu                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the MIT License                                 *
 ***************************************************************************/

/** \file
    \brief Program to demonstrate compile-time primes list generation
    
*/

#include <iostream>
#include <limits>

#include "sint.h"

// This is a workaround for compilers that could not pass such a big integer as a preprocessor definition
#if !defined(NUM) || NUM == 0 
static const int_t N = 100;
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

// #if !defined(CPP) || CPP != 11
#include "metafactor.h"
//typedef ListOfPrimes<LastQPrime,N>::Result PrimesList;
// #else  // c++11
// #include "metafactor11.h"
// typedef ListOfPrimes<LastQPrime,N>::type PrimesList;
// #endif

#include "metaprimes.h"

//typedef ListOfPrimes<7,2*3*5*7>::Result PList;

int main(int argc, char *argv[])
{
//  typedef Loki::Typelist<sint<5>,Loki::NullType> RList5;
//   typedef TYPELIST_7(sint<7>,sint<11>,sint<13>,sint<17>,sint<19>,sint<23>,sint<29>) RList;
//   typedef FilterRList<210,typename RList::Tail,11> Filter;
//   typedef Loki::Typelist<typename RList::Head, typename Filter::CheckPrimes> PrimesToCheck;
//  typedef GenPrimesFormula<210,30,1,RList,PrimesToCheck>::Result PrimesList;
//   typedef GenPrimesFormula<210,30,1,RList>::Result PrimesList30;
//   typedef Loki::TL::Append<PrimesList5,Loki::Typelist<sint<31>,PrimesList30> >::Result PList;
//    typedef FilterRList<210,RList>::ExcludedPrimes PrimesList;
    
  typedef GenPrimes<1000>::Result PrimesList;
  typelist_out<PrimesList>::print(std::cout, '\n');
  std::cout << Loki::TL::Length<PrimesList>::value + 3 << std::endl;

  // Run-time check of the list
  if (CheckPrimesList<PrimesList>::apply())
    std::cout << "Ok!" << std::endl;
  
  //std::cin.get();
  return 0;
}
