/***************************************************************************
 *   Copyright (C) 2015 by Vladimir Mirnyy, blog.scpp.eu                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the MIT License                                 *
 ***************************************************************************/

#ifndef __variadic_typelist_h
#define __variadic_typelist_h

/** \file
    \brief compile-time list of types based on variadic templates from C++11 
*/

#include <type_traits>

// A class template that just a list of types:
template <class... T> struct typelist { };


template <typename TList> 
struct get_first;

template <typename T, typename ...List> 
struct get_first<typelist<T,List...>>
{
  typedef T type;
};

template <typename T> 
struct get_first<typelist<T>>
{
  typedef T type;
};

template <> 
struct get_first<typelist<>>
{
  typedef typelist<> type;
};


template <typename List1, typename List2> 
struct typelist_cat;

template <typename ...List1, typename ...List2>
struct typelist_cat<typelist<List1...>, typelist<List2...>>
{
  using type = typelist<List1..., List2...>;
};

template <typename T, typename ...List2>
struct typelist_cat<T, typelist<List2...>>
{
  using type = typelist<T, List2...>;
};

template <typename T, typename ...List1>
struct typelist_cat<typelist<List1...>,T>
{
  using type = typelist<List1... ,T>;
};


template<typename List>
struct typelist_out;

template<typename T, typename ...Args>
struct typelist_out<typelist<T, Args...> >
{
  static void print(std::ostream& os = std::cout, const char sep = '\t') 
  {
    os << T::value << sep;
    typelist_out<typelist<Args...>>::print(os, sep);
  }
};

template<typename T1, typename T2, typename ...Args>
struct typelist_out<typelist<spair<T1,T2>, Args...> >
{
  static void print(std::ostream& os = std::cout, const char sep = '\t') 
  {
    os << T1::value << "^" << T2::value << sep;
    typelist_out<typelist<Args...>>::print(os, sep);
  }
};

template<>
struct typelist_out<typelist<>>
{
  static void print(std::ostream& os = std::cout, const char sep = '\t') 
  { 
    os << std::endl;
  }
};

#endif 
