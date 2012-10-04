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

#ifndef SNARK_GRAPHICS_APPLICATIONS_LABELPOINTS_POINTWITHID_H_
#define SNARK_GRAPHICS_APPLICATIONS_LABELPOINTS_POINTWITHID_H_

#include <comma/base/types.h>
#include <comma/visiting/traits.h>
#include <snark/visiting/eigen.h>

namespace snark { namespace graphics { namespace View {

struct PointWithId // quick and dirty
{
    Eigen::Vector3d point;
    comma::uint32 id;
};

} } } // namespace snark { namespace graphics { namespace View {

namespace comma { namespace visiting {

template <> struct traits< snark::graphics::View::PointWithId >
{
    template < typename Key, class Visitor >
    static void visit( Key, snark::graphics::View::PointWithId& p, Visitor& v )
    {
        v.apply( "point", p.point );
        v.apply( "id", p.id );
    }
    
    template < typename Key, class Visitor >
    static void visit( Key, const snark::graphics::View::PointWithId& p, Visitor& v )
    {
        v.apply( "point", p.point );
        v.apply( "id", p.id );
    }    
};

} } // namespace comma { namespace visiting {

#endif // SNARK_GRAPHICS_APPLICATIONS_LABELPOINTS_POINTWITHID_H_
