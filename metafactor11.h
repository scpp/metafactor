/***************************************************************************
 *   Copyright (C) 2015 by Vladimir Mirnyy, blog.scpp.eu                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the MIT License                                 *
 ***************************************************************************/

#ifndef __metafactor11_h
#define __metafactor11_h

/** \file
    \brief factorization meta-algorithms 
*/

#include <cmath>

#include "metafactorcommon.h"
#include "metaprimes11.h"


template<typename TList>
struct Product;

template<typename H, typename ...Tail>
struct Product<typelist<H,Tail...> >
{
  static const ulong_t value = H::value * Product<typelist<Tail...>>::value;
};

template<typename H>
struct Product<typelist<H> >
{
  static const ulong_t value = H::value;
};

template<>
struct Product<typelist<>>
{
//  static const ulong_t value = 0;
};

#if MODE == 1
typedef typelist<ulong_<3>,ulong_<2>> QFactorList;
// static const ulong_t StartQ = 2*3;
#elif MODE == 2
typedef typelist<ulong_<5>,ulong_<3>,ulong_<2>> QFactorList;
// static const ulong_t StartQ = 2*3*5;
#elif MODE == 3
typedef typelist<ulong_<7>,ulong_<5>,ulong_<3>,ulong_<2>> QFactorList;
// static const ulong_t StartQ = 2*3*5*7;
#elif MODE == 4
typedef typelist<ulong_<11>,ulong_<7>,ulong_<5>,ulong_<3>,ulong_<2>> QFactorList;
// static const ulong_t StartQ = 2*3*5*7*11;
#endif

static const ulong_t StartQ = Product<QFactorList>::value;


template<typename TList>
struct Check;

template<typename H, typename ...Tail>
struct Check<typelist<H,Tail...> > {
  static ulong_t get() {
    return ipow<H::first::value, H::second::value>::value * Check<typelist<Tail...>>::get();
  }
};
template<>
struct Check<typelist<>> {
  static ulong_t get() {
    return 1;
  }
};



template<int Start, typename List> 
struct select_reminders;

template<int Start, typename T, typename ...List> 
struct select_reminders<Start, typelist<T, List...>>
{
  using next = typename select_reminders<Start, typelist<List...>>::type;
  using type = typename std::conditional<(Start < T::value), 
               typename typelist_cat<T,next>::type, next>::type;
};

template<int Start, typename T> 
struct select_reminders<Start, typelist<T>>
{
  using type = typelist<T>;
};


template<int I = MODE-1, ulong_t Limit = StartQ, typename QFList = QFactorList>
struct InitList;

template<int I, ulong_t Limit, typename H, typename ...Tail>
struct InitList<I, Limit, typelist<H,Tail...> >
{
  static const ulong_t NextLimit = Limit/H::value;
  using Prev = InitList<I-1,NextLimit,typelist<Tail...>>;
  using Formula = Hierarchic::GenPrimes<Limit,NextLimit,typename Prev::Reminders>;
  
  using Reminders = typename Formula::NextRList;
  using PrimesList = typename typelist_cat<typename Prev::PrimesList, typename Formula::type>::type;
};

template<ulong_t Limit, typename H, typename ...Tail>
struct InitList<0, Limit, typelist<H, Tail...> >
{
  using Reminders = typelist<ulong_<5>>;
  using PrimesList = typelist<ulong_<2>, ulong_<3>, ulong_<5>>;
};


using InitialPrimesList = InitList<>::PrimesList;
using Reminders = InitList<>::Reminders;


constexpr ulong_t factor_power(ulong_t N, ulong_t Factor)
{
  return (N % Factor == 0) ? 1+factor_power(N/Factor, Factor) : 0;
}

constexpr ulong_t full_factor(ulong_t N, ulong_t Factor)
{
  return (N % Factor == 0) ? Factor*full_factor(N/Factor, Factor) : 1;
}


template<ulong_t N, unsigned int Q, ulong_t K, typename RList>
struct select_factors;

