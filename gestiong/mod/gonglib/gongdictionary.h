#ifndef _GONG_DICTIONARY_H
#define _GONG_DICTIONARY_H

/** @file gongdictionary.h A map that can be accessed either by key or sequentially.
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <map>
#include <stdexcept>
#include <iostream>
#include "gongdebug.h"
#include "gongxtring.h"

namespace gong {

template<typename T>
class Dictionary : public std::map<Xtring, T>
{
public:
    typedef typename std::map<Xtring, T>::iterator iterator;
    typedef typename std::map<Xtring, T>::const_iterator const_iterator;
    typedef typename std::map<Xtring, T>::key_type key_type;
    typedef typename std::map<Xtring, T>::mapped_type mapped_type;
    typedef typename std::map<Xtring, T>::key_compare key_compare;

    Dictionary<T>() : std::map<Xtring, T>(), mNullValue()
    {}

    /**
    	*  @brief Map copy constructor.
    	*  @param xm A %map of identical element and allocator types.
    	*
    	*  The newly-created %map uses a copy of the allocation object used
    	*  by @a x.       */
    Dictionary<T>(const Dictionary &xm) : std::map<Xtring, T>(xm),
        mSequential(xm.mSequential), mNullValue()
    {}

    iterator find(const Xtring &key) {
        return std::map<Xtring, T>::find(key.upper());
    }
    const_iterator find(const Xtring &key) const {
        return std::map<Xtring, T>::find(key.upper());
    }
    const Xtring &getKey(uint pos) const {
        if( pos < mSequential.size() )
            return mSequential[pos];
        else
            std::__throw_out_of_range(__N("Dictionary::at"));
    }

    std::map<Xtring, T> &set(const key_type& __k, const T &newvalue)
    {
        iterator __i = find(__k.upper());
        if (__i == this->end() ) {
            insert( __k.upper(), newvalue );
        } else {
            (*__i).second = newvalue;
        }
        return *this;
    }

    bool insert(const key_type& __k, const T &last)
    {
        if( (std::map<Xtring, T>::insert(std::pair<Xtring,T>(__k.upper(), last))).second )
        {
            mSequential.push_back( __k.upper() );
            return true;
        }
        return false;
    }

    void clear() {
        std::map<Xtring, T>::clear();
        mSequential.clear();
    }

    const T operator[](uint pos) const
    {
        if( pos < mSequential.size() )
            return operator[] (mSequential[pos]);
        else {
            _GONG_DEBUG_PRINT(7, "The keys are: " + toString() );
            std::__throw_out_of_range(__N("Dictionary::at"));
        }
    }

    /* This function is const because we want to avoid the insert syntax: Dict[key] = value; */
    mapped_type &operator[](const key_type& __k) const
    {
        const_iterator __i = find( __k.upper() );
        if( __i == this->end() ) {
            _GONG_DEBUG_PRINT(6, Xtring::printf("WARNING: Key '%s' not found in dictionary", __k.c_str()) );
            _GONG_DEBUG_PRINT(7, "The keys are: " + toString() );
            return const_cast<mapped_type &>(mNullValue); // Not to return a temporary value
        }
        return const_cast<mapped_type &>((*__i).second);
    }


    Xtring toString(const char *sep =", ") const {
        Xtring ret;
        for( const_iterator __i = std::map<Xtring, T>::begin();
                __i != std::map<Xtring, T>::end();
                ++ __i ) {
            if( __i != std::map<Xtring, T>::begin() )
                ret += sep;
            ret += (*__i).first;
        }
        return ret;
    }

    /**
     *  @brief Erases an element from a %map.
     *  @param  position  An iterator pointing to the element to be erased.
     *
     *  This function erases an element, pointed to by the given iterator,
     *  from a %map.  Note that this function only erases the element, and
     *  that if the element is itself a pointer, the pointed-to memory is not
     *  touched in any way.  Managing the pointer is the user's responsibilty.
     */
    void erase(iterator position)
    {
        for( std::vector<Xtring>::iterator it = mSequential.begin();
                it!= mSequential.end();
                it++)
            if( (*position).first == *it )
            {
                mSequential.erase(it);
                break;
            }
        std::map<Xtring, T>::erase(position);
    }


    /**
     *  @brief Erases the sequential n-th element from the map
     *  @param  index The index of the element to be deleted
     */
    void erase( uint index )
    {
        std::map<Xtring, T>::erase( std::map<Xtring, T>::find( getKey( index ) ) );
    }


    /**
     *  @brief Erases elements according to the provided key.
     *  @param key Key of element to be erased.
     *  @return  The number of elements erased.
     *
     *  This function erases all the elements located by the given key from
     *  a %map.
     *  Note that this function only erases the element, and that if
     *  the element is itself a pointer, the pointed-to memory is not touched
     *  in any way.  Managing the pointer is the user's responsibilty.
     */
    uint erase(const Xtring &key)
    {
        for( std::vector<Xtring>::iterator it = mSequential.begin();
                it!= mSequential.end();
                it++) {
            if( key.upper() == (*it).upper() )
            {
                mSequential.erase(it);
                break;
            }
        }
        return std::map<Xtring, T>::erase(key.upper());
    }


private:
    std::vector<Xtring> mSequential;
    mapped_type mNullValue;
};

} // namespace gong

#endif // _GONG_DICTIONARY_H
