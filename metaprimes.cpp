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
#if !defined(LIMIT) || LIMIT == 0 
static const int_t N = 100;
#else
static const int_t N = LIMIT;
#endif

#ifndef MODE
#define MODE 3
#endif


#if !defined(CPP) || CPP != 11

#include "metafactor.h"
#include "metaprimes.h"
typedef GeneratePrimesF2direct<N>::Result PrimesList;

#else  // c++11

#include "metafactor11.h"
#include "metaprimes11.h"
typedef GeneratePrimesF2direct<N>::type PrimesList;

#endif



int main(int argc, char *argv[])
{

  typelist_out<PrimesList>::print(std::cout, '\n');
//   std::cout << Loki::TL::Length<PrimesList>::value << std::endl;

  // Run-time check of the list
  if (CheckPrimesListAtRunTime<PrimesList>::apply())
    std::cout << "Ok!" << std::endl;
  
  //std::cin.get();
  return 0;
}
