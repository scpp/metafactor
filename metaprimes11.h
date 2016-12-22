/***************************************************************************
 *   Copyright (C) 2015 by Vladimir Mirnyy, blog.scpp.eu                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the MIT License                                 *
 ***************************************************************************/

#ifndef __metaprimes11_h
#define __metaprimes11_h

/** \file
    \brief Primes generation meta-algorithms using variadic templates
*/

#include <cmath>

#include "metafactorcommon.h"
#include "metaprimescommon.h"

#include "cexpr.h"



// Returns true, if Candidate is not divisible by any entry of TrialList
template<ulong_t Candidate, typename TrialList, bool doExit = false>
struct isNotDivisibleByListMembers;

template<ulong_t Candidate, typename H, typename ...Tail>
struct isNotDivisibleByListMembers<Candidate,typelist<H,Tail...>,false>
{
  static const bool value = (Candidate % H::value != 0) && 
         isNotDivisibleByListMembers<Candidate,typelist<Tail...>,(H::value*H::value >= Candidate)>::value;
};

template<ulong_t Candidate>
struct isNotDivisibleByListMembers<Candidate,typelist<>,false>
{
  static const bool value = true;
};

template<ulong_t Candidate, typename H, typename ...Tail>
struct isNotDivisibleByListMembers<Candidate,typelist<H,Tail...>,true>
{
  static const bool value = true;
};

template<ulong_t Candidate>
struct isNotDivisibleByListMembers<Candidate,typelist<>,true>
{
  static const bool value = true;
};

// Generate all primes less equal Limit with a constant Q in the formula Q*K+R
// Second exit criteria: (K+1 == InitRList::Head::value),
// is needed to exit the loop to go over to the next  Q
template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckFactors,
         typename RemindersList = InitRList, bool doExit = false>
struct GenPrimesFormula;

// Next loop step over RList
template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckFactors, typename H, typename ...Tail>
struct GenPrimesFormula<Limit,Q,K,InitRList,CheckFactors,typelist<H,Tail...>,false>
{
  static const ulong_t candidate = Q*K + H::value;
  typedef typename GenPrimesFormula<Limit, Q, K, InitRList, CheckFactors, typelist<Tail...>, (candidate > Limit)>::type nextIter;
  typedef typename std::conditional<(candidate <= Limit && isNotDivisibleByListMembers<candidate, CheckFactors>::value), 
          typename typelist_cat<ulong_<candidate>,nextIter>::type, nextIter>::type type;
};

// Next loop step over K
template<ulong_t Limit, ulong_t Q, ulong_t K, typename Head, typename ...List, typename CheckFactors>
struct GenPrimesFormula<Limit,Q,K,typelist<Head,List...>,CheckFactors,typelist<>,false>
{
  typedef typelist<Head,List...> InitRList;
  static const ulong_t candidate = Q*(K+1) + 1;
  typedef typename GenPrimesFormula<Limit, Q, K+1, InitRList, CheckFactors, InitRList, (K+1 == Head::value)>::type nextIter;
  typedef typename std::conditional<(candidate <= Limit && (K+1 < Head::value) && isNotDivisibleByListMembers<candidate, CheckFactors>::value), 
          typename typelist_cat<ulong_<candidate>,nextIter>::type, nextIter>::type type;
};

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckFactors, typename H, typename ...Tail>
struct GenPrimesFormula<Limit,Q,K,InitRList,CheckFactors,typelist<H,Tail...>,true>
{
  typedef typelist<> type;
};

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckFactors>
struct GenPrimesFormula<Limit,Q,K,InitRList,CheckFactors,typelist<>,true>
{
  typedef typelist<> type;
};




template<typename List, ulong_t N>
struct EliminateNonPrimesLoop;

template<typename H, typename ...Tail, ulong_t N>
struct EliminateNonPrimesLoop<typelist<H, Tail...>, N>
{
  typedef typename EliminateNonPrimesLoop<typelist<Tail...>,N>::type Next;
  typedef typename std::conditional<(H::value%N != 0), typename typelist_cat<H,Next>::type, Next>::type type;
};

