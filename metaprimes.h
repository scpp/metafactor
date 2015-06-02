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

template<int_t PrimeCandidate>
struct CheckNone
{
  static const bool value = true;
};

template<int_t PrimeCandidate>
struct CheckSmallPrimes5
{
  static const bool value = (PrimeCandidate%5 != 0);
};

template<int_t PrimeCandidate>
struct CheckSmallPrimes2_5
{
  static const bool value = (PrimeCandidate%2 != 0) && (PrimeCandidate%3 != 0) && (PrimeCandidate%5 != 0);
};

template<int_t PrimeCandidate>
struct CheckSmallPrimes13_29
{
  static const bool value = (PrimeCandidate%13 != 0) && (PrimeCandidate%17 != 0)
                         && (PrimeCandidate%19 != 0) && (PrimeCandidate%23 != 0) && (PrimeCandidate%29 != 0);
};

template<int_t PrimeCandidate>
struct CheckSmallPrimes11_29
{
  static const bool value = (PrimeCandidate%11 != 0) && (PrimeCandidate%13 != 0) && (PrimeCandidate%17 != 0)
                         && (PrimeCandidate%19 != 0) && (PrimeCandidate%23 != 0) && (PrimeCandidate%29 != 0);
};

template<int_t PrimeCandidate>
struct CheckSmallPrimes7_29
{
  static const bool value = (PrimeCandidate%7 != 0) && CheckSmallPrimes11_29<PrimeCandidate>::value;
};

template<int_t PrimeCandidate>
struct CheckSmallPrimes2_29
{
  static const bool value = CheckSmallPrimes2_5<PrimeCandidate>::value && CheckSmallPrimes7_29<PrimeCandidate>::value;
};

template<int_t PrimeCandidate, int K = 1,
bool C1 = ((6*K+1)*(6*K+1) <= PrimeCandidate),
bool C2 = ((PrimeCandidate % (6*K+1) != 0) && (PrimeCandidate % (6*K+5) != 0))> 
struct IsPrime6loop;

template<int_t PrimeCandidate, int K> 
struct IsPrime6loop<PrimeCandidate, K, true, true>
{
  static const bool value = IsPrime6loop<PrimeCandidate, K+1>::value;
};

template<int_t PrimeCandidate, int K> 
struct IsPrime6loop<PrimeCandidate, K, true, false>
{
  static const bool value = false;
};

template<int_t PrimeCandidate, int K> 
struct IsPrime6loop<PrimeCandidate, K, false, true>
{
  static const bool value = true;
};

template<int_t PrimeCandidate, int K> 
struct IsPrime6loop<PrimeCandidate, K, false, false>
{
  static const bool value = false;
};

template<int_t PrimeCandidate> 
struct IsPrime6loop<PrimeCandidate, 1, false, false>
{
  static const bool value = true;
};


///// -> The implementation below is significantly slower than one above
// template<int_t PrimeCandidate, int K = 1, 
// bool doExit = ((6*K+1)*(6*K+1) > PrimeCandidate)>
// struct IsPrime6loop;
// 
// template<int_t PrimeCandidate, int K>
// struct IsPrime6loop<PrimeCandidate,K,false>
// {
//   static const bool C1 = (PrimeCandidate % (6*K+1) != 0);
//   static const bool C2 = (PrimeCandidate % (6*K+5) != 0);
//   static const bool value = C1 && C2 && IsPrime6loop<PrimeCandidate,K+1>::value;
// };
// 
// template<int_t PrimeCandidate, int K>
// struct IsPrime6loop<PrimeCandidate,K,true>
// {
//   static const bool value = true;
// };

template<template<int_t> class InitCheck = CheckSmallPrimes2_5>
struct IsPrime6
{
  template<int_t PrimeCandidate>
  struct Do {
    static const bool value = (InitCheck<PrimeCandidate>::value && IsPrime6loop<PrimeCandidate>::value);
  };
};



template<int_t PrimeCandidate, int K = 1,
bool C1 = ((30*K+1)*(30*K+1) <= PrimeCandidate),
bool C2 = (PrimeCandidate % (30*K+1) != 0) && (PrimeCandidate % (30*K+7) != 0)
       && (PrimeCandidate % (30*K+11) != 0) && (PrimeCandidate % (30*K+13) != 0)
       && (PrimeCandidate % (30*K+17) != 0) && (PrimeCandidate % (30*K+19) != 0)
       && (PrimeCandidate % (30*K+23) != 0) && (PrimeCandidate % (30*K+29) != 0) > 
struct IsPrime30loop;

template<int_t PrimeCandidate, int K> 
struct IsPrime30loop<PrimeCandidate, K, true, true>
{
  static const bool value = IsPrime30loop<PrimeCandidate, K+1>::value;
};

template<int_t PrimeCandidate, int K> 
struct IsPrime30loop<PrimeCandidate, K, true, false>
{
  static const bool value = false;
};

template<int_t PrimeCandidate, int K> 
struct IsPrime30loop<PrimeCandidate, K, false, true>
{
  static const bool value = true;
};

