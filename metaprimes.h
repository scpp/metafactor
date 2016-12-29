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
  typedef typename Loki::Select<(H::value%N != 0 || H::value == N), Loki::Typelist<H,Next>, Next>::Result Result;
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



//////////////////////////////////////////////////////////
template<ulong_t Limit, typename List, ulong_t Counter=0, bool doExit = false>
struct EliminateNonPrimesWithFactor;

template<ulong_t Limit, typename List, ulong_t Counter>
struct EliminateNonPrimesWithFactor<Limit, List, Counter, false>
{
  typedef typename Loki::TL::TypeAt<List, Counter>::Result Factor;
  static const ulong_t N = Factor::value;
  typedef typename EliminateNonPrimesLoop<List, N>::Result NewList;
  typedef typename EliminateNonPrimesWithFactor<Limit,NewList,Counter+1,(N*N > Limit)>::Result Result;
};

template<ulong_t Limit, typename List, ulong_t Counter>
struct EliminateNonPrimesWithFactor<Limit, List, Counter, true>
{
    typedef List Result;
};


//////////////////////////////////////////////////////////////////////////////////////////

/// Iterates over all reminders in RList for given Q and K
template<ulong_t Limit, ulong_t Q, ulong_t K, typename RList,
         typename CheckPrimeCandidate, bool doExit = false>
struct GeneratePrimesRLoop;

template<ulong_t Limit, ulong_t Q, ulong_t K, typename H, typename Tail, typename CheckPrimeCandidate>
struct GeneratePrimesRLoop<Limit,Q,K,Loki::Typelist<H,Tail>,CheckPrimeCandidate,false>
{
  static const ulong_t candidate = Q*K + H::value;
  typedef typename GeneratePrimesRLoop<Limit, Q, K, Tail, CheckPrimeCandidate, (candidate > Limit)>::Result nextIter;
  typedef typename Loki::Select<(candidate <= Limit && CheckPrimeCandidate::template Do<candidate>::value),
          Loki::Typelist<ulong_<candidate>,nextIter>, nextIter>::Result Result;
};

template<ulong_t Limit, ulong_t Q, ulong_t K, typename CheckPrimeCandidate>
struct GeneratePrimesRLoop<Limit,Q,K,Loki::NullType,CheckPrimeCandidate,false>
{ typedef Loki::NullType Result; };

template<ulong_t Limit, ulong_t Q, ulong_t K, typename H, typename Tail, typename CheckPrimeCandidate>
struct GeneratePrimesRLoop<Limit,Q,K,Loki::Typelist<H,Tail>,CheckPrimeCandidate,true>
{ typedef Loki::NullType Result; };

template<ulong_t Limit, ulong_t Q, ulong_t K, typename CheckPrimeCandidate>
struct GeneratePrimesRLoop<Limit,Q,K,Loki::NullType,CheckPrimeCandidate,true>
{ typedef Loki::NullType Result; };


// Generate all primes less equal Limit with a constant Q in the formula Q*K+R
// Similar to GenPrimesFormula, but without second exit condition,
// that means the same formula is used up to the Limit
template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList,
         typename CheckPrimeCandidate, bool doExit = false>
struct GeneratePrimes;

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,false>
{
  typedef typename GeneratePrimesRLoop<Limit, Q, K, InitRList, CheckPrimeCandidate>::Result List;
  typedef typename GeneratePrimes<Limit, Q, K+1, InitRList, CheckPrimeCandidate, (Q*(K+1) >= Limit)>::Result nextIter;
  typedef typename Loki::TL::Append<List,nextIter>::Result Result;
};

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,true>
{ typedef Loki::NullType Result; };




//////////////////////////////////////////////////////////////////////////////////////////

/// Class templates under this namespace generate single deep recursion.
/// Increasing K or taking the next reminder will go into the next recursion level.
/// The algorithms here are not used anymore and remain as an example, because
/// compilers still have difficulties with very deep template recursions taking much CPU-time and RAM
namespace SingleRecursion {


// Generate all primes less equal Limit with a constant Q in the formula Q*K+R
// Similar to GenPrimesFormula, but without second exit condition,
// that means the same formula is used up to the Limit
template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckPrimeCandidate,
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
: public GeneratePrimes<Limit, Q, K+1, InitRList, CheckPrimeCandidate> {};

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename H, typename Tail, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,Loki::Typelist<H,Tail>,true>
{ typedef Loki::NullType Result; };

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,Loki::NullType,true>
{ typedef Loki::NullType Result; };

}  // SingleRecursion


