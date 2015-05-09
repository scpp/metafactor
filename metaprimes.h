/***************************************************************************
 *   Copyright (C) 2015 by Vladimir Mirnyy, blog.scpp.eu                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the MIT License                                 *
 ***************************************************************************/

#ifndef __metaprimes_h
#define __metaprimes_h

/** \file
    \brief Primes generation meta-algorithms 
*/

#include <cmath>

#include "metafactorcommon.h"

#include "Typelist.h"


template<int PrimeCandidate, int K = 1,
bool C1 = ((6*K+1)*(6*K+1) <= PrimeCandidate),
bool C2 = ((PrimeCandidate % (6*K+1) != 0) && (PrimeCandidate % (6*K+5) != 0))> 
struct IsPrime;

template<int PrimeCandidate, int K> 
struct IsPrime<PrimeCandidate, K, true, true>
{
  static const bool value = IsPrime<PrimeCandidate, K+1>::value;
};

template<int PrimeCandidate, int K> 
struct IsPrime<PrimeCandidate, K, true, false>
{
  static const bool value = false;
};

template<int PrimeCandidate, int K> 
struct IsPrime<PrimeCandidate, K, false, true>
{
  static const bool value = true;
};

template<int PrimeCandidate, int K> 
struct IsPrime<PrimeCandidate, K, false, false>
{
  static const bool value = false;
};



template<int Start, int Limit, int K, 
int C = (6*K + 1 <= Limit)>
struct NextPrimesDirect;

template<int Start, int Limit, int K> 
struct NextPrimesDirect<Start, Limit, K, true>
{
  static const int PrimeCandidate1 = 6*K + 1;
  static const int PrimeCandidate2 = 6*K + 5;
  static const bool p1 = IsPrime<PrimeCandidate1>::value && (PrimeCandidate1 % 5 != 0) && (PrimeCandidate1 > Start);
  static const bool p2 = IsPrime<PrimeCandidate2>::value && (PrimeCandidate2 % 5 != 0) && (PrimeCandidate2 <= Limit) && (PrimeCandidate2 > Start);
  typedef typename NextPrimesDirect<Start, Limit, K+1>::Result NextIter;
  typedef typename Loki::Select<p1 && p2, 
    Loki::Typelist<sint<PrimeCandidate1>, Loki::Typelist<sint<PrimeCandidate2>, NextIter> >, 
    typename Loki::Select<p1, Loki::Typelist<sint<PrimeCandidate1>, NextIter>, 
    typename Loki::Select<p2, Loki::Typelist<sint<PrimeCandidate2>, NextIter>, NextIter>::Result>::Result>::Result Result;
};

template<int Start, int Limit, int K> 
struct NextPrimesDirect<Start, Limit, K, false>
{
  typedef Loki::NullType Result;
};



template<int_t Candidate, typename InitRList, bool doExit = false>
struct CheckPrime;

template<int_t Candidate, typename H, typename Tail>
struct CheckPrime<Candidate,Loki::Typelist<H,Tail>,false>
{
  static const bool value = (Candidate % H::value != 0) && 
         CheckPrime<Candidate,Tail,(H::value*H::value >= Candidate)>::value;
};

template<int_t Candidate>
struct CheckPrime<Candidate,Loki::NullType,false>
{
  static const bool value = true;
};

template<int_t Candidate, typename H, typename Tail>
struct CheckPrime<Candidate,Loki::Typelist<H,Tail>,true>
{
  static const bool value = true;
};

template<int_t Candidate>
struct CheckPrime<Candidate,Loki::NullType,true>
{
  static const bool value = true;
};


template<int_t Limit, int_t Q, int_t K, typename InitRList, typename CheckFactors,
         typename RList = InitRList, bool doExit = false>
struct GenPrimesFormula;

template<int_t Limit, int_t Q, int_t K, typename InitRList, typename CheckFactors, 
typename H, typename Tail>
struct GenPrimesFormula<Limit,Q,K,InitRList,CheckFactors,Loki::Typelist<H,Tail>,false>
{
  static const int_t candidate = Q*K + H::value;
  typedef typename GenPrimesFormula<Limit, Q, K, InitRList, CheckFactors, Tail, (candidate > Limit)>::Result nextIter;
  typedef typename Loki::Select<(candidate <= Limit && CheckPrime<candidate, CheckFactors>::value), 
          Loki::Typelist<sint<candidate>,nextIter>, nextIter>::Result Result;
//   typedef typename Loki::Select<(candidate <= Limit && (candidate%5 != 0) && IsPrime<candidate>::value), 
//           Loki::Typelist<sint<candidate>,nextIter>, nextIter>::Result Result;
};

