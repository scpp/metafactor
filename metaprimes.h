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
