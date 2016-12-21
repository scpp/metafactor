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


// Returns true, if Candidate is not divisible by any entry of TrialList
template<ulong_t Candidate, typename TrialList, bool doExit = false>
struct isNotDivisibleByListMembers;

template<ulong_t Candidate, typename H, typename Tail>
struct isNotDivisibleByListMembers<Candidate,Loki::Typelist<H,Tail>,false>
{
  static const bool value = (Candidate % H::value != 0) && 
         isNotDivisibleByListMembers<Candidate,Tail,(H::value*H::value >= Candidate)>::value;
};

template<ulong_t Candidate>
struct isNotDivisibleByListMembers<Candidate,Loki::NullType,false>
{
  static const bool value = true;
};

template<ulong_t Candidate, typename H, typename Tail>
struct isNotDivisibleByListMembers<Candidate,Loki::Typelist<H,Tail>,true>
{
  static const bool value = true;
};

template<ulong_t Candidate>
struct isNotDivisibleByListMembers<Candidate,Loki::NullType,true>
{
  static const bool value = true;
};

// Generate all primes less equal Limit with a constant Q in the formula Q*K+R
// Second exit criteria: (K+1 == InitRList::Head::value)
// is needed to exit the loop to go over to the next  Q
template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckFactors,
         typename RemindersList = InitRList, bool doExit = false>
struct GenPrimesFormula;

// Next loop step over RList
template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckFactors,
typename H, typename Tail>
struct GenPrimesFormula<Limit,Q,K,InitRList,CheckFactors,Loki::Typelist<H,Tail>,false>
{
  static const ulong_t candidate = Q*K + H::value;
  typedef typename GenPrimesFormula<Limit, Q, K, InitRList, CheckFactors, Tail, (candidate > Limit)>::Result nextIter;
  typedef typename Loki::Select<(candidate <= Limit && isNotDivisibleByListMembers<candidate, CheckFactors>::value), 
          Loki::Typelist<ulong_<candidate>,nextIter>, nextIter>::Result Result;
};

// Next loop step over K
template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckFactors>
struct GenPrimesFormula<Limit,Q,K,InitRList,CheckFactors,Loki::NullType,false>
{
  static const ulong_t candidate = Q*(K+1) + 1;
  typedef typename GenPrimesFormula<Limit, Q, K+1, InitRList, CheckFactors, InitRList, (K+1 == InitRList::Head::value)>::Result nextIter;
  typedef typename Loki::Select<(candidate <= Limit && (K+1 < InitRList::Head::value) && isNotDivisibleByListMembers<candidate, CheckFactors>::value), 
          Loki::Typelist<ulong_<candidate>,nextIter>, nextIter>::Result Result;
};

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckFactors, typename H, typename Tail>
struct GenPrimesFormula<Limit,Q,K,InitRList,CheckFactors,Loki::Typelist<H,Tail>,true>
{
  typedef Loki::NullType Result;
};

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckFactors>
struct GenPrimesFormula<Limit,Q,K,InitRList,CheckFactors,Loki::NullType,true>
{
  typedef Loki::NullType Result;
};



template<typename List, ulong_t N>
struct EliminateNonPrimesLoop;

template<typename H, typename Tail, ulong_t N>
struct EliminateNonPrimesLoop<Loki::Typelist<H, Tail>, N>
{
  typedef typename EliminateNonPrimesLoop<Tail,N>::Result Next;
  typedef typename Loki::Select<(H::value%N != 0), Loki::Typelist<H,Next>, Next>::Result Result;
};

template<ulong_t N>
struct EliminateNonPrimesLoop<Loki::NullType,N>
{
  typedef Loki::NullType Result;
};


//////////////////////////////////////////////////////////
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



//////////////////////////////////////////////////////////////////////////////////////////

// Generate all primes less equal Limit with a constant Q in the formula Q*K+R
// Similar to GenPrimesFormula, but without second exit condition,
// that means the same formula is used up to the Limit
template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList,
         typename CheckPrimeCandidate = IsPrime6<>,
         typename RList = InitRList, bool doExit = false>
struct GeneratePrimes;

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename H, typename Tail, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,Loki::Typelist<H,Tail>,false>
{
  static const ulong_t candidate = Q*K + H::value;
  typedef typename GeneratePrimes<Limit, Q, K, InitRList, CheckPrimeCandidate, Tail, (candidate > Limit)>::Result nextIter;
  typedef typename Loki::Select<(candidate <= Limit && CheckPrimeCandidate::template Do<candidate>::value), 
          Loki::Typelist<ulong_<candidate>,nextIter>, nextIter>::Result Result;
};

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,Loki::NullType,false>
: public GeneratePrimes<Limit, Q, K+1, InitRList> {};

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename H, typename Tail, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,Loki::Typelist<H,Tail>,true>
{ typedef Loki::NullType Result; };

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,Loki::NullType,true>
{ typedef Loki::NullType Result; };