template<ulong_t N, unsigned int Q, ulong_t K, typename H, typename ...Tail>
struct select_factors<N,Q,K,typelist<H,Tail...> >
{
  static const ulong_t candidate = Q*K + H::value;
  using next = typename select_factors<N,Q,K,typelist<Tail...>>::type;
  using type = typename std::conditional<(N % candidate == 0), typename typelist_cat<H,next>::type, next>::type;
};

template<ulong_t N, unsigned int Q, ulong_t K>
struct select_factors<N,Q,K,typelist<> >
{
  using type = typelist<>;
};



template<ulong_t N, unsigned int Q, ulong_t K, typename RList, bool doExit = false>
struct factor_loop;

template<ulong_t N, unsigned int Q, ulong_t K, typename H, typename ...Tail>
struct factor_loop<N,Q,K,typelist<H,Tail...>,false>
{
  static const ulong_t candidate = Q*K + H::value;
  using trial = try_factor<N, candidate>;
  static const ulong_t P = trial::power;
  static const ulong_t F = trial::factor;
//   static const ulong_t P = factor_power(N, candidate);
//   static const ulong_t F = full_factor(N, candidate);
  using T = pair_<ulong_<candidate>, ulong_<P> >;
  using nextIter = typename factor_loop<N/F, Q, K, typelist<Tail...>, (candidate*candidate > N)>::type;
  using type = typename std::conditional<(P > 0), typename typelist_cat<T,nextIter>::type, nextIter>::type;
};

template<ulong_t N, unsigned int Q, ulong_t K>
struct factor_loop<N,Q,K,typelist<>,false>
{
  static const ulong_t candidate = Q*(K+1) + 1;
  using RList = typename select_factors<N,Q,K+1,Reminders>::type;
  using type = typename factor_loop<N,Q,K+1,RList,(candidate*candidate > N)>::type;
};

template<ulong_t N, unsigned int Q, ulong_t K, typename RList>
struct factor_loop<N,Q,K,RList,true>
: public factor_loop<N,Q,K,typelist<>,true> {};

template<ulong_t N, unsigned int Q, ulong_t K>
struct factor_loop<N,Q,K,typelist<>,true>
{
  using type = typelist<pair_<ulong_<N>, ulong_<1>>>;
};

template<unsigned int Q, ulong_t K, typename RList>
struct factor_loop<1,Q,K,RList,true>
{
  using type = typelist<>;
};

template<unsigned int Q, ulong_t K>
struct factor_loop<1,Q,K,typelist<>,true>
{
  using type = typelist<>;
};




template<ulong_t N, unsigned int Q, ulong_t K, typename RList, bool doExit = false>
struct factor_loop_r;

template<ulong_t N, unsigned int Q, ulong_t K, typename H, typename ...Tail>
struct factor_loop_r<N,Q,K,typelist<H,Tail...>,false>
{
  static const ulong_t candidate = Q*K + H::value;
  using trial = try_factor<N, candidate>;
  static const ulong_t P = trial::power;
  static const ulong_t F = trial::factor;
//   static const ulong_t P = factor_power(N, candidate);
//   static const ulong_t F = full_factor(N, candidate);
  using T = pair_<ulong_<candidate>, ulong_<P> >;
  using nextIter = typename factor_loop_r<N/F, Q, K, typelist<Tail...>, (candidate*candidate > N)>::type;
  using type = typename std::conditional<(P > 0), typename typelist_cat<T,nextIter>::type, nextIter>::type;
};

template<ulong_t N, unsigned int Q, ulong_t K>
struct factor_loop_r<N,Q,K,typelist<>,false>
{
  using type = typelist<>;
};

template<ulong_t N, unsigned int Q, ulong_t K>
struct factor_loop_r<N,Q,K,typelist<>,true>
{
  using type = typelist<pair_<ulong_<N>, ulong_<1>>>;
};

