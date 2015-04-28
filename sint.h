/***************************************************************************
 *   Copyright (C) 2009-2014 by Vladimir Mirnyy                            *
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

/** \file
    \brief Definition of holders for static integer types
*/

#ifndef __sint_h
#define __sint_h

#ifdef _MSC_VER
typedef unsigned __int64 int_t;
#else
typedef unsigned long int_t;
#endif
//typedef unsigned long uint_t;
typedef unsigned short short_t;

/// Integer number metacontainer.
/**     Integer N is wrapped into container class to handle integers and other
        compile-time number-classes using the same operation classes specializing
        them for particular number-container.
 \param N an integer number
*/
template<int_t N>
struct sint {
  typedef int_t value_type;
  static const int_t value = N;
};

template<typename T1, typename T2>
struct spair {
  typedef T1 first;
  typedef T2 second;
};

#endif
