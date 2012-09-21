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

#ifndef RUR_GRAPHICS_COLOURED_H_
#define RUR_GRAPHICS_COLOURED_H_

#include <boost/array.hpp>
#include <comma/Visiting/traits.h>
#include <rur/graphics/Vertex.h>

namespace rur { namespace graphics {

template < typename T, typename S >
struct coloured // real quick and dirty
{
    T value;
    colour< S > colour;
    coloured() {}
    coloured( const T& value, const colour< S > = colours< S >::black() ) : value( value ), colour( colour ) {}
};

} } // namespace rur { namespace graphics {

namespace rur { namespace Visiting { 

/// visiting traits
template < typename T, typename S > struct traits< comma::graphics::coloured< T, S > >
{
    /// const visiting
    template < typename Key, class Visitor >
    static void visit( const Key&, const comma::graphics::coloured< T, S >& p, Visitor& v )
    {
        v.apply( "value", p.value );
        v.apply( "colour", p.colour );
    }
    
    /// const visiting
    template < typename Key, class Visitor >
    static void visit( const Key&, comma::graphics::coloured< T, S >& p, Visitor& v )
    {
        v.apply( "value", p.value );
        v.apply( "colour", p.colour );
    }    
};

} } // namespace rur { namespace Visiting {

#endif /*RUR_GRAPHICS_GL_COLOURED_H_*/