namespace F6K {

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
    Loki::Typelist<ulong_<PrimeCandidate1>, Loki::Typelist<ulong_<PrimeCandidate2>, NextIter> >,
    typename Loki::Select<p1, Loki::Typelist<ulong_<PrimeCandidate1>, NextIter>,
    typename Loki::Select<p2, Loki::Typelist<ulong_<PrimeCandidate2>, NextIter>, NextIter>::Result>::Result>::Result Result;
};

template<int Start, int Limit, int K>
struct NextPrimesDirect<Start, Limit, K, false>
{
  typedef Loki::NullType Result;
};


/////////////////////////////////////////////
template<ulong_t Limit>
struct GeneratePrimesDirect
{
  typedef typename NextPrimesDirect<0,Limit,1>::Result PrimesList;
  typedef typename Loki::Typelist<ulong_<2>,Loki::Typelist<ulong_<3>,Loki::Typelist<ulong_<5>,PrimesList> > > Result;
};


/////////////////////////////////////////////
template<ulong_t Limit>
struct GeneratePrimesWithList
{
  typedef TYPELIST_1(ulong_<5>) List1;
  typedef Loki::Typelist<ulong_<1>,List1> RList;

//   typedef typename GeneratePrimes<Limit,6,1,RList,IsPrime6<CheckSmallPrimes5> >::Result PrimesList;
  typedef typename GeneratePrimes<Limit,6,1,RList,IsPrime30<CheckSmallPrimes13_29> >::Result PrimesList;
  typedef Loki::Typelist<ulong_<2>,Loki::Typelist<ulong_<3>,Loki::Typelist<ulong_<5>,PrimesList> > > Result;
};

}  // F6K


namespace F30K {

// Experimental code to achieve shorter compile-times comparing to GenPrimes
template<int Limit, int K = 1, int C = (30*K + 1 <= Limit)>
struct NextPrimesDirect;

template<int Limit, int K>
struct NextPrimesDirect<Limit, K, true>
{
  typedef IsPrime30<CheckSmallPrimes7_29> isPrime;
  static const ulong_t C1 = 30*K + 1;
  static const ulong_t C2 = 30*K + 7;
  static const ulong_t C3 = 30*K + 11;
  static const ulong_t C4 = 30*K + 13;
  static const ulong_t C5 = 30*K + 17;
  static const ulong_t C6 = 30*K + 19;
  static const ulong_t C7 = 30*K + 23;
  static const ulong_t C8 = 30*K + 29;
  static const bool p1 = isPrime::Do<C1>::value;
  static const bool p2 = isPrime::Do<C2>::value && (C2 <= Limit);
  static const bool p3 = isPrime::Do<C3>::value && (C3 <= Limit);
  static const bool p4 = isPrime::Do<C4>::value && (C4 <= Limit);
  static const bool p5 = isPrime::Do<C5>::value && (C5 <= Limit);
  static const bool p6 = isPrime::Do<C6>::value && (C6 <= Limit);
  static const bool p7 = isPrime::Do<C7>::value && (C7 <= Limit);
  static const bool p8 = isPrime::Do<C8>::value && (C8 <= Limit);
  typedef typename NextPrimesDirect<Limit, K+1>::Result NextIter;

  typedef typename AddType<ulong_<C8>,p8,NextIter>::Result R8;
  typedef typename AddType<ulong_<C7>,p7,R8>::Result R7;
  typedef typename AddType<ulong_<C6>,p6,R7>::Result R6;
  typedef typename AddType<ulong_<C5>,p5,R6>::Result R5;
  typedef typename AddType<ulong_<C4>,p4,R5>::Result R4;
  typedef typename AddType<ulong_<C3>,p3,R4>::Result R3;
  typedef typename AddType<ulong_<C2>,p2,R3>::Result R2;
  typedef typename AddType<ulong_<C1>,p1,R2>::Result Result;
};

template<int Limit, int K>
struct NextPrimesDirect<Limit, K, false>
{
  typedef Loki::NullType Result;
};


/////////////////////////////////////////////
template<ulong_t Limit>
struct GeneratePrimesDirect
{
  typedef TYPELIST_10(ulong_<2>,ulong_<3>,ulong_<5>,ulong_<7>,ulong_<11>,ulong_<13>,ulong_<17>,ulong_<19>,ulong_<23>,ulong_<29>) List1;
  typedef typename NextPrimesDirect<Limit>::Result PrimesList;
  typedef typename Loki::TL::Append<List1, PrimesList>::Result Result;
};


/////////////////////////////////////////////
template<ulong_t Limit>
struct GeneratePrimesWithList
{
  typedef TYPELIST_7(ulong_<7>,ulong_<11>,ulong_<13>,ulong_<17>,ulong_<19>,ulong_<23>,ulong_<29>) List1;
  typedef Loki::Typelist<ulong_<1>,List1> RList;

//   typedef typename GeneratePrimes<Limit,30,1,RList,IsPrime6<CheckNone> >::Result PrimesList;
  typedef typename GeneratePrimes<Limit,30,1,RList,IsPrime30<CheckNone> >::Result PrimesList;
  typedef Loki::Typelist<ulong_<2>,Loki::Typelist<ulong_<3>,Loki::Typelist<ulong_<5>,List1> > > List2;
  typedef typename Loki::TL::Append<List2, PrimesList>::Result Result;
};

}  // F30K