namespace F6K {


/////////////////////////////////////////////
template<ulong_t Limit>
struct GeneratePrimesWithList
{
  typedef TYPELIST_1(ulong_<5>) List1;
  typedef Loki::Typelist<ulong_<1>,List1> RList;

//  typedef typename GeneratePrimes<Limit,6,1,RList,IsPrime6<CheckSmallPrimes5> >::Result PrimesList;
  typedef typename GeneratePrimes<Limit,6,1,RList,IsPrime30<CheckSmallPrimes5_29> >::Result PrimesList;
  typedef Loki::Typelist<ulong_<2>,Loki::Typelist<ulong_<3>,Loki::Typelist<ulong_<5>,PrimesList> > > Result;
};


template<ulong_t Limit, ulong_t K = 1, bool doExit = false>
struct GenerateInitCandidateList;

template<ulong_t Limit, ulong_t K>
struct GenerateInitCandidateList<Limit, K, false>
{
    static const ulong_t candidate1 = 6*K+1;
    static const ulong_t candidate2 = 6*K+5;
    static const bool C = (candidate1 <= Limit && candidate2 <= Limit);
    typedef typename GenerateInitCandidateList<Limit, K+1, !C>::Result nextIter;
    typedef typename Loki::Select<C, Loki::Typelist<ulong_<candidate1>,Loki::Typelist<ulong_<candidate2>,nextIter> >,
            typename Loki::Select<(candidate1 <= Limit), Loki::Typelist<ulong_<candidate1>,nextIter>, nextIter>::Result>::Result Result;
};

template<ulong_t Limit, ulong_t K>
struct GenerateInitCandidateList<Limit, K, true>
{
    typedef Loki::NullType Result;
};


template<ulong_t Limit>
struct Sieve
{
    typedef typename GenerateInitCandidateList<Limit>::Result InitList;
    // EliminateNonPrimesWithFactor takes the first element as the first trial factor
    typedef Loki::Typelist<ulong_<5>,InitList> List;
    typedef typename EliminateNonPrimesWithFactor<Limit, List>::Result NewList;
    // complete list with 2 and 3
    typedef Loki::Typelist<ulong_<2>,Loki::Typelist<ulong_<3>,NewList>> Result;
};


}  // F6K


namespace F30K {

/////////////////////////////////////////////
template<ulong_t Limit>
struct GeneratePrimesWithList
{
  typedef TYPELIST_7(ulong_<7>,ulong_<11>,ulong_<13>,ulong_<17>,ulong_<19>,ulong_<23>,ulong_<29>) List1;
  typedef Loki::Typelist<ulong_<1>,List1> RList;

//   typedef typename GeneratePrimes<Limit,30,1,RList,IsPrime6<CheckNone> >::Result PrimesList;
  typedef typename GeneratePrimes<Limit,30,1,RList,IsPrime30<CheckSmallPrimes7_29> >::Result PrimesList;
  typedef Loki::Typelist<ulong_<2>,Loki::Typelist<ulong_<3>,Loki::Typelist<ulong_<5>,List1> > > List2;
  typedef typename Loki::TL::Append<List2, PrimesList>::Result Result;
};


template<ulong_t Limit, ulong_t K, typename List, bool doExit = false>
struct GenerateInitCandidateListLoop;


template<ulong_t Limit, ulong_t K>
struct GenerateInitCandidateList
{
    typedef TYPELIST_8(ulong_<1>, ulong_<7>, ulong_<11>, ulong_<13>, ulong_<17>, ulong_<19>, ulong_<23>, ulong_<29>) Reminders;
    typedef typename GenerateInitCandidateListLoop<Limit, K, Reminders>::Result Result;
};


template<ulong_t Limit, ulong_t K, typename H, typename Tail>
struct GenerateInitCandidateListLoop<Limit, K, Loki::Typelist<H,Tail>, false>
{
    static const ulong_t N = 30*K + H::value;
    static const bool C = (N <= Limit);
    typedef typename GenerateInitCandidateListLoop<Limit, K, Tail, !C>::Result nextIter;
    typedef typename Loki::Select<C, Loki::Typelist<ulong_<N>, nextIter>, nextIter>::Result Result;
};

template<ulong_t Limit, ulong_t K, typename H, typename Tail>
struct GenerateInitCandidateListLoop<Limit, K, Loki::Typelist<H,Tail>, true>
{
    typedef Loki::NullType Result;
};

template<ulong_t Limit, ulong_t K>
struct GenerateInitCandidateListLoop<Limit, K, Loki::NullType, false>
{
    typedef typename GenerateInitCandidateList<Limit, K+1>::Result Result;
};

template<ulong_t Limit, ulong_t K>
struct GenerateInitCandidateListLoop<Limit, K, Loki::NullType, true>
{
    typedef Loki::NullType Result;
};




template<ulong_t Limit>
struct Sieve
{
    typedef typename GenerateInitCandidateList<Limit, 1>::Result InitList;
    // EliminateNonPrimesWithFactor takes the first element as the first trial factor
    typedef Loki::Typelist<ulong_<7>,
            Loki::Typelist<ulong_<11>,
            Loki::Typelist<ulong_<13>,
            Loki::Typelist<ulong_<17>,
            Loki::Typelist<ulong_<19>,
            Loki::Typelist<ulong_<23>,
            Loki::Typelist<ulong_<29>, InitList> > > > > > > List;
    typedef typename EliminateNonPrimesWithFactor<Limit, List>::Result NewList;
//    // complete list with 2, 3, 5
    typedef Loki::Typelist<ulong_<2>, Loki::Typelist<ulong_<3>, Loki::Typelist<ulong_<5>, NewList> > > Result;
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
//  typedef typename GeneratePrimes<Limit,210,1,RemindersListForFormula210k,IsPrime6<CheckNone> >::Result PrimesList;
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
