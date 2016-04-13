/***************************************************************************
 *   Copyright (C) 2015 by Vladimir Mirnyy, blog.scpp.eu                   *
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
typedef unsigned __int64 int_t;
#else
typedef unsigned long uint_t;
#endif
//typedef unsigned long uint_t;
typedef unsigned short short_t;

/// Integer number metacontainer.
/**     Integer N is wrapped into container class to handle integers and other
        compile-time number-classes using the same operation classes specializing
        them for particular number-container.
 \param N an integer number
*/
template<uint_t N>
struct sint {
  typedef uint_t value_type;
  static const uint_t value = N;
};

template<typename T1, typename T2>
struct spair {
  typedef T1 first;
  typedef T2 second;
};

#endif
