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

#ifndef WIN32
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#endif
#include <deque>
#include <iostream>
#include <cmath>
#include <sstream>
#include <boost/bind.hpp>
#include <comma/Base/exception.h>
#include <comma/Base/Types.h>
#include <comma/Csv/Stream.h>
#include <comma/Io/Select.h>
#include "./CameraReader.h"

namespace rur { namespace graphics { namespace View {

CameraReader::CameraReader( Csv::Options& options )
    : options( options )
    , m_shutdown( false )
    , m_istream( options.filename, options.binary() ? Io::Mode::binary : Io::Mode::ascii )
{
}

void CameraReader::shutdown()
{
    if( m_shutdown ) { return; }
    m_shutdown = true;
    if( m_thread ) { m_thread->join(); }
    m_istream.close();
}

bool CameraReader::isShutdown() const { return m_shutdown; }

void CameraReader::read()
{
    while( !m_shutdown && readOnce() );
    std::cerr << "view-points: end of camera stream " << options.filename << std::endl;
    m_shutdown = true;
}

void CameraReader::start()
{
    m_thread.reset( new boost::thread( boost::bind( &CameraReader::read, boost::ref( *this ) ) ) );
}

bool CameraReader::readOnce()
{
    try
    {
        if( !m_stream ) // quick and dirty: handle named pipes
        {
            if( !m_istream() ) { return true; }
            m_stream.reset( new Csv::InputStream< PointWithorientation >( *m_istream, options ) );
        }
        const PointWithorientation* p = m_stream->read();
        if( p == NULL ) { m_shutdown = true; return false; }
        boost::recursive_mutex::scoped_lock lock( m_mutex );
        m_position = p->point;
        m_orientation = p->orientation;
        return true;
    }
    catch( std::exception& ex ) { std::cerr << "view-points: " << ex.what() << std::endl; }
    catch( ... ) { std::cerr << "view-points: unknown exception" << std::endl; }
    return false;
}

Eigen::Vector3d CameraReader::position() const
{
    boost::recursive_mutex::scoped_lock lock( m_mutex );
    return m_position;
};

Eigen::Vector3d CameraReader::orientation() const
{
    boost::recursive_mutex::scoped_lock lock( m_mutex );
    return m_orientation;
};

} } } // namespace rur { namespace graphics { namespace View {
