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

#include "variadic_typelist.h"

// try to use c++11 constexpr functions
namespace cexpr {

struct CheckNone
{
    static constexpr const bool value(const ulong_t) { return true; }
};

struct CheckSmallPrimes5
{
    static constexpr const bool value(const ulong_t PrimeCandidate)
    {
        return (PrimeCandidate%5 != 0);
    }
};

struct CheckSmallPrimes2_5
{
    static constexpr const bool value(const ulong_t PrimeCandidate)
    {
        return (PrimeCandidate%2 != 0) && (PrimeCandidate%3 != 0) && (PrimeCandidate%5 != 0);
    }
};

struct CheckSmallPrimes13_29
{
    static constexpr const bool value(const ulong_t PrimeCandidate)
    {
        return (PrimeCandidate%13 != 0) && (PrimeCandidate%17 != 0)
            && (PrimeCandidate%19 != 0) && (PrimeCandidate%23 != 0) && (PrimeCandidate%29 != 0);
    }
};

struct CheckSmallPrimes11_29
{
    static constexpr const bool value(const ulong_t PrimeCandidate)
    {
        return (PrimeCandidate%11 != 0) && (PrimeCandidate%13 != 0) && (PrimeCandidate%17 != 0)
            && (PrimeCandidate%19 != 0) && (PrimeCandidate%23 != 0) && (PrimeCandidate%29 != 0);
    }
};

struct CheckSmallPrimes7_29
{
    static constexpr const bool value(const ulong_t PrimeCandidate)
    {
        return (PrimeCandidate%7 != 0) && cexpr::CheckSmallPrimes11_29::value(PrimeCandidate);
    }
};

struct CheckSmallPrimes2_29
{
    static constexpr const bool value(const ulong_t PrimeCandidate)
    {
        return cexpr::CheckSmallPrimes2_5::value(PrimeCandidate) && cexpr::CheckSmallPrimes7_29::value(PrimeCandidate);
    }
};

////////////////////////////////////////////////////////
struct IsPrime6loop
{
    static constexpr const bool value(const ulong_t PrimeCandidate, const int K=1)
    {
        const bool C1 = ((6*K+1)*(6*K+1) <= PrimeCandidate);
        const bool C2 = ((PrimeCandidate % (6*K+1) != 0) && (PrimeCandidate % (6*K+5) != 0));
        return (!C1 && !C2) ? ((K == 1) ? true : false) : ((C1 && C2) ? IsPrime6loop::value(PrimeCandidate, K+1) : (C1 ? false : true /* C2 */));
    }
};


template<class InitCheck = CheckSmallPrimes2_5>
struct IsPrime6
{
    static constexpr const bool value(const ulong_t PrimeCandidate)
    {
        return (InitCheck::value(PrimeCandidate) && cexpr::IsPrime6loop::value(PrimeCandidate));
    }
};


////////////////////////////////////////////////////////
struct IsPrime30loop
{
    static constexpr const bool value(const ulong_t PrimeCandidate, const int K=1)
    {
        const bool C1 = ((30*K+1)*(30*K+1) <= PrimeCandidate);
        const bool C2 = (PrimeCandidate % (30*K+1) != 0) && (PrimeCandidate % (30*K+7) != 0)
                    && (PrimeCandidate % (30*K+11) != 0) && (PrimeCandidate % (30*K+13) != 0)
                    && (PrimeCandidate % (30*K+17) != 0) && (PrimeCandidate % (30*K+19) != 0)
                    && (PrimeCandidate % (30*K+23) != 0) && (PrimeCandidate % (30*K+29) != 0);
        return (!C1 && !C2) ? ((K == 1) ? (PrimeCandidate%7 != 0) : false) : ((C1 && C2) ? IsPrime30loop::value(PrimeCandidate, K+1) : (C1 ? false : true /* C2 */));
    }
};


template<class InitCheck = CheckSmallPrimes2_29>
struct IsPrime30
{
    static constexpr const bool value(const ulong_t PrimeCandidate)
    {
        return (InitCheck::value(PrimeCandidate) && cexpr::IsPrime30loop::value(PrimeCandidate));
    }
};


}  // cexpr


