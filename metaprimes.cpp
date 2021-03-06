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


#if !defined(LIMIT) || (LIMIT == 0)
static const ulong_t N = 1000;
#else
static const ulong_t N = LIMIT;
#endif

#ifndef MODE
#define MODE 1
#endif


#if !defined(CPP) || CPP != 11

#include "metafactor.h"
#include "metaprimes.h"

// Top 3 compile-time generation implementations (for gcc 5.4)
#if MODE == 1
typedef F210K::GeneratePrimesWithList<N>::Result PrimesList;
#elif MODE == 2
typedef F30K::GeneratePrimesWithList<N>::Result PrimesList;
//typedef F30K::Sieve<N>::Result PrimesList;
#elif MODE == 3
typedef F6K::GeneratePrimesWithList<N>::Result PrimesList;
//typedef F6K::Sieve<N>::Result PrimesList;
#endif

#else  // c++11 or higher

#include "metafactor11.h"
#include "metaprimes11.h"

// Top 3 compile-time generation implementations (for gcc 5.4)
// They are generally slower than without c++11 above
#if MODE == 1
typedef F210K::GeneratePrimesWithList<N>::type PrimesList;
#elif MODE == 2
typedef F30K::GeneratePrimesWithList<N>::type PrimesList;
#elif MODE == 3
typedef F6K::GeneratePrimesWithList<N>::type PrimesList;
#endif

#endif



int main(int argc, char *argv[])
{

  typelist_out<PrimesList>::print(std::cout, '\t');
//   std::cout << Loki::TL::Length<PrimesList>::value << std::endl;

  std::cout << "Check ... ";

  // Run-time check of the list
  if (CheckPrimesListAtRunTime<PrimesList>::apply())
    std::cout << "Ok!" << std::endl;
  
  //std::cin.get();
  return 0;
}
