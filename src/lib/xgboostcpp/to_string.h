/*!
  @file   to_string.h
  @author David Hirvonen
  @brief  Declaration of missing stdlib routines in Android NDK.

  \copyright Copyright 2014-2016 Elucideye, Inc. All rights reserved.
  \license{This project is released under the 3 Clause BSD License.}

*/

#ifndef __xgboostcpp_to_string_h__
#define __xgboostcpp_to_string_h__

#if ANDROID

#include "xgboostcpp/xgboostcpp_def.h"

#include <string>
#include <sstream>
#include <cstdlib>

XGBOOSTCPP_BEGIN_NAMESPACE(std)

template <typename T>
inline std::string to_string(T value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}

template <typename T>
inline T stringTo(const std::string& s)
{
    std::stringstream conv;
    conv << s;
    T t;
    conv >> t;
    return t;
}

inline long double strtold(const std::string& s)
{
    return stringTo<long double>(s);
}
inline long double strtold(const char* str, char** str_end)
{
    return strtod(str, str_end);
}
inline long long stoll(const std::string& s)
{
    return stringTo<long long>(s);
}
inline int stoi(const std::string& s)
{
    return stringTo<int>(s);
}
inline unsigned long stoul(const std::string& s)
{
    return stringTo<unsigned long>(s);
}
inline unsigned long long stoull(const std::string& s)
{
    return stringTo<unsigned long long>(s);
}
inline float stof(const std::string& s)
{
    return stringTo<float>(s);
}
inline long stol(const std::string& s)
{
    return stringTo<long>(s);
}
inline double stod(const std::string& s)
{
    return stringTo<double>(s);
}
inline long double stold(const std::string& s)
{
    return stringTo<long double>(s);
}

XGBOOSTCPP_END_NAMESPACE(std)

#endif // ANDROID
#endif // __xgboostcpp_stdlib_string_h__