template<ulong_t N>
struct EliminateNonPrimesLoop<typelist<>,N>
{
  typedef typelist<> type;
};


template<typename List, typename PossibleFactors>
struct EliminateNonPrimes;

template<typename List, typename H, typename ...Tail>
struct EliminateNonPrimes<List, typelist<H, Tail...> >
{
  typedef typename EliminateNonPrimesLoop<List,H::value>::type NewList;
  typedef typename EliminateNonPrimes<NewList,typelist<Tail...>>::type type;
};

template<typename List>
struct EliminateNonPrimes<List, typelist<>>
{
  typedef List type;
};


typedef typelist<ulong_<2>,ulong_<3>,ulong_<5>> typelist235;


//////////////////////////////////////////////////////////////////////////////////////////

/// Iterates over all reminders in RList for given Q and K
template<ulong_t Limit, ulong_t Q, ulong_t K, typename RList,
         typename CheckPrimeCandidate, bool doExit = false>
struct GeneratePrimesRLoop;

template<ulong_t Limit, ulong_t Q, ulong_t K, typename H, typename ...Tail, typename CheckPrimeCandidate>
struct GeneratePrimesRLoop<Limit,Q,K,typelist<H,Tail...>,CheckPrimeCandidate,false>
{
  static const ulong_t candidate = Q*K + H::value;
  typedef typename GeneratePrimesRLoop<Limit, Q, K, typelist<Tail...>, CheckPrimeCandidate, (candidate > Limit)>::type nextIter;
  typedef typename std::conditional<(candidate <= Limit && CheckPrimeCandidate::value(candidate)),
          typename typelist_cat<ulong_<candidate>,nextIter>::type, nextIter>::type type;
};

template<ulong_t Limit, ulong_t Q, ulong_t K, typename CheckPrimeCandidate>
struct GeneratePrimesRLoop<Limit,Q,K,typelist<>,CheckPrimeCandidate,false>
{ typedef typelist<> type; };

template<ulong_t Limit, ulong_t Q, ulong_t K, typename H, typename ...Tail, typename CheckPrimeCandidate>
struct GeneratePrimesRLoop<Limit,Q,K,typelist<H,Tail...>,CheckPrimeCandidate,true>
{ typedef typelist<> type; };

template<ulong_t Limit, ulong_t Q, ulong_t K, typename CheckPrimeCandidate>
struct GeneratePrimesRLoop<Limit,Q,K,typelist<>,CheckPrimeCandidate,true>
{ typedef typelist<> type; };


// Generate all primes less equal Limit with a constant Q in the formula Q*K+R
// Similar to GenPrimesFormula, but without second exit condition,
// that means the same formula is used up to the Limit
template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList,
         typename CheckPrimeCandidate, bool doExit = false>
struct GeneratePrimes;

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,false>
{
  typedef typename GeneratePrimesRLoop<Limit, Q, K, InitRList, CheckPrimeCandidate>::type List;
  typedef typename GeneratePrimes<Limit, Q, K+1, InitRList, CheckPrimeCandidate, (Q*(K+1) >= Limit)>::type nextIter;
  typedef typename typelist_cat<List,nextIter>::type type;
};

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,true>
{ typedef typelist<> type; };


//////////////////////////////////////////////////////////////////////////////////////////

