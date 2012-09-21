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

#ifndef RUR_GRAPHICS_GL_VERTEX_HEADER_GUARD_
#define RUR_GRAPHICS_GL_VERTEX_HEADER_GUARD_

#include <rur/graphics/point.h>
#include <rur/graphics/colours.h>

namespace rur { namespace graphics {

template < typename T, typename S = T >
class Vertex
{
    public:
        Vertex() {}
        Vertex( const Vertex& rhs ) { this->operator=( rhs ); }
        Vertex( const point_xyz< T >& p ) : m_point( p ), m_colour( colours< S >::Black() ) {}
        Vertex( const point_xyz< T >& p, const graphics::colour< S >& c ) : m_point( p ), m_colour( c ) {} 
               
        const point_xyz< T >& point() const { return m_point; }
        const graphics::colour< S >& colour() const { return m_colour; }
        point_xyz< T >& point() { return m_point; }
        graphics::colour< S >& colour() { return m_colour; }
        
    private: 
        point_xyz< T > m_point;
        graphics::colour< S > m_colour;
};

} } // namespace rur { namespace graphics {

#endif // RUR_GRAPHICS_GL_VERTEX_HEADER_GUARD_

