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
#include <comma/base/exception.h>
#include <comma/base/types.h>
#include <comma/csv/stream.h>
#include <comma/io/select.h>
#include "./CameraReader.h"

namespace snark { namespace graphics { namespace View {

CameraReader::CameraReader( comma::csv::options& options )
    : options( options )
    , m_shutdown( false )
    , m_istream( options.filename, options.binary() ? comma::io::mode::binary : comma::io::mode::ascii )
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
            m_stream.reset( new comma::csv::input_stream< point_with_orientation >( *m_istream, options ) );
        }
        const point_with_orientation* p = m_stream->read();
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

} } } // namespace snark { namespace graphics { namespace View {
