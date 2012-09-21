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

#ifndef RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_POINTWITHID_H_
#define RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_POINTWITHID_H_

#include <comma/Base/Types.h>
#include <comma/Eigen/Visiting.h>
#include <Qt3D/qcolor4ub.h>
#include "./ShapeWithId.h"

namespace rur { namespace graphics { namespace View {

struct PointWithId // quick and dirty
{
    PointWithId() : id( 0 ), block( 0 ) {}
    Eigen::Vector3d point;
    Eigen::Vector3d orientation;
    UInt32 id;
    UInt32 block;
    QColor4ub color;
    std::string label;
    double scalar;
};

} } } // namespace rur { namespace graphics { namespace View {

namespace rur { namespace Visiting {

template <> struct traits< comma::graphics::View::PointWithId >
{
    template < typename Key, class Visitor >
    static void visit( Key, comma::graphics::View::PointWithId& p, Visitor& v )
    {
        v.apply( "point", p.point );
        v.apply( "roll", p.orientation.x() );
        v.apply( "pitch", p.orientation.y() );
        v.apply( "yaw", p.orientation.z() );
        v.apply( "id", p.id );
        v.apply( "block", p.block );
        v.apply( "colour", p.color );
        v.apply( "label", p.label );
        v.apply( "scalar", p.scalar );
    }

    template < typename Key, class Visitor >
    static void visit( Key, const comma::graphics::View::PointWithId& p, Visitor& v )
    {
        v.apply( "point", p.point );
        v.apply( "roll", p.orientation.x() );
        v.apply( "pitch", p.orientation.y() );
        v.apply( "yaw", p.orientation.z() );
        v.apply( "id", p.id );
        v.apply( "block", p.block );
        v.apply( "color", p.color );
        v.apply( "label", p.label );
        v.apply( "scalar", p.scalar );
    }
};


} } // namespace rur { namespace Visiting {

#endif /*RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_POINTWITHID_H_*/