/// Class templates under this namespace generate single deep recursion.
/// Increasing K or taking the next reminder will go into the next recursion level.
/// The algorithms here are not used anymore and remain as an example, because
/// compilers still have difficulties with very deep template recursions taking much CPU-time and RAM
namespace SingleRecursion {

// Generate all primes less equal Limit with a constant Q in the formula Q*K+R
// Similar to GenPrimesFormula, but without second exit condition,
// that means the same formula is used up to the Limit
template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList,
         typename CheckPrimeCandidate, typename RList = InitRList, bool doExit = false>
struct GeneratePrimes;

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename H, typename ...Tail, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,typelist<H,Tail...>,false>
{
  static const ulong_t candidate = Q*K + H::value;
  typedef typename GeneratePrimes<Limit, Q, K, InitRList, CheckPrimeCandidate, typelist<Tail...>, (candidate > Limit)>::type nextIter;
  typedef typename std::conditional<(candidate <= Limit && CheckPrimeCandidate::value(candidate)),
          typename typelist_cat<ulong_<candidate>,nextIter>::type, nextIter>::type type;
};

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,typelist<>,false>
: public GeneratePrimes<Limit, Q, K+1, InitRList, CheckPrimeCandidate> {};

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename H, typename ...Tail, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,typelist<H,Tail...>,true>
{ typedef typelist<> type; };

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,typelist<>,true>
{ typedef typelist<> type; };

} // SingleRecursion


namespace F6K {


/////////////////////////////////////////////////
template<ulong_t Limit>
struct GeneratePrimesWithList
{
  typedef typelist<ulong_<5>> List1;
  typedef typelist<ulong_<1>,ulong_<5>> RList;

//  typedef typename GeneratePrimes<Limit,6,1,RList,cexpr::IsPrime6<cexpr::CheckSmallPrimes5> >::type PrimesList;
  typedef typename GeneratePrimes<Limit,6,1,RList,cexpr::IsPrime30<cexpr::CheckSmallPrimes5_29> >::type PrimesList;
  typedef typename typelist_cat<typelist235,PrimesList>::type type;
};

} // F6K


namespace F30K {

/////////////////////////////////////////////////
template<ulong_t Limit>
struct GeneratePrimesWithList
{
  typedef typelist<ulong_<7>,ulong_<11>,ulong_<13>,ulong_<17>,ulong_<19>,ulong_<23>,ulong_<29>> List1;
  typedef typename typelist_cat<ulong_<1>,List1>::type RList;

#if (__cplusplus >= 201402L)  // c++14
  typedef typename GeneratePrimes<Limit,30,1,RList,cexpr::IsPrime30<cexpr::CheckSmallPrimes7_29> >::type PrimesList;
#else  // c++11
  typedef typename GeneratePrimes<Limit,30,1,RList,IsPrime30<CheckSmallPrimes7_29> >::type PrimesList;
#endif
  /// could be also checked with IsPrime6 instead of IsPrime30
  //   typedef typename GeneratePrimes<Limit,30,1,RList,IsPrime6<CheckNone> >::type PrimesList;

  typedef typename typelist_cat<typelist235,List1>::type List2;
  typedef typename typelist_cat<List2, PrimesList>::type type;
};

}  // F30K


namespace F210K {

template<ulong_t Limit>
struct GeneratePrimesWithList
{
  typedef typelist<ulong_<11>,ulong_<13>,ulong_<17>,ulong_<19>,ulong_<23>,ulong_<29>> List1;
  // Generate list containing primes and divisible by 11 and 13
  // The formula 30k+r is used, but from generated numbers only divisible by 7 are eliminated
  typedef typename typelist_cat<typelist<ulong_<1>,ulong_<7>>,List1>::type RemindersListForFormula30k;
  typedef typename GenPrimesFormula<210,30,1,RemindersListForFormula30k,typelist<ulong_<7>>>::type NewList;
  typedef typename typelist_cat<typename typelist_cat<typelist<ulong_<1>>,List1>::type, NewList>::type RemindersListForFormula210k;
  typedef typename EliminateNonPrimes<NewList,typelist<ulong_<11>,ulong_<13>>>::type PList;

  // Creates a list of primes starting with 211 and not exceeding Limit
#if (__cplusplus >= 201402L)  // c++14
  typedef typename GeneratePrimes<Limit,210,1,RemindersListForFormula210k,cexpr::IsPrime30<cexpr::CheckSmallPrimes11_29> >::type PrimesList;
#else  // c++11
    typedef typename GeneratePrimes<Limit,210,1,RemindersListForFormula210k,IsPrime30<CheckSmallPrimes11_29> >::type PrimesList;
#endif
    //   typedef typename GeneratePrimes<Limit,210,1,RemindersListForFormula210k,IsPrime6<CheckNone> >::type PrimesList;

