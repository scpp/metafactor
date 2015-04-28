/***************************************************************************
 *   Copyright (C) 2014 by Vladimir Mirnyy                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/

#ifndef __gfftfactor_h
#define __gfftfactor_h

/** \file
    \brief Factorization meta-algorithms 
*/

#include "metafactorcommon.h"

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


// template<typename TList>
// struct OutTypeList;
// 
// template<typename H, typename Tail>
// struct OutTypeList<Loki::Typelist<H,Tail> > {
//   static void print(std::ostream& os, const char sep = '\t') {
//     os << H::first::value << "^" << H::second::value << sep;
//     OutTypeList<Tail>::print(os, sep);
//   }
// };
// template<>
// struct OutTypeList<Loki::NullType> {
//   static void print(std::ostream& os, const char sep = '\t') {
//     os << std::endl;
//   }
// };



template<typename TList>
struct Check;

template<typename H, typename Tail>
struct Check<Loki::Typelist<H,Tail> > {
  static int_t get() {
    return ipow<H::first::value, H::second::value>::value * Check<Tail>::get();
  }
};
template<>
struct Check<Loki::NullType> {
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



template<typename T, typename NextIter, bool C>
struct FCondition;

template<typename T, typename NextIter>
struct FCondition<T,NextIter,true>
{
  typedef Loki::Typelist<T, NextIter> Result;
};

template<typename T, typename NextIter>
struct FCondition<T,NextIter,false>
{
  typedef NextIter Result;
};


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

template<int_t N, short Q, int_t K, typename RList, bool doExit = false>
struct RLoop;

template<int_t N, short Q, int_t K, typename H, typename Tail>
struct RLoop<N,Q,K,Loki::Typelist<H,Tail>,false>
{
  static const int_t Candidate = Q*K + H::value;
//  typedef check_candidate<N,Candidate> TCand;
  typedef try_factor<N, Candidate> trial;
  typedef spair<sint<Candidate>, sint<trial::power> > T;
  typedef typename RLoop<N/trial::factor, Q, K, Tail, (Candidate*Candidate > N)>::Result NextIter;
  typedef typename FCondition<T,NextIter,(trial::power > 0)>::Result Result;
};

template<int_t N, short Q, int_t K, typename H, typename Tail>
struct RLoop<N,Q,K,Loki::Typelist<H,Tail>,true>
: public RLoop<N,Q,K,Loki::NullType, true> {};

template<int_t N, int_t K>
struct Condition {
 static const bool value = ((N % (30*K+1) == 0) || (N % (30*K+7) == 0) || (N % (30*K+11) == 0) || (N % (30*K+13) == 0)
   || (N % (30*K+17) == 0) || (N % (30*K+19) == 0) || (N % (30*K+23) == 0) || (N % (30*K+29) == 0));
  
};

template<int_t N, short Q, int_t K>
struct RLoop<N,Q,K,Loki::NullType, false>
{
  static const int_t Candidate = Q*(K+1) + 1;
//   static const bool b = CheckAll<N,Q,K+1,Reminders>::value;
//   static const bool b = Condition<N,K+1>::value;
  typedef typename SelectFactors<N,Q,K+1,Reminders>::Result RList;
//   typedef typename Loki::Select<b, typename RLoop<N,Q,K+1,typename SelectFactors<N,Q,K+1,Reminders>::Result,(Candidate*Candidate > N)>::Result, 
//                                    typename RLoop<N,Q,K+1,Loki::NullType>::Result>::Result Result;
  typedef typename RLoop<N,Q,K+1,RList,(Candidate*Candidate > N)>::Result Result;
};

template<int_t N, short Q, int_t K>
struct RLoop<N,Q,K,Loki::NullType, true>
{
  typedef spair<sint<N>, sint<1> > T;
  typedef Loki::Typelist<T, Loki::NullType> Result;
};

template<short Q, int_t K, typename RList>
struct RLoop<1,Q,K,RList,true>
{
  typedef Loki::NullType Result;
};

template<short Q, int_t K>
struct RLoop<1,Q,K,Loki::NullType,true>
{
  typedef Loki::NullType Result;
};


/*
template<int_t N, int_t K,  
bool C1 = ((30*K+1)*(30*K+1) <= N),
bool C2 = ((N % (30*K+1) == 0) || (N % (30*K+7) == 0) || (N % (30*K+11) == 0) || (N % (30*K+13) == 0)
   || (N % (30*K+17) == 0) || (N % (30*K+19) == 0) || (N % (30*K+23) == 0) || (N % (30*K+29) == 0)
)>
struct KLoop;

template<int_t N, int_t K>
struct KLoop<N, K, true, true>
{
  static const int_t C1 = 30*K + 1;
  static const int_t C2 = 30*K + 7;
  static const int_t C3 = 30*K + 11;
  static const int_t C4 = 30*K + 13;
  static const int_t C5 = 30*K + 17;
  static const int_t C6 = 30*K + 19;
  static const int_t C7 = 30*K + 23;
  static const int_t C8 = 30*K + 29;
  
  static const int_t P1 = try_factor<N, C1>::value;
  static const int_t P2 = try_factor<N, C2>::value;
  static const int_t P3 = try_factor<N, C3>::value;
  static const int_t P4 = try_factor<N, C4>::value;
  static const int_t P5 = try_factor<N, C5>::value;
  static const int_t P6 = try_factor<N, C6>::value;
  static const int_t P7 = try_factor<N, C7>::value;
  static const int_t P8 = try_factor<N, C8>::value;
  
  static const int_t F1 = ipow<C1, P1>::value;
  static const int_t F2 = ipow<C2, P2>::value;
  static const int_t F3 = ipow<C3, P3>::value;
  static const int_t F4 = ipow<C4, P4>::value;
  static const int_t F5 = ipow<C5, P5>::value;
  static const int_t F6 = ipow<C6, P6>::value;
  static const int_t F7 = ipow<C7, P7>::value;
  static const int_t F8 = ipow<C8, P8>::value;
  
  typedef spair<sint<C1>, sint<P1> > T1;
  typedef spair<sint<C2>, sint<P2> > T2;
  typedef spair<sint<C3>, sint<P3> > T3;
  typedef spair<sint<C4>, sint<P4> > T4;
  typedef spair<sint<C5>, sint<P5> > T5;
  typedef spair<sint<C6>, sint<P6> > T6;
  typedef spair<sint<C7>, sint<P7> > T7;
  typedef spair<sint<C8>, sint<P8> > T8;
  
  static const int_t NextN = N/F1/F2/F3/F4/F5/F6/F7/F8;
  typedef typename KLoop<NextN, K+1>::Result NextIter;
  
  typedef typename FCondition<T8,NextIter,(P8>0)>::Result R1;
  typedef typename FCondition<T7,R1,(P7>0)>::Result R2;
  typedef typename FCondition<T6,R2,(P6>0)>::Result R3;
  typedef typename FCondition<T5,R3,(P5>0)>::Result R4;
  typedef typename FCondition<T4,R4,(P4>0)>::Result R5;
  typedef typename FCondition<T3,R5,(P3>0)>::Result R6;
  typedef typename FCondition<T2,R6,(P2>0)>::Result R7;
  typedef typename FCondition<T1,R7,(P1>0)>::Result Result;
};

template<int_t N, int_t K>
struct KLoop<N, K, true, false> 
: public KLoop<N, K+1> {};

template<int_t N, int_t K, bool C>
struct KLoop<N, K, false, C>
{
  typedef spair<sint<N>, sint<1> > T;
  typedef Loki::Typelist<T, Loki::NullType> Result;
};

template<int_t K, bool C>
struct KLoop<1, K, false, C>
{
  typedef Loki::NullType Result;
};
*/

template<typename Num,
typename StartList = InitialPrimesList>
struct Factorization;

// Factorization using trial deletion from InitialPrimesList
template<int_t N, typename H, typename Tail>
struct Factorization<sint<N>, Loki::Typelist<H,Tail> >
{
  //static const int_t N = Num::value;
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
//: public KLoop<N, 1> {};
{
  static const short Q = StartQ;
  static const int_t Candidate = Q + 1;
  typedef typename SelectFactors<N,Q,1,Reminders>::Result RList;
  typedef typename RLoop<N,Q,1,RList,(Candidate*Candidate > N)>::Result Result;
};

// End of factorization
template<typename H, typename Tail>
struct Factorization<sint<1>, Loki::Typelist<H,Tail> > {
  typedef Loki::NullType Result;
};


#endif /*__gfftfactor_h*/
