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
#include <fstream>
#include <iostream>
#include <cmath>
#include <sstream>
#include <boost/bind.hpp>
#include <Eigen/Core>
#include <comma/base/exception.h>
#include <comma/base/types.h>
#include <comma/csv/stream.h>
#include <comma/io/select.h>
#include <snark/graphics/qt3d/rotation_matrix.h>
#include "./Reader.h"
#include "./Texture.h"

namespace snark { namespace graphics { namespace View {

Reader::Reader( QGLView& viewer, comma::csv::options& options, std::size_t size, coloured* c, unsigned int pointSize, const std::string& label, const QVector3D& offset )
    : size( size )
    , pointSize( pointSize )
    , options( options )
    , m_viewer( viewer )
    , m_colored( c )
    , m_shutdown( false )
    , m_show( true )
    , m_istream( options.filename, options.binary() ? comma::io::mode::binary : comma::io::mode::ascii, comma::io::mode::non_blocking )
    , m_label( label )
    , m_offset( offset )
{
    std::vector< std::string > v = comma::split( options.fields, ',' ); // quick and dirty
}

void Reader::shutdown()
{
    m_shutdown = true;
    if( m_thread ) { m_thread->join(); }
    m_istream.close();
}

bool Reader::isShutdown() const { return m_shutdown; }

void Reader::show( bool s ) { m_show = s; }

bool Reader::show() const { return m_show; }

void Reader::read()
{
    while( !m_shutdown && readOnce() );
    std::cerr << "view-points: end of " << options.filename << std::endl;
    m_shutdown = true;
}

void Reader::updatePoint( const Eigen::Vector3d& offset )
{
    if( m_point )
    {
        boost::mutex::scoped_lock lock( m_mutex );
        m_translation = QVector3D( m_point->x() - offset.x(), m_point->y() - offset.y(), m_point->z() - offset.z() );
        if( m_orientation )
        {
            const Eigen::Quaterniond& q = snark::rotation_matrix( *m_orientation ).quaternion();
            m_quaternion = QQuaternion( q.w(), q.x(), q.y(), q.z() );
        }
    }
}

void Reader::drawLabel( QGLPainter *painter, const QVector3D& position, const std::string& label )
{
    painter->modelViewMatrix().push();
    painter->modelViewMatrix().translate( position );

    QMatrix4x4 world = painter->modelViewMatrix().top();
    Eigen::Matrix3d R;
    R << world( 0, 0 ) , world( 0, 1 ), world( 0, 2 ),
            world( 1, 0 ) , world( 1, 1 ), world( 1, 2 ),
            world( 2, 0 ) , world( 2, 1 ), world( 2, 2 );
    R.transposeInPlace();
    snark::rotation_matrix rotation( R );
    Eigen::Quaterniond q = rotation.quaternion();
    painter->modelViewMatrix().rotate( QQuaternion( q.w(), q.x(), q.y(), q.z() ) );
    painter->modelViewMatrix().translate( m_offset );
    double scale = 1 / ( double ) m_viewer.height();
    if( m_viewer.camera()->projectionType() == QGLCamera::Orthographic )
    {
        scale *= 0.25 * m_viewer.camera()->viewSize().width();
    }
    else
    {
        Eigen::Vector3d t( world( 0, 3 ) , world( 1, 3 ), world( 2, 3 ) );
        scale *= 0.2 * t.norm();
    }
    painter->modelViewMatrix().scale( scale ); // TODO make size configurable ?
    drawText( painter, label.c_str(), m_color );
    painter->modelViewMatrix().pop();
}

void Reader::drawLabel( QGLPainter *painter, const QVector3D& position )
{
    drawLabel( painter, position, m_label );
}


void Reader::drawText( QGLPainter *painter, const QString& string, const QColor4ub& color )
{
    Texture texture( string, color );
    texture.draw( painter );
}

} } } // namespace snark { namespace graphics { namespace View {
