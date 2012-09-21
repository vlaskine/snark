// This file is part of rur, a generic and flexible library 
// for robotics research.
//
// Copyright (C) 2011 The University of Sydney
//
// rur is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// rur is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License 
// for more details.
//
// You should have received a copy of the GNU General Public
// License along with rur. If not, see <http://www.gnu.org/licenses/>.

#ifndef RUR_GRAPHICS_MAP_HEADER_GUARD_
#define RUR_GRAPHICS_MAP_HEADER_GUARD_

#include <map>
#include <rur/graphics/impl/renderable.h>

namespace rur { namespace graphics {

template < typename K, typename T >
class Map : public impl::renderable< T >
{
    public:
        Map( std::size_t capacity ) : impl::renderable< T >( capacity ) {}
        
        std::size_t capacity() const { return this->m_storage.capacity(); }
        
        std::size_t size() const { return this->m_storage.size(); }
        
        bool empty() const { return this->m_storage.empty(); }
        
        void clear() { this->m_storage.clear(); }
        
        void insert( const std::pair< K, T >& v );
        
        void erase( K key );
        
        bool exists( K key ) const { return m_keyMap.find( key ) != m_keyMap.end(); }
        
    private:
        typedef std::map< K, std::size_t > KeyMap;
        typedef std::map< std::size_t, K > IndexMap;
        KeyMap m_keyMap;
        IndexMap m_indexMap;
};

template < typename K, typename T >
inline void Map< K, T >::insert( const std::pair< K, T >& v )
{
    std::size_t i = size();
    this->m_storage.PushBack( v.second );
    try
    {
        m_keyMap.insert( std::make_pair( v.first, i ) );
        m_indexMap.insert( std::make_pair( i, v.first ) );
    }
    catch( ... )
    {
        erase( v.first );
        throw;
    }
}

template < typename K, typename T >
inline void Map< K, T >::erase( K key )
{
    typename KeyMap::iterator k = m_keyMap.find( key );
    if( k == m_keyMap.end() ) { return; }
    std::size_t last = size() - 1;
    typename IndexMap::iterator i = m_indexMap.find( last );
    if( k->first != last )
    { 
        this->m_storage[ k->first ] = this->m_storage[ last ];
        m_keyMap[ i->second ] = k->second;
        m_indexMap[ k->second ] = i->second;
    }
    m_keyMap.erase( k );
    m_indexMap.erase( i );    
    this->m_storage.popBack();
}

} } // namespace rur { namespace graphics {

#endif // RUR_GRAPHICS_MAP_HEADER_GUARD_
