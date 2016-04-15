/***************************************************************************
 *   Copyright (C) 2015 by Vladimir Mirnyy, blog.scpp.eu                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the MIT License                                 *
 ***************************************************************************/

#ifndef __metaprimescommon_h
#define __metaprimescommon_h

/** \file
    \brief Primes generation meta-algorithms common for Loki::Typelist and typelist<...>
*/


template<ulong_t PrimeCandidate>
struct CheckNone
{
  static const bool value = true;
};

template<ulong_t PrimeCandidate>
struct CheckSmallPrimes5
{
  static const bool value = (PrimeCandidate%5 != 0);
};

template<ulong_t PrimeCandidate>
struct CheckSmallPrimes2_5
{
  static const bool value = (PrimeCandidate%2 != 0) && (PrimeCandidate%3 != 0) && (PrimeCandidate%5 != 0);
};

template<ulong_t PrimeCandidate>
struct CheckSmallPrimes13_29
{
  static const bool value = (PrimeCandidate%13 != 0) && (PrimeCandidate%17 != 0)
                         && (PrimeCandidate%19 != 0) && (PrimeCandidate%23 != 0) && (PrimeCandidate%29 != 0);
};

template<ulong_t PrimeCandidate>
struct CheckSmallPrimes11_29
{
  static const bool value = (PrimeCandidate%11 != 0) && (PrimeCandidate%13 != 0) && (PrimeCandidate%17 != 0)
                         && (PrimeCandidate%19 != 0) && (PrimeCandidate%23 != 0) && (PrimeCandidate%29 != 0);
};

template<ulong_t PrimeCandidate>
struct CheckSmallPrimes7_29
{
  static const bool value = (PrimeCandidate%7 != 0) && CheckSmallPrimes11_29<PrimeCandidate>::value;
};

template<ulong_t PrimeCandidate>
struct CheckSmallPrimes2_29
{
  static const bool value = CheckSmallPrimes2_5<PrimeCandidate>::value && CheckSmallPrimes7_29<PrimeCandidate>::value;
};

template<ulong_t PrimeCandidate, int K = 1,
bool C1 = ((6*K+1)*(6*K+1) <= PrimeCandidate),
bool C2 = ((PrimeCandidate % (6*K+1) != 0) && (PrimeCandidate % (6*K+5) != 0))> 
struct IsPrime6loop;

template<ulong_t PrimeCandidate, int K>
struct IsPrime6loop<PrimeCandidate, K, true, true>
{
  static const bool value = IsPrime6loop<PrimeCandidate, K+1>::value;
};

template<ulong_t PrimeCandidate, int K>
struct IsPrime6loop<PrimeCandidate, K, true, false>
{
  static const bool value = false;
};

template<ulong_t PrimeCandidate, int K>
struct IsPrime6loop<PrimeCandidate, K, false, true>
{
  static const bool value = true;
};

template<ulong_t PrimeCandidate, int K>
struct IsPrime6loop<PrimeCandidate, K, false, false>
{
  static const bool value = false;
};

template<ulong_t PrimeCandidate>
struct IsPrime6loop<PrimeCandidate, 1, false, false>
{
  static const bool value = true;
};


///// -> The implementation below is significantly slower than one above
// template<ulong_t PrimeCandidate, int K = 1,
// bool doExit = ((6*K+1)*(6*K+1) > PrimeCandidate)>
// struct IsPrime6loop;
// 
// template<ulong_t PrimeCandidate, int K>
// struct IsPrime6loop<PrimeCandidate,K,false>
// {
//   static const bool C1 = (PrimeCandidate % (6*K+1) != 0);
//   static const bool C2 = (PrimeCandidate % (6*K+5) != 0);
//   static const bool value = C1 && C2 && IsPrime6loop<PrimeCandidate,K+1>::value;
// };
// 
// template<ulong_t PrimeCandidate, int K>
// struct IsPrime6loop<PrimeCandidate,K,true>
// {
//   static const bool value = true;
// };

template<template<ulong_t> class InitCheck = CheckSmallPrimes2_5>
struct IsPrime6
{
  template<ulong_t PrimeCandidate>
  struct Do {
    static const bool value = (InitCheck<PrimeCandidate>::value && IsPrime6loop<PrimeCandidate>::value);
  };
};



template<ulong_t PrimeCandidate, int K = 1,
bool C1 = ((30*K+1)*(30*K+1) <= PrimeCandidate),
bool C2 = (PrimeCandidate % (30*K+1) != 0) && (PrimeCandidate % (30*K+7) != 0)
       && (PrimeCandidate % (30*K+11) != 0) && (PrimeCandidate % (30*K+13) != 0)
       && (PrimeCandidate % (30*K+17) != 0) && (PrimeCandidate % (30*K+19) != 0)
       && (PrimeCandidate % (30*K+23) != 0) && (PrimeCandidate % (30*K+29) != 0) > 
struct IsPrime30loop;

template<ulong_t PrimeCandidate, int K>
struct IsPrime30loop<PrimeCandidate, K, true, true>
{
  static const bool value = IsPrime30loop<PrimeCandidate, K+1>::value;
};

template<ulong_t PrimeCandidate, int K>
struct IsPrime30loop<PrimeCandidate, K, true, false>
{
  static const bool value = false;
};

template<ulong_t PrimeCandidate, int K>
struct IsPrime30loop<PrimeCandidate, K, false, true>
{
  static const bool value = true;
};

template<ulong_t PrimeCandidate, int K>
struct IsPrime30loop<PrimeCandidate, K, false, false>
{
  static const bool value = false;
};

// template<ulong_t PrimeCandidate>
// struct IsPrime30loop<PrimeCandidate, 1, false, true>
// {
//   static const bool value = (PrimeCandidate%5 != 0);
// };

template<ulong_t PrimeCandidate>
struct IsPrime30loop<PrimeCandidate, 1, false, false>
{
  static const bool value = (PrimeCandidate%7 != 0);
};


template<template<ulong_t> class InitCheck = CheckSmallPrimes2_29>
struct IsPrime30
{
  template<ulong_t PrimeCandidate>
  struct Do {
    static const bool value = InitCheck<PrimeCandidate>::value && IsPrime30loop<PrimeCandidate>::value;
  };
};





#endif /*__metaprimescommon_h*/
