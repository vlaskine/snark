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

#ifndef RUR_GRAPHICS_COLOURS_HEADER_GUARD_
#define RUR_GRAPHICS_COLOURS_HEADER_GUARD_

#include <comma/base/exception.h>
#include <comma/math/compare.h>
#include <rur/graphics/colour.h>

namespace rur { namespace graphics {

template < typename T >
struct colours
{
    static colour< T > red( T a = colour_traits< T >::max() ) { return colour< T >( colour_traits< T >::max(), colour_traits< T >::min(), colour_traits< T >::min(), a ); }
    static colour< T > green( T a = colour_traits< T >::max() ) { return colour< T >( colour_traits< T >::min(), colour_traits< T >::max(), colour_traits< T >::min(), a ); }
    static colour< T > blue( T a = colour_traits< T >::max() ) { return colour< T >( colour_traits< T >::min(), colour_traits< T >::min(), colour_traits< T >::max(), a ); }
    static colour< T > white( T a = colour_traits< T >::max() ) { return colour< T >( colour_traits< T >::max(), colour_traits< T >::max(), colour_traits< T >::max(), a ); }
    static colour< T > black( T a = colour_traits< T >::max() ) { return colour< T >( colour_traits< T >::min(), colour_traits< T >::min(), colour_traits< T >::min(), a ); }
    static colour< T > cyan( T a = colour_traits< T >::max() ) { return colour< T >( colour_traits< T >::min(), colour_traits< T >::max(), colour_traits< T >::max(), a ); }
    static colour< T > yellow( T a = colour_traits< T >::max() ) { return colour< T >( colour_traits< T >::max(), colour_traits< T >::max(), colour_traits< T >::min(), a ); }
    static colour< T > magenta( T a = colour_traits< T >::max() ) { return colour< T >( colour_traits< T >::max(), colour_traits< T >::min(), colour_traits< T >::max(), a ); }
};

} } // namespace rur { namespace graphics {

#endif /*RUR_GRAPHICS_COLOURS_HEADER_GUARD_*/