template<int_t Limit, int_t Q, int_t K, typename InitRList, typename CheckFactors>
struct GenPrimesFormula<Limit,Q,K,InitRList,CheckFactors,Loki::NullType,false>
{
  static const int_t candidate = Q*(K+1) + 1;
  typedef typename GenPrimesFormula<Limit, Q, K+1, InitRList, CheckFactors, InitRList, (K+1 == InitRList::Head::value)>::Result nextIter;
  typedef typename Loki::Select<(candidate <= Limit && (K+1 < InitRList::Head::value) && CheckPrime<candidate, CheckFactors>::value), 
          Loki::Typelist<sint<candidate>,nextIter>, nextIter>::Result Result;
//   typedef typename Loki::Select<(candidate <= Limit && (K+1 < InitRList::Head::value) && (candidate%5 != 0) && IsPrime<candidate>::value), 
//           Loki::Typelist<sint<candidate>,nextIter>, nextIter>::Result Result;
};

template<int_t Limit, int_t Q, int_t K, typename InitRList, typename CheckFactors, typename H, typename Tail>
struct GenPrimesFormula<Limit,Q,K,InitRList,CheckFactors,Loki::Typelist<H,Tail>,true>
{
  typedef Loki::NullType Result;
};

template<int_t Limit, int_t Q, int_t K, typename InitRList, typename CheckFactors>
struct GenPrimesFormula<Limit,Q,K,InitRList,CheckFactors,Loki::NullType,true>
{
  typedef Loki::NullType Result;
};




template<typename List, typename N>
struct InsertOrdered;

template<typename H, typename Tail, typename N>
struct InsertOrdered<Loki::Typelist<H, Tail>, N>
{
  typedef typename Loki::Select<(N::value <= H::value), Loki::Typelist<N,Loki::Typelist<H, Tail> >,
          Loki::Typelist<H,typename InsertOrdered<Tail, N>::Result> >::Result Result;
};

template<typename N>
struct InsertOrdered<Loki::NullType, N>
{
  typedef Loki::NullType Result;
};


template<int_t Q, typename List, int_t N>
struct FilterRList;

template<int_t Q, typename H, typename Tail, int_t N>
struct FilterRList<Q, Loki::Typelist<H, Tail>, N>
{
  static const bool C = (N*H::value < Q);
  typedef FilterRList<Q,Tail,N> Next;
  typedef typename Loki::Select<C, Loki::Typelist<H,typename Next::ExcludedPrimes>, Loki::NullType>::Result ExcludedPrimes;
  typedef typename Loki::Select<C, typename Next::CheckPrimes, Loki::Typelist<H, Tail> >::Result CheckPrimes;
};

template<int_t Q, int_t N>
struct FilterRList<Q, Loki::NullType, N>
{
  typedef Loki::NullType ExcludedPrimes;
  typedef Loki::NullType CheckPrimes;
};


template<typename List, int_t N>
struct EliminateNonPrimesLoop;

template<typename H, typename Tail, int_t N>
struct EliminateNonPrimesLoop<Loki::Typelist<H, Tail>, N>
{
  typedef typename EliminateNonPrimesLoop<Tail,N>::Result Next;
  typedef typename Loki::Select<(H::value%N != 0), Loki::Typelist<H,Next>, Next>::Result Result;
};

template<int_t N>
struct EliminateNonPrimesLoop<Loki::NullType,N>
{
  typedef Loki::NullType Result;
};


template<typename List, typename PossibleFactors>
struct EliminateNonPrimes;

template<typename List, typename H, typename Tail>
struct EliminateNonPrimes<List, Loki::Typelist<H, Tail> >
{
  typedef typename EliminateNonPrimesLoop<List,H::value>::Result NewList;
  typedef typename EliminateNonPrimes<NewList,Tail>::Result Result;
};

template<typename List>
struct EliminateNonPrimes<List, Loki::NullType>
{
  typedef List Result;
};


template<typename T>
struct GetFirstPrime
{
  static const int_t value = T::Head::value;
};

template<>
struct GetFirstPrime<Loki::NullType>
{
  static const int_t value = 1;
};

template<int_t Limit, int_t StartQ=6, 
typename StartRList = TYPELIST_1(sint<5>), bool doExit = false>
struct GenPrimes;

template<int_t Limit, int_t Q, typename RList>
struct GenPrimes<Limit,Q,RList,false>
{
  static const int_t NextQ = Q*RList::Head::value;
  typedef FilterRList<NextQ,typename RList::Tail,GetFirstPrime<typename RList::Tail>::value> Filter;
  typedef Loki::Typelist<typename RList::Head, typename Filter::CheckPrimes> PrimesToCheck;
  typedef Loki::Typelist<sint<Q+1>,   // Q+1 is always prime and should not be checked
    typename GenPrimesFormula<Limit,Q,1,RList,PrimesToCheck>::Result> NewList;
  typedef typename EliminateNonPrimes<NewList,typename Filter::ExcludedPrimes>::Result PrimesList;
  typedef typename Loki::TL::Append<typename RList::Tail, NewList>::Result NextRList;
  typedef typename GenPrimes<Limit, NextQ, NextRList, (NextQ+1 > Limit)>::Result NextIter;
  typedef typename Loki::TL::Append<PrimesList,NextIter>::Result Result;
};

template<int_t Limit, int_t Q, typename RList>
struct GenPrimes<Limit,Q,RList,true>
{
  typedef Loki::NullType Result;
};


