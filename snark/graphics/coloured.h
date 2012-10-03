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

#ifndef RUR_GRAPHICS_COLOURED_H_
#define RUR_GRAPHICS_COLOURED_H_

#include <boost/array.hpp>
#include <comma/Visiting/traits.h>
#include <snark/graphics/Vertex.h>

namespace snark { namespace graphics {

template < typename T, typename S >
struct coloured // real quick and dirty
{
    T value;
    colour< S > colour;
    coloured() {}
    coloured( const T& value, const colour< S > = colours< S >::black() ) : value( value ), colour( colour ) {}
};

} } // namespace snark { namespace graphics {

namespace snark { namespace Visiting { 

/// visiting traits
template < typename T, typename S > struct traits< snark::graphics::coloured< T, S > >
{
    /// const visiting
    template < typename Key, class Visitor >
    static void visit( const Key&, const snark::graphics::coloured< T, S >& p, Visitor& v )
    {
        v.apply( "value", p.value );
        v.apply( "colour", p.colour );
    }
    
    /// const visiting
    template < typename Key, class Visitor >
    static void visit( const Key&, snark::graphics::coloured< T, S >& p, Visitor& v )
    {
        v.apply( "value", p.value );
        v.apply( "colour", p.colour );
    }    
};

} } // namespace snark { namespace Visiting {

#endif /*RUR_GRAPHICS_GL_COLOURED_H_*/