/////////////////////////////////////////////
template<typename C, bool b, typename Next>
struct addType
{
  typedef Next type;
};

template<typename C, typename Next>
struct addType<C,true,Next>
{
  typedef typename typelist_cat<C,Next>::type type;
};




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

// Generate all primes less equal Limit with a constant Q in the formula Q*K+R
// Similar to GenPrimesFormula, but without second exit condition,
// that means the same formula is used up to the Limit
template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList,
         typename CheckPrimeCandidate = cexpr::IsPrime6<>,
         typename RList = InitRList, bool doExit = false>
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
: public GeneratePrimes<Limit, Q, K+1, InitRList> {};

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename H, typename ...Tail, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,typelist<H,Tail...>,true>
{ typedef typelist<> type; };

template<ulong_t Limit, ulong_t Q, ulong_t K, typename InitRList, typename CheckPrimeCandidate>
struct GeneratePrimes<Limit,Q,K,InitRList,CheckPrimeCandidate,typelist<>,true>
{ typedef typelist<> type; };




namespace F6K {

// Initial simple and efficient primes generator using formula 6k+r, r = 1,5
// It has shorter compile-times comparing to GenPrimes
template<int Start, int Limit, int K,
int C = (6*K + 1 <= Limit)>
struct nextPrimesDirect;

template<int Start, int Limit, int K>
struct nextPrimesDirect<Start, Limit, K, true>
{
  static const int Primecandidate1 = 6*K + 1;
  static const int Primecandidate2 = 6*K + 5;
#if (__cplusplus >= 201402L)  // c++14
  // The constexpr version of IsPrime30 is slightly faster and has much shorter implementation
  static const bool p1 = cexpr::IsPrime6<cexpr::CheckSmallPrimes5>::value(Primecandidate1) && (Primecandidate1 > Start);
  static const bool p2 = cexpr::IsPrime6<cexpr::CheckSmallPrimes5>::value(Primecandidate2) && (Primecandidate2 <= Limit) && (Primecandidate2 > Start);
#else
  static const bool p1 = IsPrime6<CheckSmallPrimes5>::Do<Primecandidate1>::value && (Primecandidate1 > Start);
  static const bool p2 = IsPrime6<CheckSmallPrimes5>::Do<Primecandidate2>::value && (Primecandidate2 <= Limit) && (Primecandidate2 > Start);
#endif
  typedef typename nextPrimesDirect<Start, Limit, K+1>::type nextIter;
  typedef typename std::conditional<p1 && p2,
    typename typelist_cat<typelist<ulong_<Primecandidate1>, ulong_<Primecandidate2>>, nextIter>::type,
    typename std::conditional<p1, typename typelist_cat<ulong_<Primecandidate1>, nextIter>::type,
    typename std::conditional<p2, typename typelist_cat<ulong_<Primecandidate2>, nextIter>::type, nextIter>::type>::type>::type type;
};

template<int Start, int Limit, int K>
struct nextPrimesDirect<Start, Limit, K, false>
{
  typedef typelist<> type;
};


/////////////////////////////////////////////////
template<ulong_t Limit>
struct GeneratePrimesDirect
{
  typedef typename nextPrimesDirect<0,Limit,1>::type PrimesList;
  typedef typename typelist_cat<typelist235,PrimesList>::type type;
};


/////////////////////////////////////////////////
template<ulong_t Limit>
struct GeneratePrimesWithList
{
  typedef typelist<ulong_<5>> List1;
  typedef typelist<ulong_<1>,ulong_<5>> RList;

//   typedef typename GeneratePrimes<Limit,6,1,RList,IsPrime6<CheckSmallPrimes5> >::type PrimesList;
  typedef typename GeneratePrimes<Limit,6,1,RList,cexpr::IsPrime30<cexpr::CheckSmallPrimes13_29> >::type PrimesList;
  typedef typename typelist_cat<typelist235,PrimesList>::type type;
};

} // F6K


