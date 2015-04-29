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

#include <type_traits>

// A class template that just a list of types:
template <class... T> struct typelist { };

template <typename List1, typename List2> 
struct typelist_cat;

template <typename ...List1, typename ...List2>
struct typelist_cat<typelist<List1...>, typelist<List2...>>
{
  typedef typelist<List1..., List2...> type;
};

template <typename T, typename ...List2>
struct typelist_cat<T, typelist<List2...>>
{
  typedef typelist<T, List2...> type;
};

template <typename T, typename ...List1>
struct typelist_cat<typelist<List1...>,T>
{
  typedef typelist<List1... ,T> type;
};


template<typename List>
struct typelist_out;

template<typename T, typename ...Args>
struct typelist_out<typelist<T, Args...> >
{
  static void print(std::ostream& os = std::cout, const char sep = '\t') 
  {
    os << T::value << sep;
    typelist_out<typelist<Args...>>::print(os, sep);
  }
};

template<typename T1, typename T2, typename ...Args>
struct typelist_out<typelist<spair<T1,T2>, Args...> >
{
  static void print(std::ostream& os = std::cout, const char sep = '\t') 
  {
    os << T1::value << "^" << T2::value << sep;
    typelist_out<typelist<Args...>>::print(os, sep);
  }
};

template<>
struct typelist_out<typelist<>>
{
  static void print(std::ostream& os = std::cout, const char sep = '\t') 
  { 
    os << std::endl;
  }
};



template<typename TList>
struct Check;

template<typename H, typename ...Tail>
struct Check<typelist<H,Tail...> > {
	static int_t get() {
    return ipow<H::first::value, H::second::value>::value * Check<typelist<Tail...>>::get();
  }
};
template<>
struct Check<typelist<>> {
	static int_t get() {
    return 1;
  }
};


template<int Primecandidate, int K = 1,
bool C1 = ((6*K+1)*(6*K+1) <= Primecandidate),
bool C2 = ((Primecandidate % (6*K+1) != 0) && (Primecandidate % (6*K+5) != 0))> 
struct IsPrime;

template<int Primecandidate, int K> 
struct IsPrime<Primecandidate, K, true, true>
{
  static const bool value = IsPrime<Primecandidate, K+1>::value;
};

template<int Primecandidate, int K> 
struct IsPrime<Primecandidate, K, true, false>
{
  static const bool value = false;
};

template<int Primecandidate, int K> 
struct IsPrime<Primecandidate, K, false, true>
{
  static const bool value = true;
};

template<int Primecandidate, int K> 
struct IsPrime<Primecandidate, K, false, false>
{
  static const bool value = false;
};



template<int Start, int Limit, int K, 
int C = (6*K + 1 <= Limit)>
struct nextPrimesDirect;

template<int Start, int Limit, int K> 
struct nextPrimesDirect<Start, Limit, K, true>
{
  static const int Primecandidate1 = 6*K + 1;
  static const int Primecandidate2 = 6*K + 5;
  static const bool p1 = IsPrime<Primecandidate1>::value && (Primecandidate1 % 5 != 0) && (Primecandidate1 > Start);
  static const bool p2 = IsPrime<Primecandidate2>::value && (Primecandidate2 % 5 != 0) && (Primecandidate2 <= Limit) && (Primecandidate2 > Start);
  typedef typename nextPrimesDirect<Start, Limit, K+1>::type nextIter;
  typedef typename std::conditional<p1 && p2, 
    typename typelist_cat<typelist<sint<Primecandidate1>, sint<Primecandidate2>>, nextIter>::type, 
    typename std::conditional<p1, typename typelist_cat<sint<Primecandidate1>, nextIter>::type, 
    typename std::conditional<p2, typename typelist_cat<sint<Primecandidate2>, nextIter>::type, nextIter>::type>::type>::type type;
};

template<int Start, int Limit, int K> 
struct nextPrimesDirect<Start, Limit, K, false>
{
  typedef typelist<> type;
};


template<int Start, typename List> 
struct select_reminders;

template<int Start, typename T, typename ...List> 
struct select_reminders<Start, typelist<T, List...>>
{
  typedef typename select_reminders<Start, typelist<List...>>::type next;
  typedef typename std::conditional<(Start < T::value),typename typelist_cat<T,next>::type,
     next>::type type;
};

template<int Start, typename T> 
struct select_reminders<Start, typelist<T>>
{
  typedef typelist<T> type;
};



// Two alternatives, but similar implementations
template<int Start, int Limit> 
struct ListOfPrimes
{
  // Two different approaches to build the list of primes
  // 1) Using the list builder nextPrimes
  //    Each next candidate is checked using the existing growing list of primes starting with 'CheckList'
//   typedef Loki::Typelist<sint<2>, Loki::Typelist<sint<3>, Loki::Typelist<sint<5>, Loki::NullType> > > StartList;
//   typedef Loki::Typelist<sint<5>, Loki::NullType> CheckList;
//   typedef typename Loki::TL::Append<StartList, typename nextPrimes<6, CheckList, Limit>::Result>::Result Result;

