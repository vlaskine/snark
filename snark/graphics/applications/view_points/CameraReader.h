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

#ifndef SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_CAMERAREADER_H_
#define SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_CAMERAREADER_H_

#include <deque>
#include <iostream>
#include <cmath>
#include <sstream>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <comma/csv/stream.h>
#include <comma/io/stream.h>
#include <snark/visiting/eigen.h>

namespace snark { namespace graphics { namespace View {

struct point_with_orientation // quick and dirty
{
    Eigen::Vector3d point;
    Eigen::Vector3d orientation;
};

    
class CameraReader
{
    public:
        const comma::csv::options options;

        CameraReader( comma::csv::options& options );

        void start();
        bool readOnce();
        bool ready() const { return m_stream->ready(); }
        void render();

        bool isShutdown() const;
        void shutdown();
        Eigen::Vector3d position() const;
        Eigen::Vector3d orientation() const;
        void read();

    private:
        bool m_shutdown;
        comma::io::istream m_istream;
        Eigen::Vector3d m_position;
        Eigen::Vector3d m_orientation;
        boost::scoped_ptr< comma::csv::input_stream< point_with_orientation > > m_stream;
        mutable boost::recursive_mutex m_mutex; // todo: make lock-free
        boost::scoped_ptr< boost::thread > m_thread;

};

} } } // namespace snark { namespace graphics { namespace View {

namespace comma { namespace visiting {

template <> struct traits< snark::graphics::View::point_with_orientation >
{
    template < typename Key, class Visitor >
    static void visit( Key, snark::graphics::View::point_with_orientation& p, Visitor& v )
    {
        v.apply( "point", p.point );
        v.apply( "roll", p.orientation.x() );
        v.apply( "pitch", p.orientation.y() );
        v.apply( "yaw", p.orientation.z() );
    }

    template < typename Key, class Visitor >
    static void visit( Key, const snark::graphics::View::point_with_orientation& p, Visitor& v )
    {
        v.apply( "point", p.point );
        v.apply( "roll", p.orientation.x() );
        v.apply( "pitch", p.orientation.y() );
        v.apply( "yaw", p.orientation.z() );
    }
};

} } // namespace comma { namespace visiting {
    
#endif /*SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_CAMERAREADER_H_*/
