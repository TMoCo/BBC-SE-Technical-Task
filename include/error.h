/*
* Author: Thomas Moreno Cooper
* Date Created: 04/01/2022
* Copyright 2022 under MIT License
*
* error message macro
*/

#ifndef ERROR_H
#define ERROR_H 1

#if (__cplusplus >= 202002L)
#define ERROR_MSG(format, ...) \
__m_error_msg(format __VA__OPT__(, ) __VA_ARGS__)
#else
#define ERROR_MSG(format, file, line, ...) \
__m_error_msg(format, file, line, ##__VA_ARGS__);
#endif

#include <stdio.h>
#include <stdlib.h>
inline void __m_error_msg(const char* format, const char* file, const int line, ...)
{
  fprintf(stderr, "/!\\ ERROR! in %s at %i.\nInfo:\t", file, line);
  if (format)
  {
    // see stdio.h
    va_list args;
    __crt_va_start(args, format);
    _vfprintf_p(stderr, format, args);
    __crt_va_end(args);
  }
  fflush(stderr);

#ifdef WIN32
  __debugbreak();
#endif
#ifdef NDEBUG
  abort(); // critical error
#endif
}
#endif // !ERROR_H