// Two alternatives, but similar implementations
template<int Start, int Limit> 
struct ListOfPrimes
{
  // Two different approaches to build the list of primes
  // 1) Using the list builder NextPrimes
  //    Each next candidate is checked using the existing growing list of primes starting with 'CheckList'
//   typedef Loki::Typelist<sint<2>, Loki::Typelist<sint<3>, Loki::Typelist<sint<5>, Loki::NullType> > > StartList;
//   typedef Loki::Typelist<sint<5>, Loki::NullType> CheckList;
//   typedef typename Loki::TL::Append<StartList, typename NextPrimes<6, CheckList, Limit>::Result>::Result Result;

  // 2) Using the list builder NextPrimesDirect
  //    Each next candidate is checked using the numbers (6*k+1) and (6*k+5), which contain all the primes
  //    This approach builds the list in one way without iterating it
  //    That results in a smaller (2-3 times) compile-time than the first approach
   typedef typename Loki::Typelist<sint<2>, Loki::Typelist<sint<3>, Loki::Typelist<sint<5>, Loki::Typelist<sint<7>, Loki::Typelist<sint<11>, 
           typename NextPrimesDirect<Start, Limit, 1>::Result> > > > > Result;
};


// typedef TYPELIST_2(sint<1>, sint<5>) RList6;
// 
// typedef TYPELIST_8(sint<1>, sint<7>, sint<11>, sint<13>, sint<17>, sint<19>, sint<23>, sint<29>) RList30;
// typedef Loki::TL::Reverse<RList30>::Result Reminders30;
// 
// typedef TYPELIST_43(sint<1>, sint<11>, sint<13>, sint<17>, sint<19>, sint<23>, 
// 		    sint<29>, sint<31>, sint<37>, sint<41>, sint<43>, sint<47>, sint<53>, sint<59>, sint<61>, 
// 		    sint<67>, sint<71>, sint<73>, sint<79>, sint<83>, sint<89>, sint<97>, sint<101>, sint<103>, 
// 		    sint<107>, sint<109>, sint<113>, sint<127>, sint<131>, sint<137>, sint<139>, sint<149>, 
// 		    sint<151>, sint<157>, sint<163>, sint<167>, sint<173>, sint<179>, sint<181>, sint<191>, 
// 		    sint<193>, sint<197>, sint<199>) RList210;
// typedef Loki::TL::Reverse<RList210>::Result Reminders210;
// 
// typedef Reminders210 Reminders;

//typedef TYPELIST_5(sint<2>, sint<3>, sint<5>, sint<7>, sint<11>) InitialPrimesList;

// typedef TYPELIST_10(sint<2>, sint<3>, sint<5>, sint<7>, sint<11>, 
// 		    sint<13>, sint<17>, sint<19>, sint<23>, sint<29>) InitialPrimesList;

// typedef TYPELIST_46(sint<2>, sint<3>, sint<5>, sint<7>, sint<11>, sint<13>, sint<17>, sint<19>, sint<23>, 
// 		    sint<29>, sint<31>, sint<37>, sint<41>, sint<43>, sint<47>, sint<53>, sint<59>, sint<61>, 
// 		    sint<67>, sint<71>, sint<73>, sint<79>, sint<83>, sint<89>, sint<97>, sint<101>, sint<103>, 
// 		    sint<107>, sint<109>, sint<113>, sint<127>, sint<131>, sint<137>, sint<139>, sint<149>, 
// 		    sint<151>, sint<157>, sint<163>, sint<167>, sint<173>, sint<179>, sint<181>, sint<191>, 
// 		    sint<193>, sint<197>, sint<199>) InitialPrimesList;

template<int Start, typename List, bool C = (Start < List::Head::value)> 
struct BuildList;

template<int Start, typename List> 
struct BuildList<Start, List, false>
{
  typedef typename BuildList<Start, typename List::Tail>::Result Result;
};

template<int Start, typename List> 
struct BuildList<Start, List, true>
{
  typedef Loki::Typelist<sint<1>, List> Result;
};


typedef ListOfPrimes<LastQPrime,StartQ>::Result InitialPrimesList;
//typedef Loki::TL::Reverse<BuildList<LastQPrime,InitialPrimesList>::Result>::Result Reminders;
typedef BuildList<LastQPrime,InitialPrimesList>::Result Reminders;




template<int_t N, short Q, int_t K, typename RList>
struct CheckAll;

template<int_t N, short Q, int_t K, typename H, typename Tail>
struct CheckAll<N,Q,K,Loki::Typelist<H,Tail> >
{
  static const int_t Candidate = Q*K + H::value;
  static const bool value = (N % Candidate == 0) || CheckAll<N,Q,K,Tail>::value;
};

template<int_t N, short Q, int_t K>
struct CheckAll<N,Q,K,Loki::NullType>
{
  static const bool value = false;
};


template<typename List>
struct CheckPrimesList;

template<typename H, typename T>
struct CheckPrimesList<Loki::Typelist<H,T> >
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
    return CheckPrimesList<T>::apply();
  }
};

template<>
struct CheckPrimesList<Loki::NullType>
{
  static bool apply() { return true; }
};

#endif /*__metaprimes_h*/