  // Add primes less than 211
  typedef typename typelist_cat<typelist<ulong_<2>,ulong_<3>,ulong_<5>,ulong_<7>>,List1>::type List2;
  typedef typename typelist_cat<List2, PList>::type List3;
  typedef typename typelist_cat<List3, PrimesList>::type type;
};


}  // F210K


namespace Hierarchic {

// The algorithm under this namespace is a nice and working hierarchic implementation of the prime numbers
// generation with increasing Q. But it is useless for practical applications, since the formula with Q>210
// has too many reminders R and becomes too slow and memory consuming


////////////////////////////////////////////////////
template<ulong_t Q, typename List, typename N>
struct FilterRemindersList;

template<ulong_t Q, typename H, typename ...Tail, typename N>
struct FilterRemindersList<Q, typelist<H, Tail...>, N>
{
  static const bool C = (N::value*H::value < Q);
  typedef FilterRemindersList<Q,typelist<Tail...>,N> Next;
  typedef typename std::conditional<C, typename typelist_cat<H,typename Next::ExcludedPrimes>::type, typelist<>>::type ExcludedPrimes;
  typedef typename std::conditional<C, typename Next::CheckPrimes, typelist<H, Tail...>>::type CheckPrimes;
};

template<ulong_t Q, typename N>
struct FilterRemindersList<Q, typelist<>, N>
{
  typedef typelist<> ExcludedPrimes;
  typedef typelist<> CheckPrimes;
};

template<ulong_t Q>
struct FilterRemindersList<Q, typelist<>, typelist<>>
{
  typedef typelist<> ExcludedPrimes;
  typedef typelist<> CheckPrimes;
};


////////////////////////////////////////////////////
template<ulong_t Limit, ulong_t StartQ=6,
typename StartRList = typelist<ulong_<5>>, bool doExit = false>
struct GenPrimes;

template<ulong_t Limit, ulong_t Q, typename H, typename ...Tail>
struct GenPrimes<Limit,Q,typelist<H,Tail...>,false>
{
  static const ulong_t NextQ = Q*H::value;
  typedef FilterRemindersList<NextQ,typelist<Tail...>,typename get_first<typelist<Tail...>>::type> Filter;
  typedef typename typelist_cat<H, typename Filter::CheckPrimes>::type PrimesToCheck;
  typedef typename typelist_cat<ulong_<Q+1>,   // Q+1 is always prime and should not be checked
    typename GenPrimesFormula<(NextQ<Limit) ? NextQ : Limit,Q,1,typelist<H,Tail...>,PrimesToCheck>::type>::type NewList;
  typedef typename EliminateNonPrimes<NewList,typename Filter::ExcludedPrimes>::type PrimesList;
  typedef typename typelist_cat<typelist<Tail...>, NewList>::type NextRList;
  typedef typename GenPrimes<Limit, NextQ, NextRList, (NextQ+1 > Limit)>::type NextIter;
  typedef typename typelist_cat<PrimesList,NextIter>::type type;
};

template<ulong_t Limit, ulong_t Q, typename RList>
struct GenPrimes<Limit,Q,RList,true>
{
  typedef typelist<> type;
};


template<ulong_t Limit>
struct GeneratePrimesHierarchically
{
    typedef typename typelist_cat<typelist235, typename GenPrimes<Limit>::type>::type type;
};


} // Hierarchic


template<typename List>
struct CheckPrimesListAtRunTime;

template<typename H, typename ...Tail>
struct CheckPrimesListAtRunTime<typelist<H,Tail...>>
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
    return CheckPrimesListAtRunTime<typelist<Tail...>>::apply();
  }
};

template<>
struct CheckPrimesListAtRunTime<typelist<>>
{
  static bool apply() { return true; }
};

#endif /*__metaprimes_h*/
