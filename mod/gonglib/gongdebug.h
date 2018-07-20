#ifndef _GONG_DEBUG_H
#define _GONG_DEBUG_H

/** @file gongdebug.h debug definitions
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

/******************************************************************
	Adaptaded from the GCC std library <debug/debug.h>
	To use the check functions, define _GONG_DEBUG somewhere in your project
*******************************************************************/

#include <iostream>
#include <cstdlib>

std::string methodName(const char *prettyFunction);
#   define __METHOD_NAME__ methodName(__PRETTY_FUNCTION__).c_str()

#ifdef _GONG_DEBUG

namespace __gong_debug
{
extern int _gong_debug_level;
inline void
__replacement__gong_debug_assert(const char* __file, int __line, const char* __function,
                                 const char* __condition)
{
    std::cout << __file << ':' << __line << ':' << __function << ": Assertion '"
              << __condition << "' failed." << std::endl;
    std::abort();
}
inline void
__replacement__gong_debug_assert_equals(const char* __file, int __line, const char* __function,
                                        const std::string &__expr1, const std::string &__expr2)
{
    std::cout << __file << ':' << __line << ':' << __function << ": Assertion '"
              << __expr1 << "' == '" << __expr2 << "' failed." << std::endl;
    std::abort();
}
}

#   define _GONG_THROW(_e,_m) { Xtring mm(_m); mm+=" ["; mm+= __METHOD_NAME__; mm+="]"; throw _e(mm); }

#	define _GONG_DEBUG_ASSERT(_Condition) \
		do { \
			if (! (_Condition)) \
				::__gong_debug::__replacement__gong_debug_assert(__FILE__, __LINE__, \
				__METHOD_NAME__, #_Condition); \
  		} while (false)

#	define _GONG_DEBUG_ASSERT_EQUALS(_exp1, _exp2)  \
		do { \
			if ( (_exp1) != (_exp2) ) \
				::__gong_debug::__replacement__gong_debug_assert_equals(__FILE__, __LINE__, \
				__METHOD_NAME__, _exp1, _exp2); \
		} while (false)

#	define _GONG_DEBUG_PRINT(level, message) if(level<=::__gong_debug::_gong_debug_level) { \
		for(int _gonglogprint_i=0;_gonglogprint_i<level;_gonglogprint_i++) \
			std::cout << "="; \
		std::cout << message << " [" << __METHOD_NAME__ << "]" << std::endl; }
#	define _GONG_DEBUG_TRACE(level) if(level<=::__gong_debug::_gong_debug_level) { \
		for(int _gonglogprint_i=0;_gonglogprint_i<level;_gonglogprint_i++) \
			std::cout << "="; \
		std::cout << __METHOD_NAME__ << std::endl; }

#	define _GONG_DEBUG_PRINT_LIST( list, type ) { \
	_GONG_DEBUG_PRINT(0, Xtring::printf("%s<%s>[%d]=", #list, #type, list.size() ) ); \
	for( List<type>::iterator it = list.begin(); \
		it != list.end(); \
		++ it )  \
		_GONG_DEBUG_PRINT(0, *it ); \
	}

#else

#	define _GONG_DEBUG_ASSERT(_Condition)
#	define _GONG_DEBUG_ASSERT_EQUALS(_exp1, _exp2)
#	define _GONG_DEBUG_PRINT(level, message) {}
#	define _GONG_DEBUG_TRACE(level) {}
#	define _GONG_DEBUG_PRINT_LIST( list, type ) {}
#   define _GONG_THROW(_e,_m) { throw _e(_m); }

#endif


#define _GONG_DEBUG_WARNING(message) \
	{ std::cerr << "WARNING: " << message << " [" << __METHOD_NAME__ << "]" << std::endl;std::cout.flush();}
#define _GONG_DEBUG_ERROR(message) \
	{ std::cerr << "ERROR: " << message << " [" << __METHOD_NAME__ << "]" << std::endl;std::cout.flush();}
#define _GONG_DEBUG_PERROR(message) \
    { std::cerr << "PERROR: " << message << ":" << strerror(errno) << " [" << __METHOD_NAME__ << "]" << std::endl;std::cout.flush();}

#endif  // _GONG_DEBUG_H