namespace F30K {

// Experimental code to achieve shorter compile-times comparing to GenPrimes
template<int Limit, int K = 1, int C = (30*K + 1 <= Limit)>
struct nextPrimesDirect;

template<int Limit, int K>
struct nextPrimesDirect<Limit, K, true>
{
  static const ulong_t C1 = 30*K + 1;
  static const ulong_t C2 = 30*K + 7;
  static const ulong_t C3 = 30*K + 11;
  static const ulong_t C4 = 30*K + 13;
  static const ulong_t C5 = 30*K + 17;
  static const ulong_t C6 = 30*K + 19;
  static const ulong_t C7 = 30*K + 23;
  static const ulong_t C8 = 30*K + 29;
#if (__cplusplus >= 201402L)  // c++14
  // The constexpr version of IsPrime30 is slightly faster and has much shorter implementation
  typedef cexpr::IsPrime30<cexpr::CheckSmallPrimes7_29> isPrime;
  static const bool p1 = isPrime::value(C1);
  static const bool p2 = isPrime::value(C2) && (C2 <= Limit);
  static const bool p3 = isPrime::value(C3) && (C3 <= Limit);
  static const bool p4 = isPrime::value(C4) && (C4 <= Limit);
  static const bool p5 = isPrime::value(C5) && (C5 <= Limit);
  static const bool p6 = isPrime::value(C6) && (C6 <= Limit);
  static const bool p7 = isPrime::value(C7) && (C7 <= Limit);
  static const bool p8 = isPrime::value(C8) && (C8 <= Limit);
#else
  typedef IsPrime30<CheckSmallPrimes7_29> isPrime;
  static const bool p1 = isPrime::Do<C1>::value;
  static const bool p2 = isPrime::Do<C2>::value && (C2 <= Limit);
  static const bool p3 = isPrime::Do<C3>::value && (C3 <= Limit);
  static const bool p4 = isPrime::Do<C4>::value && (C4 <= Limit);
  static const bool p5 = isPrime::Do<C5>::value && (C5 <= Limit);
  static const bool p6 = isPrime::Do<C6>::value && (C6 <= Limit);
  static const bool p7 = isPrime::Do<C7>::value && (C7 <= Limit);
  static const bool p8 = isPrime::Do<C8>::value && (C8 <= Limit);
#endif
  typedef typename nextPrimesDirect<Limit, K+1>::type NextIter;

  typedef typename addType<ulong_<C8>,p8,NextIter>::type R8;
  typedef typename addType<ulong_<C7>,p7,R8>::type R7;
  typedef typename addType<ulong_<C6>,p6,R7>::type R6;
  typedef typename addType<ulong_<C5>,p5,R6>::type R5;
  typedef typename addType<ulong_<C4>,p4,R5>::type R4;
  typedef typename addType<ulong_<C3>,p3,R4>::type R3;
  typedef typename addType<ulong_<C2>,p2,R3>::type R2;
  typedef typename addType<ulong_<C1>,p1,R2>::type type;
};

template<int Limit, int K>
struct nextPrimesDirect<Limit, K, false>
{
  typedef typelist<> type;
};


/////////////////////////////////////////////////
template<ulong_t Limit>
struct GeneratePrimesDirect
{
  typedef typelist<ulong_<2>,ulong_<3>,ulong_<5>,ulong_<7>,ulong_<11>,ulong_<13>,ulong_<17>,ulong_<19>,ulong_<23>,ulong_<29>> List1;
  typedef typename nextPrimesDirect<Limit>::type PrimesList;
  typedef typename typelist_cat<List1, PrimesList>::type type;
};


/////////////////////////////////////////////////
template<ulong_t Limit>
struct GeneratePrimesWithList
{
  typedef typelist<ulong_<7>,ulong_<11>,ulong_<13>,ulong_<17>,ulong_<19>,ulong_<23>,ulong_<29>> List1;
  typedef typename typelist_cat<ulong_<1>,List1>::type RList;

#if (__cplusplus >= 201402L)  // c++14
  typedef typename GeneratePrimes<Limit,30,1,RList,cexpr::IsPrime30<cexpr::CheckNone> >::type PrimesList;
#else  // c++11
  typedef typename GeneratePrimes<Limit,30,1,RList,IsPrime30<CheckNone> >::type PrimesList;
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
    //   typedef typename GeneratePrimes<Limit,210,1,RList,IsPrime6<CheckNone> >::type PrimesList;

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
