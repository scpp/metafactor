/***************************************************************************
 *   Copyright (C) 2015 by Vladimir Mirnyy, blog.scpp.eu                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the MIT License                                 *
 ***************************************************************************/

#ifndef __metafactor_h
#define __metafactor_h

/** \file
    \brief Factorization meta-algorithms 
*/

#include <cmath>

#include "metafactorcommon.h"
#include "metaprimes.h"

#include "Typelist.h"

template<typename TList>
struct Product;

template<typename H, typename Tail>
struct Product<Loki::Typelist<H,Tail> >
{
  static const ulong_t value = H::value * Product<Tail>::value;
};

template<typename H>
struct Product<Loki::Typelist<H,Loki::NullType> >
{
  static const ulong_t value = H::value;
};

template<>
struct Product<Loki::NullType>
{ }; // compilt-time error


#if MODE == 1
typedef TYPELIST_2(ulong_<3>,ulong_<2>) QFactorList;
// static const ulong_t StartQ = 2*3;
#elif MODE == 2
typedef TYPELIST_3(ulong_<5>,ulong_<3>,ulong_<2>) QFactorList;
// static const ulong_t StartQ = 2*3*5;
#elif MODE == 3
typedef TYPELIST_4(ulong_<7>,ulong_<5>,ulong_<3>,ulong_<2>) QFactorList;
// static const ulong_t StartQ = 2*3*5*7;
#elif MODE == 4
typedef TYPELIST_5(ulong_<11>,ulong_<7>,ulong_<5>,ulong_<3>,ulong_<2>) QFactorList;
// static const ulong_t StartQ = 2*3*5*7*11;
#endif

static const ulong_t StartQ = Product<QFactorList>::value;

template<typename List>
struct typelist_out;

template<typename H, typename Tail>
struct typelist_out<Loki::Typelist<H, Tail> >
{
  static void print(std::ostream& os = std::cout, const char sep = '\t') 
  {
    os << H::value << sep;
    typelist_out<Tail>::print(os, sep);
  }
};

template<typename H1, typename H2, typename Tail>
struct typelist_out<Loki::Typelist<pair_<H1,H2>, Tail> >
{
  static void print(std::ostream& os = std::cout, const char sep = '\t') 
  {
    os << H1::value << "^" << H2::value << sep;
    typelist_out<Tail>::print(os, sep);
  }
};

template<>
struct typelist_out<Loki::NullType>
{
  static void print(std::ostream& os = std::cout, const char sep = '\t') 
  { 
    (void)sep;
    os << std::endl; 
  }
};



template<typename TList>
struct Check;

template<typename H, typename Tail>
struct Check<Loki::Typelist<H,Tail> > {
  static ulong_t get() {
    return ipow<H::first::value, H::second::value>::value * Check<Tail>::get();
  }
};
template<>
struct Check<Loki::NullType> 
{
    static ulong_t get() {
    return 1;
  }
};


template<class TList> struct Print;

template<> struct Print<Loki::NullType> { };

template<class Head, class Tail>
struct Print<Loki::Typelist<Head,Tail> > {
   typedef typename Print<Tail>::Result Result;
};


template<int I = MODE-1, ulong_t Limit = StartQ, typename QFList = QFactorList>
struct InitList;

template<int I, ulong_t Limit, typename H, typename Tail>
struct InitList<I, Limit, Loki::Typelist<H,Tail> >
{
  static const ulong_t NextLimit = Limit/H::value;
  typedef InitList<I-1,NextLimit,Tail> Prev;
  typedef Hierarchic::GenPrimes<Limit,NextLimit,typename Prev::Reminders> Formula;
  
  typedef typename Formula::NextRList Reminders;
  typedef typename Loki::TL::Append<typename Prev::PrimesList, typename Formula::Result>::Result PrimesList;
};

template<ulong_t Limit, typename H, typename Tail>
struct InitList<0, Limit, Loki::Typelist<H,Tail> >
{
  typedef TYPELIST_1(ulong_<5>) Reminders;
  typedef TYPELIST_3(ulong_<2>, ulong_<3>, ulong_<5>) PrimesList;
};


typedef InitList<>::PrimesList InitialPrimesList;
typedef InitList<>::Reminders  Reminders;
 
