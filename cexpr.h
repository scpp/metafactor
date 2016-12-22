/***************************************************************************
 *   Copyright (C) 2015 by Vladimir Mirnyy, blog.scpp.eu                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the MIT License                                 *
 ***************************************************************************/

#ifndef __cexpr_h
#define __cexpr_h

/** \file
    \brief Constexpr functions for primes generation using variadic templates
*/

#include "variadic_typelist.h"

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

struct CheckSmallPrimes5_29
{
    static constexpr const bool value(const ulong_t PrimeCandidate)
    {
        return cexpr::CheckSmallPrimes5::value(PrimeCandidate) && cexpr::CheckSmallPrimes7_29::value(PrimeCandidate);
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


#endif /*__cexpr_h*/
