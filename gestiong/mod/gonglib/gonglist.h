#ifndef _GONG_LIST_H
#define _GONG_LIST_H

/** @file gonglist.h Templatized list of values
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <cstdarg>
#include <vector>
#include <string>

namespace gong {

template<class T>
class List: public std::vector<T>
{
public:
    typedef typename std::vector<T>::const_iterator const_iterator;
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::size_type size_type;

    List() : std::vector<T>() {};
    List( unsigned int size, ... );
    List &operator<<(const T &val) {
        if( !contains(val) ) this->push_back(val);
        return *this;
    }
    List &operator<<(const List<T> &val) {
        for( List<T>::const_iterator it = val.begin(); it != val.end(); ++it )
            operator<< (*it);
        return *this;
    }
    bool contains(const T &element) const;
    int find(const T &element) const;
    iterator begin() {
        return std::vector<T>::begin();
    }
    const_iterator begin() const {
        return std::vector<T>::begin();
    }
    iterator end() {
        return std::vector<T>::end();
    }
    const_iterator end() const {
        return std::vector<T>::end();
    }
    size_type size() const {
        return std::vector<T>::size();
    }
    std::string join(const std::string &sep) const;
    bool remove(const T &element);
#ifdef _GONG_DEBUG
    void debug_print(int debug_level = 0) const;
#endif
};

typedef List<int> IntList;
typedef List<unsigned int> UIntList;
typedef List<double> DoubleList;
typedef List<bool> BoolList;
typedef List<char> CharList;

template<class T> inline
bool List<T>::contains(const T &val) const
{
    for( const_iterator it = begin();
            it != end();
            ++it )
        if( val == *it )
            return true;
    return false;
}

template<class T> inline
int List<T>::find(const T &val) const
{
    int i = 0;
    for( const_iterator it = begin();
            it != end();
            ++it, ++i )
        if( val == *it )
            return i;
    return -1;
}

template<class T> inline
bool List<T>::remove(const T &element)
{
    int i = 0;
    for( iterator it = begin();
            it != end();
            ++it, ++i ) {
        if( element == *it ) {
            this->erase( it );
            return true;
        }
    }
    return false;
}
/** Do not use this constructor with short int as it is promoted to int and it will abort*/
template<class T> inline
extern List<T>::List(unsigned int asize, ... )
{
    va_list elements;
    va_start( elements, asize);
    for( ; asize > 0; --asize ) {
        T pt = va_arg( elements, T);
        *this << pt;
    }
    va_end( elements );
}

#ifdef _GONG_DEBUG
template<class T>
void List<T>::debug_print(int debug_level) const
{
    for( const_iterator it = begin(); it != end(); ++it )
        _GONG_DEBUG_PRINT(debug_level, *it );
}
#endif

} // namespace gong

#endif
