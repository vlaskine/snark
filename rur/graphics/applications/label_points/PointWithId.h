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

#ifndef RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_POINTWITHID_H_
#define RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_POINTWITHID_H_

#include <comma/Base/Types.h>
#include <comma/Eigen/Visiting.h>
#include <comma/Visiting/Apply.h>
#include <comma/Visiting/Visit.h>

namespace rur { namespace graphics { namespace View {

struct PointWithId // quick and dirty
{
    Eigen::Vector3d point;
    UInt32 id;
};

} } } // namespace rur { namespace graphics { namespace View {

namespace rur { namespace Visiting {

template <> struct traits< comma::graphics::View::PointWithId >
{
    template < typename Key, class Visitor >
    static void visit( Key, comma::graphics::View::PointWithId& p, Visitor& v )
    {
        v.apply( "point", p.point );
        v.apply( "id", p.id );
    }
    
    template < typename Key, class Visitor >
    static void visit( Key, const comma::graphics::View::PointWithId& p, Visitor& v )
    {
        v.apply( "point", p.point );
        v.apply( "id", p.id );
    }    
};

} } // namespace rur { namespace Visiting {

#endif // RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_POINTWITHID_H_
