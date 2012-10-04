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
#else
#include <signal.h>
#endif
#include <boost/filesystem/operations.hpp>
#include <boost/thread/thread_time.hpp>
#include "./Viewer.h"
#include <QTimer>

namespace snark { namespace graphics { namespace View {

Viewer::Viewer( const QColor4ub& background_color, double fov, bool z_up, bool orthographic,
                boost::optional< comma::csv::options > camera_csv, boost::optional< Eigen::Vector3d > cameraposition,
                boost::optional< Eigen::Vector3d > cameraorientation ) :
    qt3d::view( background_color, fov, z_up, orthographic ),
    m_lookAt( false ),
    m_cameraposition( cameraposition ),
    m_cameraorientation( cameraorientation )
{
    QTimer* timer = new QTimer( this );
    timer->start( 40 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( read() ) );
    if( camera_csv )
    {
        m_cameraReader.reset( new CameraReader( *camera_csv ) );
    }
    m_cameraFixed = m_cameraposition || m_cameraReader;
}


void Viewer::shutdown()
{
    m_shutdown = true;
    #ifdef WIN32
    ::raise( SIGTERM ); // according to windows docs, SIGINT does not work on windows
    #else // #ifdef WIN32
    ::raise( SIGINT ); // quick and dirty... or maybe not so dirty: to interrupt blocking read() in reader threads, if closed from qt window
    #endif // #ifdef WIN32
    if( m_cameraReader ) { m_cameraReader->shutdown(); }
    for( unsigned int i = 0; i < readers.size(); ++i ) { readers[i]->shutdown(); }
}

void Viewer::initializeGL( QGLPainter *painter )
{
    (void) painter;
    glEnable(GL_BLEND);
//     glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    qglClearColor( m_background_color.toColor() );
    if( m_cameraReader ) { m_cameraReader->start(); }
    for( unsigned int i = 0; i < readers.size(); ++i ) { readers[i]->start(); }
}

void Viewer::read()
{
    for( unsigned int i = 0; !m_offset && i < readers.size(); ++i )
    {
        if( readers[i]->empty() ) { continue; }
        Eigen::Vector3d p = readers[i]->somePoint();
        m_offset = std::fabs( p.x() ) > 1000 || std::fabs( p.y() ) > 1000 || std::fabs( p.z() ) > 1000 ? p : Eigen::Vector3d( 0, 0, 0 );
        std::cerr << "view-points: " << i << " scene offset (" << m_offset->transpose() << ")" << std::endl;
    }
    if( !m_offset ) { return; }
    for( unsigned int i = 0; i < readers.size(); ++i )
    {
        readers[i]->update( *m_offset );        
    }
    m_shutdown = true;
    for( unsigned int i = 0; m_shutdown && i < readers.size(); ++i )
    {
        m_shutdown &= readers[i]->isShutdown();
        // TODO std in is always opened 
    }

    if( !m_cameraReader && m_cameraposition )
    {
        setCameraPosition( *m_cameraposition, *m_cameraorientation );
        m_cameraposition.reset();
        m_cameraorientation.reset();
    }
    else if( m_cameraReader )
    {
        Eigen::Vector3d position = m_cameraReader->position();
        Eigen::Vector3d orientation = m_cameraReader->orientation();
        if( !m_cameraposition || !m_cameraposition->isApprox( position ) || !m_cameraorientation->isApprox( orientation ) )
        {
            m_cameraposition = position;
            m_cameraorientation = orientation;
            setCameraPosition( position, orientation );
        }
    }
    else if( readers[0]->m_extents && readers[0]->m_extents->size() > 0 && ( m_shutdown || readers[0]->m_extents->size() >= readers[0]->size / 10 ) )
    {
        QVector3D min( readers[0]->m_extents->min().x(), readers[0]->m_extents->min().y(), readers[0]->m_extents->min().z() );
        QVector3D max( readers[0]->m_extents->max().x(), readers[0]->m_extents->max().y(), readers[0]->m_extents->max().z() );
        updateView( min, max );
        if( !m_cameraFixed && !m_lookAt )
        {
            m_lookAt = true;
            lookAtCenter();
        }
    }
    if( !m_shutdown )
    {
        update();
    }
}


void Viewer::paintGL( QGLPainter *painter )
{    
    for( unsigned int i = 0; i < readers.size(); ++i )
    {
        if( !readers[i]->show() ) { continue; }
        if( readers[i]->pointSize > 1 ) { ::glEnable( GL_POINT_SMOOTH ); }
        ::glPointSize( readers[i]->pointSize );
        readers[i]->render( painter );
        if( readers[i]->pointSize > 1 ) { ::glDisable( GL_POINT_SMOOTH ); }
    }
        
    draw_coordinates( painter );
}

void Viewer::setCameraPosition ( const Eigen::Vector3d& position, const Eigen::Vector3d& orientation )
{
    Eigen::Vector3d p = position - *m_offset;
    if( m_z_up )
    {
        camera()->setUpVector( QVector3D( 0, 0, 1 ) );
    }
    else
    {
        camera()->setUpVector( QVector3D( 0, 0, -1 ) );
    }
    camera()->setEye( QVector3D( p.x(), p.y(), p.z() ) );
    double cos_yaw = std::cos( orientation.z() );
    double sin_yaw = std::sin( orientation.z() );
    double sin_pitch = std::sin( orientation.y() );
    double cos_pitch = std::cos( orientation.y() );
    Eigen::Vector3d direction( cos_pitch * cos_yaw, cos_pitch * sin_yaw, sin_pitch ); // todo: quick and dirty, forget about roll for now
    Eigen::Vector3d scene_centre = p + direction * 50; // quick and dirty
    Eigen::Vector3d where = p + direction;
    camera()->setCenter( QVector3D( where.x(), where.y(), where.z() ) );
    m_sceneCenter = QVector3D( scene_centre.x(), scene_centre.y(), scene_centre.z() );
}


} } } // namespace snark { namespace graphics { namespace View {
