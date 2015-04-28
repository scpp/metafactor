/***************************************************************************
 *   Copyright (C) 2015 by Vladimir Mirnyy                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/

#ifndef __metafactorcommon_h
#define __metafactorcommon_h

#include "sint.h"

template<int_t N, int_t P>
struct ipow {
  static const int_t value = ipow<N,P-1>::value * N;
};

template<int_t N>
struct ipow<N,1> {
  static const int_t value = N;
};

template<int_t N>
struct ipow<N,0> {
  static const int_t value = 1;
};



template<int_t N, int_t Factor, 
bool C = (N % Factor == 0)>
struct try_factor;
  
template<int_t N, int_t Factor>
struct try_factor<N, Factor, true> {
  typedef try_factor<N/Factor, Factor> next;
  static const int_t power = next::power + 1;
  static const int_t factor = next::factor * Factor;
};

template<int_t N, int_t Factor> 
struct try_factor<N, Factor, false> {
  static const int_t power = 0;
  static const int_t factor = 1;
};

template<int_t Factor> 
struct try_factor<0, Factor, true> {
  static const int_t power = 0;
  static const int_t factor = 1;
};



template<int_t N, int_t Candidate, bool C = (Candidate*Candidate <= N)>
struct check_candidate;

template<int_t N, int_t Candidate>
struct check_candidate<N, Candidate, true>
{
  typedef try_factor<N, Candidate> trial;
  static const int_t power  = trial::power;
  static const int_t nextN  = N/trial::factor;
  static const bool isValid = true;
};

template<int_t N, int_t Candidate>
struct check_candidate<N, Candidate, false>
{
  static const int_t power  = 0;
  static const int_t nextN  = N;
  static const bool isValid = false;
};



#endif /*__metafactorcommon_h*/
