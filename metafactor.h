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
struct typelist_out<Loki::Typelist<spair<H1,H2>, Tail> >
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
    os << std::endl; 
  }
};



template<typename TList>
struct Check;

template<typename H, typename Tail>
struct Check<Loki::Typelist<H,Tail> > {
  static int_t get() {
    return ipow<H::first::value, H::second::value>::value * Check<Tail>::get();
  }
};
template<>
struct Check<Loki::NullType> 
{
    static int_t get() {
    return 1;
  }
};


template<class TList> struct Print;

template<> struct Print<Loki::NullType> { };

template<class Head, class Tail>
struct Print<Loki::Typelist<Head,Tail> > {
   typedef typename Print<Tail>::Result Result;
};


template<int I = MODE-1>
struct InitList 
{
  typedef InitList<I-1> Prev;
  typedef GenPrimes<StartQ,StartQ/LastQPrime,typename Prev::Reminders> Formula;
  
  typedef typename Formula::NextRList Reminders;
  typedef typename Loki::TL::Append<typename Prev::PrimesList, typename Formula::Result>::Result PrimesList;
};

template<>
struct InitList<0>
{
  typedef TYPELIST_1(sint<5>) Reminders;
  typedef TYPELIST_3(sint<2>, sint<3>, sint<5>) PrimesList;
};


typedef InitList<MODE>::PrimesList InitialPrimesList;
typedef InitList<MODE>::Reminders  Reminders;
 
// typedef TYPELIST_1(sint<5>) Reminders1;
// typedef TYPELIST_3(sint<2>, sint<3>, sint<5>) InitialPrimesList1;
// 
// typedef TYPELIST_7(sint<7>, sint<11>, sint<13>, sint<17>, sint<19>, sint<23>, sint<29>) Reminders2;
// typedef typename Loki::TL::Append<InitialPrimesList1, Reminders2>::Result InitialPrimesList2;
// 
// typedef GenPrimes<210,30,Reminders2> Formula30k;
// typedef Formula30k::NextRList Reminders3;
// typedef typename Loki::TL::Append<InitialPrimesList2, Formula30k::Result>::Result InitialPrimesList3;
// 
// typedef GenPrimes<210*11,210,Reminders3> Formula210k;
// typedef Formula210k::NextRList Reminders4;
// typedef typename Loki::TL::Append<InitialPrimesList3, Formula210k::Result>::Result InitialPrimesList4;




template<int_t N, short Q, int_t K, typename RList>
struct SelectFactors;

template<int_t N, short Q, int_t K, typename H, typename Tail>
struct SelectFactors<N,Q,K,Loki::Typelist<H,Tail> >
{
  static const int_t Candidate = Q*K + H::value;
  typedef typename SelectFactors<N,Q,K,Tail>::Result Next;
  typedef typename Loki::Select<(N % Candidate == 0), Loki::Typelist<H,Next>, Next>::Result Result;
};

template<int_t N, short Q, int_t K>
struct SelectFactors<N,Q,K,Loki::NullType>
{
  typedef Loki::NullType Result;
};


template<int_t N, unsigned int Q, int_t K, typename RList, bool doExit = false>
struct FactorLoop;

template<int_t N, unsigned int Q, int_t K, typename H, typename Tail>
struct FactorLoop<N,Q,K,Loki::Typelist<H,Tail>,false>
{
  static const int_t candidate = Q*K + H::value;
  typedef try_factor<N, candidate> trial;
  typedef spair<sint<candidate>, sint<trial::power> > T;
  typedef typename FactorLoop<N/trial::factor, Q, K, Tail, (candidate*candidate > N)>::type nextIter;
  typedef typename Loki::Select<(trial::power > 0), Loki::Typelist<T,nextIter>, nextIter>::Result Result;
};

template<int_t N, unsigned int Q, int_t K, typename H, typename Tail>
struct FactorLoop<N,Q,K,Loki::Typelist<H,Tail>,true>
: public FactorLoop<N,Q,K,Loki::NullType,true> {};

template<int_t N, unsigned int Q, int_t K>
struct FactorLoop<N,Q,K,Loki::NullType,false>
{
  static const int_t Candidate = Q*(K+1) + 1;
  typedef typename SelectFactors<N,Q,K+1,Reminders>::type RList;
  typedef typename FactorLoop<N,Q,K+1,RList,(Candidate*Candidate > N)>::Result Result;
};

template<int_t N, unsigned int Q, int_t K>
struct FactorLoop<N,Q,K,Loki::NullType,true>
{
  typedef spair<sint<N>, sint<1> > T;
  typedef Loki::Typelist<T,Loki::NullType> Result;
};

template<unsigned int Q, int_t K, typename RList>
struct FactorLoop<1,Q,K,RList,true>
{
  typedef Loki::NullType Result;
};

template<unsigned int Q, int_t K>
struct FactorLoop<1,Q,K,Loki::NullType,true>
{
  typedef Loki::NullType Result;
};


template<typename Num,
typename StartList = InitialPrimesList>
struct Factorization;

// Factorization using trial deletion from InitialPrimesList
template<int_t N, typename H, typename Tail>
struct Factorization<sint<N>, Loki::Typelist<H,Tail> >
{
  typedef try_factor<N, H::value> trial;
  static const int_t P = trial::power;
  typedef sint<N/trial::factor> NextNum;
  typedef typename Factorization<NextNum,Tail>::Result Next;
  typedef typename Loki::Select<(P > 0), 
     Loki::Typelist<spair<sint<H::value>, sint<P> >, Next>, Next>::Result Result;
};

// Further factorization 
template<int_t N>
struct Factorization<sint<N>, Loki::NullType> 
{
  static const short Q = StartQ;
  static const int_t Candidate = Q + 1;
  typedef typename SelectFactors<N,Q,1,Reminders>::Result RList;
  typedef typename FactorLoop<N,Q,1,RList,(Candidate*Candidate > N)>::Result Result;
};

// End of factorization
template<typename H, typename Tail>
struct Factorization<sint<1>, Loki::Typelist<H,Tail> > {
  typedef Loki::NullType Result;
};


#endif /*__metafactor_h*/