// typedef TYPELIST_1(ulong_<5>) Reminders1;
// typedef TYPELIST_3(ulong_<2>, ulong_<3>, ulong_<5>) InitialPrimesList1;
// 
// typedef TYPELIST_7(ulong_<7>, ulong_<11>, ulong_<13>, ulong_<17>, ulong_<19>, ulong_<23>, ulong_<29>) Reminders2;
// typedef typename Loki::TL::Append<InitialPrimesList1, Reminders2>::Result InitialPrimesList2;
// 
// typedef GenPrimes<210,30,Reminders2> Formula30k;
// typedef Formula30k::NextRList Reminders3;
// typedef typename Loki::TL::Append<InitialPrimesList2, Formula30k::Result>::Result InitialPrimesList3;
// 
// typedef GenPrimes<210*11,210,Reminders3> Formula210k;
// typedef Formula210k::NextRList Reminders4;
// typedef typename Loki::TL::Append<InitialPrimesList3, Formula210k::Result>::Result InitialPrimesList4;




template<ulong_t N, short Q, ulong_t K, typename RList>
struct SelectFactors;

template<ulong_t N, short Q, ulong_t K, typename H, typename Tail>
struct SelectFactors<N,Q,K,Loki::Typelist<H,Tail> >
{
  static const ulong_t Candidate = Q*K + H::value;
  typedef typename SelectFactors<N,Q,K,Tail>::Result Next;
  typedef typename Loki::Select<(N % Candidate == 0), Loki::Typelist<H,Next>, Next>::Result Result;
};

template<ulong_t N, short Q, ulong_t K>
struct SelectFactors<N,Q,K,Loki::NullType>
{
  typedef Loki::NullType Result;
};


template<ulong_t N, unsigned int Q, ulong_t K, typename RList, bool doExit = false>
struct FactorLoop;

template<ulong_t N, unsigned int Q, ulong_t K, typename H, typename Tail>
struct FactorLoop<N,Q,K,Loki::Typelist<H,Tail>,false>
{
  static const ulong_t candidate = Q*K + H::value;
  typedef try_factor<N, candidate> trial;
  typedef pair_<ulong_<candidate>, ulong_<trial::power> > T;
  typedef typename FactorLoop<N/trial::factor, Q, K, Tail, (candidate*candidate > N)>::Result nextIter;
  typedef typename Loki::Select<(trial::power > 0), Loki::Typelist<T,nextIter>, nextIter>::Result Result;
};

template<ulong_t N, unsigned int Q, ulong_t K, typename RList>
struct FactorLoop<N,Q,K,RList,true>
: public FactorLoop<N,Q,K,Loki::NullType,true> {};

template<ulong_t N, unsigned int Q, ulong_t K>
struct FactorLoop<N,Q,K,Loki::NullType,false>
{
  static const ulong_t Candidate = Q*(K+1) + 1;
  typedef typename SelectFactors<N,Q,K+1,Reminders>::Result RList;
  typedef typename FactorLoop<N,Q,K+1,RList,(Candidate*Candidate > N)>::Result Result;
};

template<ulong_t N, unsigned int Q, ulong_t K>
struct FactorLoop<N,Q,K,Loki::NullType,true>
{
  typedef pair_<ulong_<N>, ulong_<1> > T;
  typedef Loki::Typelist<T,Loki::NullType> Result;
};

template<unsigned int Q, ulong_t K, typename RList>
struct FactorLoop<1,Q,K,RList,true>
{
  typedef Loki::NullType Result;
};

template<unsigned int Q, ulong_t K>
struct FactorLoop<1,Q,K,Loki::NullType,true>
{
  typedef Loki::NullType Result;
};


template<typename Num,
typename StartList = InitialPrimesList>
struct Factorization;

// Factorization using trial deletion from InitialPrimesList
template<ulong_t N, typename H, typename Tail>
struct Factorization<ulong_<N>, Loki::Typelist<H,Tail> >
{
  typedef try_factor<N, H::value> trial;
  static const ulong_t P = trial::power;
  typedef ulong_<N/trial::factor> NextNum;
  typedef typename Factorization<NextNum,Tail>::Result Next;
  typedef typename Loki::Select<(P > 0), 
     Loki::Typelist<pair_<ulong_<H::value>, ulong_<P> >, Next>, Next>::Result Result;
};

// Further factorization 
template<ulong_t N>
struct Factorization<ulong_<N>, Loki::NullType>
{
  static const short Q = StartQ;
  static const ulong_t Candidate = Q + 1;
  typedef typename SelectFactors<N,Q,1,Reminders>::Result RList;
  typedef typename FactorLoop<N,Q,1,RList,(Candidate*Candidate > N)>::Result Result;
};

// End of factorization
template<typename H, typename Tail>
struct Factorization<ulong_<1>, Loki::Typelist<H,Tail> > {
  typedef Loki::NullType Result;
};


#endif /*__metafactor_h*/