template<int_t PrimeCandidate, int K> 
struct IsPrime30loop<PrimeCandidate, K, false, false>
{
  static const bool value = false;
};

// template<int_t PrimeCandidate> 
// struct IsPrime30loop<PrimeCandidate, 1, false, true>
// {
//   static const bool value = (PrimeCandidate%5 != 0);
// };

template<int_t PrimeCandidate> 
struct IsPrime30loop<PrimeCandidate, 1, false, false>
{
  static const bool value = (PrimeCandidate%7 != 0);
};


template<template<int_t> class InitCheck = CheckSmallPrimes2_29>
struct IsPrime30
{
  template<int_t PrimeCandidate>
  struct Do {
    static const bool value = InitCheck<PrimeCandidate>::value && IsPrime30loop<PrimeCandidate>::value;
  };
};


template<int Start, int Limit, int K, 
int C = (6*K + 1 <= Limit)>
struct NextPrimesDirect;

template<int Start, int Limit, int K> 
struct NextPrimesDirect<Start, Limit, K, true>
{
  static const int PrimeCandidate1 = 6*K + 1;
  static const int PrimeCandidate2 = 6*K + 5;
  static const bool p1 = IsPrime6<CheckSmallPrimes5>::Do<PrimeCandidate1>::value && (PrimeCandidate1 > Start);
  static const bool p2 = IsPrime6<CheckSmallPrimes5>::Do<PrimeCandidate2>::value && (PrimeCandidate2 <= Limit) && (PrimeCandidate2 > Start);
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

/////////////////////////////////////////////

template<typename C, bool b, typename Next>
struct AddType
{
  typedef Next Result;
};

template<typename C, typename Next>
struct AddType<C,true,Next>
{
  typedef Loki::Typelist<C,Next> Result;
};

template<int Limit, int K = 1, int C = (30*K + 1 <= Limit)>
struct NextPrimesDirect30;

template<int Limit, int K> 
struct NextPrimesDirect30<Limit, K, true>
{
  typedef IsPrime30<CheckSmallPrimes7_29> isPrime;
  static const int_t C1 = 30*K + 1;
  static const int_t C2 = 30*K + 7;
  static const int_t C3 = 30*K + 11;
  static const int_t C4 = 30*K + 13;
  static const int_t C5 = 30*K + 17;
  static const int_t C6 = 30*K + 19;
  static const int_t C7 = 30*K + 23;
  static const int_t C8 = 30*K + 29;
  static const bool p1 = isPrime::Do<C1>::value;
  static const bool p2 = isPrime::Do<C2>::value && (C2 <= Limit);
  static const bool p3 = isPrime::Do<C3>::value && (C3 <= Limit);
  static const bool p4 = isPrime::Do<C4>::value && (C4 <= Limit);
  static const bool p5 = isPrime::Do<C5>::value && (C5 <= Limit);
  static const bool p6 = isPrime::Do<C6>::value && (C6 <= Limit);
  static const bool p7 = isPrime::Do<C7>::value && (C7 <= Limit);
  static const bool p8 = isPrime::Do<C8>::value && (C8 <= Limit);
  typedef typename NextPrimesDirect30<Limit, K+1>::Result NextIter;
  
  typedef typename AddType<sint<C8>,p8,NextIter>::Result R8;
  typedef typename AddType<sint<C7>,p7,R8>::Result R7;
  typedef typename AddType<sint<C6>,p6,R7>::Result R6;
  typedef typename AddType<sint<C5>,p5,R6>::Result R5;
  typedef typename AddType<sint<C4>,p4,R5>::Result R4;
  typedef typename AddType<sint<C3>,p3,R4>::Result R3;
  typedef typename AddType<sint<C2>,p2,R3>::Result R2;
  typedef typename AddType<sint<C1>,p1,R2>::Result Result;
};

template<int Limit, int K> 
struct NextPrimesDirect30<Limit, K, false>
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
};