  // 2) Using the list builder nextPrimesDirect
  //    Each next candidate is checked using the numbers (6*k+1) and (6*k+5), which contain all the primes
  //    This approach builds the list in one way without iterating it
  //    That results in a smaller (2-3 times) compile-time than the first approach
//    typedef typename Loki::Typelist<sint<2>, Loki::Typelist<sint<3>, Loki::Typelist<sint<5>, Loki::Typelist<sint<7>, Loki::Typelist<sint<11>, 
//            typename nextPrimesDirect<Start, Limit, 1>::type> > > > > Result;
   typedef typename typelist_cat<typelist<sint<2>, sint<3>, sint<5>, sint<7>, sint<11>>, 
           typename nextPrimesDirect<Start, Limit, 1>::type>::type type;
};


typedef ListOfPrimes<LastQPrime,StartQ>::type InitialPrimesList;
typedef typelist_cat<sint<1>,select_reminders<LastQPrime,InitialPrimesList>::type>::type Reminders;



template<int_t N, unsigned int Q, int_t K, typename RList>
struct select_factors;

template<int_t N, unsigned int Q, int_t K, typename H, typename ...Tail>
struct select_factors<N,Q,K,typelist<H,Tail...> >
{
  static const int_t candidate = Q*K + H::value;
  typedef typename select_factors<N,Q,K,typelist<Tail...>>::type next;
  typedef typename std::conditional<(N % candidate == 0), 
          typename typelist_cat<H,next>::type, next>::type type;
};

template<int_t N, unsigned int Q, int_t K>
struct select_factors<N,Q,K,typelist<> >
{
  typedef typelist<> type;
};



template<int_t N, unsigned int Q, int_t K, typename RList, bool doExit = false>
struct factor_loop;

template<int_t N, unsigned int Q, int_t K, typename H, typename ...Tail>
struct factor_loop<N,Q,K,typelist<H,Tail...>,false>
{
  static const int_t candidate = Q*K + H::value;
  typedef try_factor<N, candidate> trial;
  typedef spair<sint<candidate>, sint<trial::power> > T;
  typedef typename factor_loop<N/trial::factor, Q, K, typelist<Tail...>, (candidate*candidate > N)>::type nextIter;
  typedef typename std::conditional<(trial::power > 0),typename typelist_cat<T,nextIter>::type, nextIter>::type type;
};

template<int_t N, unsigned int Q, int_t K, typename H, typename ...Tail>
struct factor_loop<N,Q,K,typelist<H,Tail...>,true>
: public factor_loop<N,Q,K,typelist<>,true> {};

template<int_t N, unsigned int Q, int_t K>
struct factor_loop<N,Q,K,typelist<>,false>
{
  static const int_t candidate = Q*(K+1) + 1;
  typedef typename select_factors<N,Q,K+1,Reminders>::type RList;
  typedef typename factor_loop<N,Q,K+1,RList,(candidate*candidate > N)>::type type;
};

template<int_t N, unsigned int Q, int_t K>
struct factor_loop<N,Q,K,typelist<>,true>
{
  typedef spair<sint<N>, sint<1> > T;
  typedef typelist<T> type;
};

template<unsigned int Q, int_t K, typename RList>
struct factor_loop<1,Q,K,RList,true>
{
  typedef typelist<> type;
};

template<unsigned int Q, int_t K>
struct factor_loop<1,Q,K,typelist<>,true>
{
  typedef typelist<> type;
};



template<typename Num, 
unsigned int Q = StartQ,
typename StartList = InitialPrimesList>
struct factorization;

// factorization using trial deletion from InitialPrimesList
template<int_t N, unsigned int Q, typename H, typename ...Tail>
struct factorization<sint<N>, Q, typelist<H,Tail...> >
{
  static const int_t candidate = H::value;
  typedef try_factor<N, candidate> trial;
  static const int_t P = trial::power;
  typedef spair<sint<candidate>, sint<P> > T;
  typedef sint<N/trial::factor> nextN;
  typedef typename factorization<nextN,Q,typelist<Tail...>>::type next;
  typedef typename std::conditional<(P > 0), 
     typename typelist_cat<T, next>::type, next>::type type;
};

// Further factorization 
template<int_t N, unsigned int Q>
struct factorization<sint<N>, Q, typelist<> > 
{
  static const int_t candidate = Q + 1;
  typedef typename select_factors<N,Q,1,Reminders>::type RList;
  typedef typename factor_loop<N,Q,1,RList,(candidate*candidate > N)>::type type;
};

// End of factorization
template<unsigned int Q, typename H, typename ...List>
struct factorization<sint<1>, Q, typelist<H, List...> > {
  typedef typelist<> type;
};

template<unsigned int Q>
struct factorization<sint<1>, Q, typelist<> > {
  typedef typelist<> type;
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
