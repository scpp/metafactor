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

#if !defined(CPP) || CPP != 11
#include "metafactor.h"
typedef ListOfPrimes<LastQPrime,N>::Result PrimesList;
#else  // c++11
#include "metafactor11.h"
typedef ListOfPrimes<LastQPrime,N>::type PrimesList;
#endif


//typedef ListOfPrimes<7,2*3*5*7>::Result PList;

int main(int argc, char *argv[])
{
  typelist_out<PrimesList>::print();
//  std::cout << Loki::TL::Length<PrimesList>::value << std::endl;

  // Run-time check of the list
  if (CheckPrimesList<PrimesList>::apply())
    std::cout << "Ok!" << std::endl;
  
  //std::cin.get();
  return 0;
}