template<ulong_t N, unsigned int Q, ulong_t K, typename RList>
struct factor_loop_r<N,Q,K,RList,true>
{
  using type = typelist<pair_<ulong_<N>, ulong_<1>>>;
};

template<unsigned int Q, ulong_t K, typename RList>
struct factor_loop_r<1,Q,K,RList,true>
{
  using type = typelist<>;
};

template<unsigned int Q, ulong_t K>
struct factor_loop_r<1,Q,K,typelist<>,true>
{
  using type = typelist<>;
};



template<ulong_t N, unsigned int Q, ulong_t K, typename RList, bool doExit = false>
struct factor_loop_k
{
  static const ulong_t candidate = Q*(K+1) + 1;
  using ListK = typename factor_loop_r<N,Q,K,Reminders>::type;
  using Next = typename factor_loop_k<N,Q,K+1,Reminders,(candidate*candidate > N)>::type;
  using type = typename typelist_cat<ListK,Next>::type;
};

template<ulong_t N, unsigned int Q, ulong_t K, typename RList>
struct factor_loop_k<N,Q,K,RList,true>
{
  using type = typelist<pair_<ulong_<N>, ulong_<1>>>;
};



template<typename Num, 
unsigned int Q = StartQ,
typename StartList = InitialPrimesList>
struct factorization;

// factorization using trial deletion from InitialPrimesList
template<ulong_t N, unsigned int Q, typename H, typename ...Tail>
struct factorization<ulong_<N>, Q, typelist<H,Tail...> >
{
  static const ulong_t candidate = H::value;
  using trial = try_factor<N, candidate>;
  static const ulong_t P = trial::power;
  static const ulong_t F = trial::factor;
//   static const ulong_t P = factor_power(N, candidate);
//   static const ulong_t F = full_factor(N, candidate);
  using T = pair_<ulong_<candidate>, ulong_<P> >;
  using nextN = ulong_<N/F>;
  using next = typename factorization<nextN,Q,typelist<Tail...>>::type;
  using type = typename std::conditional<(P > 0), 
     typename typelist_cat<T, next>::type, next>::type;
};

// Further factorization 
template<ulong_t N, unsigned int Q>
struct factorization<ulong_<N>, Q, typelist<> >
{
  static const ulong_t candidate = Q + 1;
  using RList = typename select_factors<N,Q,1,Reminders>::type;
//   using RList = Reminders;
  using type = typename factor_loop<N,Q,1,RList,(candidate*candidate > N)>::type;
};

// End of factorization
template<unsigned int Q, typename H, typename ...List>
struct factorization<ulong_<1>, Q, typelist<H, List...> > {
  using type = typelist<>;
};

template<unsigned int Q>
struct factorization<ulong_<1>, Q, typelist<> > {
  using type = typelist<>;
};


template<typename List>
struct CheckPrimesList;

template<typename H, typename ...Tail>
struct CheckPrimesList<typelist<H,Tail...> >
{
  static bool apply() 
  {
    if (H::value > 2 && H::value % 2 == 0) {
      std::cout << ">>> " << H::value << std::endl;
      return false;
    }
    if (H::value > 3 && H::value % 3 == 0) {
      std::cout << ">>> " << H::value << std::endl;
      return false;
    }
    if (H::value > 5 && H::value % 5 == 0) {
      std::cout << ">>> " << H::value << std::endl;
      return false;
    }
    int k = 1;
    while (6*k+1 <= (int)floor(sqrt((double)H::value))) {
      if (H::value % (6*k+1) == 0) {
	std::cout << ">>> " << H::value << std::endl;
	return false;
      }
      if (H::value % (6*k+5) == 0) {
	std::cout << ">>> " << H::value << std::endl;
	return false;
      }
      k++;
    }
    return CheckPrimesList<typelist<Tail...>>::apply();
  }
};

template<>
struct CheckPrimesList<typelist<>>
{
  static bool apply() { return true; }
};

#endif /*__metafactor11_h*/
