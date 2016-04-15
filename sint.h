/***************************************************************************
 *   Copyright (C) 2016 by Vladimir Mirnyy, blog.scpp.eu                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the MIT License                                 *
 ***************************************************************************/

/** \file
    \brief Definition of holders for static integer types
*/

#ifndef __sint_h
#define __sint_h

#ifdef _MSC_VER
typedef __int32 int_t;
typedef unsigned __int32 uint_t;
typedef __int64 long_t;
typedef unsigned __int64 ulong_t;
#else
typedef int int_t;
typedef unsigned int uint_t;
typedef long long_t;
typedef unsigned long ulong_t;
#endif
typedef short short_t;
typedef unsigned short ushort_t;


/// Integer number metacontainer.
/**     Integer N is wrapped into container class to handle integers and other
        compile-time number-classes using the same operation classes specializing
        them for particular number-container.
 \param N an integer number
*/
#define STATIC_INTEGER_CLASS(Type, Name) \
template<Type N>                         \
struct Name {                        \
   typedef Type value_type;              \
   static const Type value = N;          \
};

STATIC_INTEGER_CLASS(int_t, int_)
STATIC_INTEGER_CLASS(uint_t, uint_)
STATIC_INTEGER_CLASS(long_t, long_)
STATIC_INTEGER_CLASS(ulong_t, ulong_)
#undef STATIC_INTEGER_CLASS


template<typename T1, typename T2>
struct pair_ {
  typedef T1 first;
  typedef T2 second;
};

#endif