namespace F210K {

template<ulong_t Limit>
struct GeneratePrimesWithList
{
  typedef TYPELIST_6(ulong_<11>,ulong_<13>,ulong_<17>,ulong_<19>,ulong_<23>,ulong_<29>) List1;
  // Generate list containing primes and divisible by 11 and 13
  // The formula 30k+r is used, but from generated numbers only divisible by 7 are eliminated
  typedef Loki::Typelist<ulong_<1>,Loki::Typelist<ulong_<7>,List1> > RemindersListForFormula30k;
  typedef typename GenPrimesFormula<210,30,1,RemindersListForFormula30k,TYPELIST_1(ulong_<7>)>::Result NewList;
  typedef typename Loki::TL::Append<Loki::Typelist<ulong_<1>,List1>, NewList>::Result RemindersListForFormula210k;
  typedef typename EliminateNonPrimes<NewList,TYPELIST_2(ulong_<11>,ulong_<13>)>::Result PList;

  // Creates a list of primes starting with 211 and not exceeding Limit
//   typedef typename GeneratePrimes<Limit,210,1,RList,IsPrime6<CheckNone> >::Result PrimesList;
  typedef typename GeneratePrimes<Limit,210,1,RemindersListForFormula210k,IsPrime30<CheckSmallPrimes11_29> >::Result PrimesList;

  // Add primes less than 211
  typedef Loki::Typelist<ulong_<2>,Loki::Typelist<ulong_<3>,Loki::Typelist<ulong_<5>,Loki::Typelist<ulong_<7>,List1> > > > List2;
  typedef typename Loki::TL::Append<List2, PList>::Result List3;
  typedef typename Loki::TL::Append<List3, PrimesList>::Result Result;
};

} // F210K


namespace Hierarchic {

// The algorithm under this namespace is a nice and working hierarchic implementation of the prime numbers
// generation with increasing Q. But it is useless for practical applications, since the formula with Q>210
// has too many reminders R and becomes too slow and memory consuming

//////////////////////////////////////////////////////////
template<ulong_t Q, typename List, ulong_t N>
struct FilterRemindersList;

template<ulong_t Q, typename H, typename Tail, ulong_t N>
struct FilterRemindersList<Q, Loki::Typelist<H, Tail>, N>
{
  static const bool C = (N*H::value < Q);
  typedef FilterRemindersList<Q,Tail,N> Next;
  typedef typename Loki::Select<C, Loki::Typelist<H,typename Next::ExcludedPrimes>, Loki::NullType>::Result ExcludedPrimes;
  typedef typename Loki::Select<C, typename Next::CheckPrimes, Loki::Typelist<H, Tail> >::Result CheckPrimes;
};

template<ulong_t Q, ulong_t N>
struct FilterRemindersList<Q, Loki::NullType, N>
{
  typedef Loki::NullType ExcludedPrimes;
  typedef Loki::NullType CheckPrimes;
};


//////////////////////////////////////////////////////////
template<typename T>
struct GetFirstNumberInList
{
  static const ulong_t value = T::Head::value;
};

template<>
struct GetFirstNumberInList<Loki::NullType>
{
  static const ulong_t value = 1;
};


//////////////////////////////////////////////////////////
template<ulong_t Limit, ulong_t StartQ=6,
typename StartRList = TYPELIST_1(ulong_<5>), bool doExit = false>
struct GenPrimes;

template<ulong_t Limit, ulong_t Q, typename RList>
struct GenPrimes<Limit,Q,RList,false>
{
  static const ulong_t NextQ = Q*RList::Head::value;
  typedef FilterRemindersList<NextQ,typename RList::Tail,GetFirstNumberInList<typename RList::Tail>::value> Filter;
  typedef Loki::Typelist<typename RList::Head, typename Filter::CheckPrimes> PrimesToCheck;
  typedef Loki::Typelist<ulong_<Q+1>,   // Q+1 is always prime and should not be checked ???
    typename GenPrimesFormula<(NextQ<Limit) ? NextQ : Limit,Q,1,RList,PrimesToCheck>::Result> NewList;
  typedef typename EliminateNonPrimes<NewList,typename Filter::ExcludedPrimes>::Result PrimesList;
  typedef typename Loki::TL::Append<typename RList::Tail, NewList>::Result NextRList;
  typedef typename GenPrimes<Limit, NextQ, NextRList, (NextQ+1 > Limit)>::Result NextIter;
  typedef typename Loki::TL::Append<PrimesList,NextIter>::Result Result;
};

template<ulong_t Limit, ulong_t Q, typename RList>
struct GenPrimes<Limit,Q,RList,true>
{
  typedef Loki::NullType Result;
};


template<ulong_t Limit>
struct GeneratePrimesHierarchically
{
    typedef Loki::Typelist<ulong_<2>, Loki::Typelist<ulong_<3>, Loki::Typelist<ulong_<5>, typename GenPrimes<Limit>::Result> > > Result;
};

} // Hierarchic



//////////////////////////////////////////////////////////
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
