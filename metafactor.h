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


#endif /*__metafactor_h*/
