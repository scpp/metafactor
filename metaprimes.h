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
#include "metaprimescommon.h"

#include "Typelist.h"


// Initial simple and efficient primes generator using formula 6k+r, r = 1,5
// It has shorter compile-times comparing to GenPrimes
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

// Experimental code to achieve shorter compile-times comparing to GenPrimes
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

// Returns true, if Candidate is not divisible by any entry of TrialList
template<int_t Candidate, typename TrialList, bool doExit = false>
struct isNotDivisibleByListMembers;

template<int_t Candidate, typename H, typename Tail>
struct isNotDivisibleByListMembers<Candidate,Loki::Typelist<H,Tail>,false>
{
  static const bool value = (Candidate % H::value != 0) && 
         isNotDivisibleByListMembers<Candidate,Tail,(H::value*H::value >= Candidate)>::value;
};

template<int_t Candidate>
struct isNotDivisibleByListMembers<Candidate,Loki::NullType,false>
{
  static const bool value = true;
};

template<int_t Candidate, typename H, typename Tail>
struct isNotDivisibleByListMembers<Candidate,Loki::Typelist<H,Tail>,true>
{
  static const bool value = true;
};

template<int_t Candidate>
struct isNotDivisibleByListMembers<Candidate,Loki::NullType,true>
{
  static const bool value = true;
};

// Generate all primes less equal Limit with a constant Q in the formula Q*K+R
// Second exit criteria: (K+1 == InitRList::Head::value)
template<int_t Limit, int_t Q, int_t K, typename InitRList, typename CheckFactors,
         typename RemindersList = InitRList, bool doExit = false>
struct GenPrimesFormula;

// Next loop step over RList
template<int_t Limit, int_t Q, int_t K, typename InitRList, typename CheckFactors, 
typename H, typename Tail>
struct GenPrimesFormula<Limit,Q,K,InitRList,CheckFactors,Loki::Typelist<H,Tail>,false>
{
  static const int_t candidate = Q*K + H::value;
  typedef typename GenPrimesFormula<Limit, Q, K, InitRList, CheckFactors, Tail, (candidate > Limit)>::Result nextIter;
  typedef typename Loki::Select<(candidate <= Limit && isNotDivisibleByListMembers<candidate, CheckFactors>::value), 
          Loki::Typelist<sint<candidate>,nextIter>, nextIter>::Result Result;
};

// Next loop step over K
template<int_t Limit, int_t Q, int_t K, typename InitRList, typename CheckFactors>
struct GenPrimesFormula<Limit,Q,K,InitRList,CheckFactors,Loki::NullType,false>
{
  static const int_t candidate = Q*(K+1) + 1;
  typedef typename GenPrimesFormula<Limit, Q, K+1, InitRList, CheckFactors, InitRList, (K+1 == InitRList::Head::value)>::Result nextIter;
  typedef typename Loki::Select<(candidate <= Limit && (K+1 < InitRList::Head::value) && isNotDivisibleByListMembers<candidate, CheckFactors>::value), 
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



template<int_t Q, typename List, int_t N>
struct FilterRemindersList;

template<int_t Q, typename H, typename Tail, int_t N>
struct FilterRemindersList<Q, Loki::Typelist<H, Tail>, N>
{
  static const bool C = (N*H::value < Q);
  typedef FilterRemindersList<Q,Tail,N> Next;
  typedef typename Loki::Select<C, Loki::Typelist<H,typename Next::ExcludedPrimes>, Loki::NullType>::Result ExcludedPrimes;
  typedef typename Loki::Select<C, typename Next::CheckPrimes, Loki::Typelist<H, Tail> >::Result CheckPrimes;
};

template<int_t Q, int_t N>
struct FilterRemindersList<Q, Loki::NullType, N>
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
struct GetFirstNumberInList
{
  static const int_t value = T::Head::value;
};

template<>
struct GetFirstNumberInList<Loki::NullType>
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
  typedef FilterRemindersList<NextQ,typename RList::Tail,GetFirstNumberInList<typename RList::Tail>::value> Filter;
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

// Generate all primes less equal Limit with a constant Q in the formula Q*K+R
// Similar to GenPrimesFormula, but without second exit condition
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


template<typename List>
struct CheckPrimesListAtRunTime;

template<typename H, typename T>
struct CheckPrimesListAtRunTime<Loki::Typelist<H,T> >
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
    return CheckPrimesListAtRunTime<T>::apply();
  }
};

template<>
struct CheckPrimesListAtRunTime<Loki::NullType>
{
  static bool apply() { return true; }
};

#endif /*__metaprimes_h*/