template<int_t Limit, int_t Q, int_t K, typename InitRList, typename CheckFactors>
struct GenPrimesFormula<Limit,Q,K,InitRList,CheckFactors,Loki::NullType,false>
{
  static const int_t candidate = Q*(K+1) + 1;
  typedef typename GenPrimesFormula<Limit, Q, K+1, InitRList, CheckFactors, InitRList, (K+1 == InitRList::Head::value)>::Result nextIter;
  typedef typename Loki::Select<(candidate <= Limit && (K+1 < InitRList::Head::value) && CheckPrime<candidate, CheckFactors>::value), 
          Loki::Typelist<sint<candidate>,nextIter>, nextIter>::Result Result;
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
    typename GenPrimesFormula<(NextQ<Limit) ? NextQ : Limit,Q,1,RList,PrimesToCheck>::Result> NewList;
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

//////////////////////////////////////////////////////////////////////////////////////////

template<int_t Limit, int_t Q, int_t K, typename InitRList, 
         typename CheckPrimeCandidate = IsPrime6<>,
         typename RList = InitRList, bool doExit = false>
struct GeneratePrimes;

template<int_t Limit, int_t Q, int_t K, typename InitRList, typename H, typename Tail, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,Loki::Typelist<H,Tail>,false>
{
  static const int_t candidate = Q*K + H::value;
  typedef typename GeneratePrimes<Limit, Q, K, InitRList, CheckPrimeCandidate, Tail, (candidate > Limit)>::Result nextIter;
  typedef typename Loki::Select<(candidate <= Limit && CheckPrimeCandidate::template Do<candidate>::value), 
          Loki::Typelist<sint<candidate>,nextIter>, nextIter>::Result Result;
};

template<int_t Limit, int_t Q, int_t K, typename InitRList, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,Loki::NullType,false>
: public GeneratePrimes<Limit, Q, K+1, InitRList> {};

template<int_t Limit, int_t Q, int_t K, typename InitRList, typename H, typename Tail, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,Loki::Typelist<H,Tail>,true>
{ typedef Loki::NullType Result; };

template<int_t Limit, int_t Q, int_t K, typename InitRList, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,Loki::NullType,true>
{ typedef Loki::NullType Result; };


template<int_t Limit>
struct GeneratePrimesF1
{
  typedef TYPELIST_1(sint<5>) List1;
  typedef Loki::Typelist<sint<1>,List1> RList;
  
//   typedef typename GeneratePrimes<Limit,6,1,RList,IsPrime6<CheckSmallPrimes5> >::Result PrimesList;
  typedef typename GeneratePrimes<Limit,6,1,RList,IsPrime30<CheckSmallPrimes13_29> >::Result PrimesList;
  typedef Loki::Typelist<sint<2>,Loki::Typelist<sint<3>,Loki::Typelist<sint<5>,PrimesList> > > Result;
};


template<int_t Limit>
struct GeneratePrimesF1direct
{
  typedef typename NextPrimesDirect<0,Limit,1>::Result PrimesList;
  typedef typename Loki::Typelist<sint<2>,Loki::Typelist<sint<3>,Loki::Typelist<sint<5>,PrimesList> > > Result;
};


template<int_t Limit>
struct GeneratePrimesF2
{
  typedef TYPELIST_7(sint<7>,sint<11>,sint<13>,sint<17>,sint<19>,sint<23>,sint<29>) List1;
  typedef Loki::Typelist<sint<1>,List1> RList;
  
//   typedef typename GeneratePrimes<Limit,30,1,RList,IsPrime6<CheckNone> >::Result PrimesList;
  typedef typename GeneratePrimes<Limit,30,1,RList,IsPrime30<CheckNone> >::Result PrimesList;
  typedef Loki::Typelist<sint<2>,Loki::Typelist<sint<3>,Loki::Typelist<sint<5>,List1> > > List2;
  typedef typename Loki::TL::Append<List2, PrimesList>::Result Result;
};


template<int_t Limit>
struct GeneratePrimesF2direct
{
  typedef TYPELIST_10(sint<2>,sint<3>,sint<5>,sint<7>,sint<11>,sint<13>,sint<17>,sint<19>,sint<23>,sint<29>) List1;
  typedef typename NextPrimesDirect30<Limit>::Result PrimesList;
  typedef typename Loki::TL::Append<List1, PrimesList>::Result Result;
};


template<int_t Limit>
struct GeneratePrimesF3
{
  typedef TYPELIST_6(sint<11>,sint<13>,sint<17>,sint<19>,sint<23>,sint<29>) List1;
  // Generate list containing primes and divisible by 11 and 13 
  // The formula 30k+r is used, but from generated numbers only divisible by 7 are eliminated
  typedef Loki::Typelist<sint<1>,Loki::Typelist<sint<7>,List1> > RemindersListForFormula30k;
  typedef typename GenPrimesFormula<210,30,1,RemindersListForFormula30k,TYPELIST_1(sint<7>)>::Result NewList;
  typedef typename Loki::TL::Append<Loki::Typelist<sint<1>,List1>, NewList>::Result RemindersListForFormula210k;
  typedef typename EliminateNonPrimes<NewList,TYPELIST_2(sint<11>,sint<13>)>::Result PList;

  // Creates a list of primes starting with 211 and not exceeding Limit
//   typedef typename GeneratePrimes<Limit,210,1,RList,IsPrime6<CheckNone> >::Result PrimesList;
  typedef typename GeneratePrimes<Limit,210,1,RemindersListForFormula210k,IsPrime30<CheckSmallPrimes11_29> >::Result PrimesList;
  
  // Add primes less than 211
  typedef Loki::Typelist<sint<2>,Loki::Typelist<sint<3>,Loki::Typelist<sint<5>,Loki::Typelist<sint<7>,List1> > > > List2;
  typedef typename Loki::TL::Append<List2, PList>::Result List3;
  typedef typename Loki::TL::Append<List3, PrimesList>::Result Result;
};

// Obsolete and unused!
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
   typedef typename Loki::Typelist<sint<2>, Loki::Typelist<sint<3>, Loki::Typelist<sint<5>, Loki::Typelist<sint<7>,  
           typename NextPrimesDirect<Start, Limit, 1>::Result> > > > Result;
};


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
