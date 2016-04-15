/***************************************************************************
 *   Copyright (C) 2015 by Vladimir Mirnyy, blog.scpp.eu                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the MIT License                                 *
 ***************************************************************************/

#ifndef __metafactorcommon_h
#define __metafactorcommon_h

#include "sint.h"

template<ulong_t N, ulong_t P>
struct ipow {
  static const ulong_t value = ipow<N,P-1>::value * N;
};

template<ulong_t N>
struct ipow<N,1> {
  static const ulong_t value = N;
};

template<ulong_t N>
struct ipow<N,0> {
  static const ulong_t value = 1;
};



template<ulong_t N, ulong_t Factor,
bool C = (N % Factor == 0)>
struct try_factor;
  
template<ulong_t N, ulong_t Factor>
struct try_factor<N, Factor, true> {
  typedef try_factor<N/Factor, Factor> next;
  static const ulong_t power = next::power + 1;
  static const ulong_t factor = next::factor * Factor;
};

template<ulong_t N, ulong_t Factor>
struct try_factor<N, Factor, false> {
  static const ulong_t power = 0;
  static const ulong_t factor = 1;
};

template<ulong_t Factor>
struct try_factor<0, Factor, true> {
  static const ulong_t power = 0;
  static const ulong_t factor = 1;
};



template<ulong_t N, ulong_t Candidate, bool C = (Candidate*Candidate <= N)>
struct check_candidate;

template<ulong_t N, ulong_t Candidate>
struct check_candidate<N, Candidate, true>
{
  typedef try_factor<N, Candidate> trial;
  static const ulong_t power  = trial::power;
  static const ulong_t nextN  = N/trial::factor;
  static const bool isValid = true;
};

template<ulong_t N, ulong_t Candidate>
struct check_candidate<N, Candidate, false>
{
  static const ulong_t power  = 0;
  static const ulong_t nextN  = N;
  static const bool isValid = false;
};



#endif /*__metafactorcommon_h*/
