// This file is part of snark, a generic and flexible library 
// for robotics research.
//
// Copyright (C) 2011 The University of Sydney
//
// snark is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// snark is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License 
// for more details.
//
// You should have received a copy of the GNU General Public
// License along with snark. If not, see <http://www.gnu.org/licenses/>.

#ifndef SNARK_GRAPHICS_VECTOR_HEADER_GUARD_
#define SNARK_GRAPHICS_VECTOR_HEADER_GUARD_

#include <snark/graphics/impl/renderable.h>

namespace snark { namespace graphics {

template < typename T >
struct Vector : public impl::renderable< T >
{
    Vector( std::size_t capacity ) : impl::renderable< T >( capacity ) {}
    
    std::size_t capacity() const { return this->m_storage.capacity(); }
    
    std::size_t size() const { return this->m_storage.size(); }
    
    bool empty() const { return this->m_storage.empty(); }
    
    void clear() { this->m_storage.clear(); }
    
    void push_back( const T& t ) { this->m_storage.pushBack( t ); }
    
    /// @todo a temporary poor man's set function, definitely get a better
    ///       semantics, i.e. instead of element in GL buffer,
    ///       define buffer handling per type (e.g. Vertex, Triangle, etc)
    void set( std::size_t i, const T& t ) { this->m_storage[i] = t; }
};

} } // namespace snark { namespace graphics {

#endif // SNARK_GRAPHICS_VECTOR_HEADER_GUARD_
