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

#include <cmath>
#include <vector>
#include <boost/array.hpp> 
#include <boost/lexical_cast.hpp> 
#include <comma/Math/Compare.h>
#include <comma/Math/Math.h>
#include "./Viewer.h"

namespace snark { namespace graphics { namespace View {

Viewer::Viewer( const std::vector< Csv::Options >& options
              , bool labelDuplicated
              , const QColor4ub& background_color
              , bool orthographic, double fieldOfView )
    : Qt3D::View( background_color, fieldOfView, false, orthographic )
    , navigate( *this )
    , pickId( *this )
    , selectPartition( *this )
    , selectId( *this )
    , selectClip( *this )
    , fill( *this )
    , m_currentTool( &navigate )
    , m_options( options )
    , m_labelDuplicated( labelDuplicated )
{

}
              
void Viewer::saveStateToFile() {}
              
void Viewer::show( std::size_t i, bool visible )
{
    m_datasets[i]->visible( visible );
    update();
}

void Viewer::setWritable( std::size_t i, bool writable )
{
    m_datasets[i]->writable( writable );
}

void Viewer::save()
{
    for( std::size_t i = 0; i < m_datasets.size(); ++i )
    {
        if( m_datasets[i]->writable() ) { m_datasets[i]->save(); }
    }
}

const std::vector< boost::shared_ptr< Dataset > >& Viewer::datasets() const { return m_datasets; }

Dataset& Viewer::dataset( std::size_t index ) { return *m_datasets[index]; }
    
const Dataset& Viewer::dataset( std::size_t index ) const { return *m_datasets[index]; }

void Viewer::reload()
{
    for( std::size_t i = 0; i < m_datasets.size(); ++i )
    {
        std::string filename = m_datasets[i]->filename();
        Csv::Options options = m_datasets[i]->options();
        bool writable = m_datasets[i]->writable();
        bool visible = m_datasets[i]->visible();
        m_datasets[i].reset();
        m_datasets[i].reset( new Dataset( filename, options, *m_offset, m_labelDuplicated ) );
        if( !m_datasets[i]->valid() ) { std::cerr << "label-points: failed to reload datasets" << std::endl; exit( -1 ); }
        m_datasets[i]->init();
        m_datasets[i]->writable( writable );
        m_datasets[i]->visible( visible );
    }
    setCamera();
    update();
}

void Viewer::setCamera()
{
    Extents< Eigen::Vector3d > extents;
    for( std::size_t i = 0; i < m_datasets.size(); ++i ) { extents.add( m_datasets[i]->extents() ); }
    extents.add( extents.min() - Eigen::Vector3d( 10, 10, 10 ) ); // expand a bit
    extents.add( extents.max() + Eigen::Vector3d( 10, 10, 10 ) ); // expand a bit
    Eigen::Vector3d minCorner = extents.min() - m_datasets[0]->offset();
    Eigen::Vector3d maxCorner = extents.max() - m_datasets[0]->offset();
    QVector3D min( minCorner.x(), minCorner.y(), minCorner.z() );
    QVector3D max( maxCorner.x(), maxCorner.y(), maxCorner.z() );
    updateView( min, max );
    lookAtCenter();
    updateZFar();
}

void Viewer::initializeGL( QGLPainter *painter )
{
    ::glDisable( GL_LIGHTING );
//     setBackgroundColor( QColor( m_background_color.red(), m_background_color.green(), m_background_color.blue() ) );
    m_datasets.push_back( boost::shared_ptr< Dataset >( new Dataset( m_options[0].filename, m_options[0], m_labelDuplicated ) ) );
    if( !m_datasets[0]->valid() ) { std::cerr << "label-points: failed to load dataset " << m_options[0].filename << std::endl; exit( -1 ); }
    m_datasets[0]->init();
    m_datasets[0]->writable( true );
    m_datasets[0]->visible( true );
    m_offset = m_datasets[0]->offset();
    for( std::size_t i = 1; i < m_options.size(); ++i )
    {
        m_datasets.push_back( boost::shared_ptr< Dataset >( new Dataset( m_options[i].filename, m_options[i], *m_offset, m_labelDuplicated ) ) );
        if( !m_datasets.back()->valid() ) { std::cerr << "label-points: failed to load dataset " << m_options[i].filename << std::endl; exit( -1 ); }
        m_datasets.back()->init();
        m_datasets.back()->writable( false );
        m_datasets.back()->visible( true );
    }
    setCamera();
}

void Viewer::paintGL( QGLPainter *painter )
{
    updateZFar();
    static bool emitted = false;
    if( !emitted ) { emitted = true; emit initialized(); } // real quick and dirty: somehow emit fails in init()
    ::glPointSize( 1 );
    int selectionPointSize = 1;
    for( std::size_t i = 0; i < m_datasets.size(); ++i )
    {
        m_datasets[i]->draw( painter );
        if( m_datasets[i]->visible() ) { selectionPointSize = 3; }
    }
    ::glEnable( GL_POINT_SMOOTH );
    ::glPointSize( selectionPointSize );
    for( std::size_t i = 0; i < m_datasets.size(); ++i ) { m_datasets[i]->selection().draw( painter ); }
    ::glDisable( GL_POINT_SMOOTH );
    m_currentTool->draw( painter );
    draw_coordinates( painter );
}

void Viewer::mousePressEvent( QMouseEvent* e )
{
    m_currentTool->onMousePress( e );
//     GL::View::mousePressEvent( e );  
}

void Viewer::mouseReleaseEvent( QMouseEvent* e )
{
    m_currentTool->onMouseRelease( e );
//     GL::View::mouseReleaseEvent( e );
}

void Viewer::mouseMoveEvent( QMouseEvent *e )
{
    m_currentTool->onMouseMove( e );
//     GL::View::mouseMoveEvent( e );
}

boost::optional< std::pair< Eigen::Vector3d, comma::UInt32 > > Viewer::pointSelection( const QPoint& point, bool writableOnly )
{
    boost::optional< std::pair< Eigen::Vector3d, comma::UInt32 > > result;
    boost::optional< QVector3D > point3d = getPoint( point );
    if( point3d )
    {
        Eigen::Vector3d p(  point3d->x(), point3d->y(), point3d->z() );
        if( m_offset )
        {
            p += *m_offset;
        }
        std::cerr << " clicked point " << p.transpose() << std::endl;

        Extents< Eigen::Vector3d > e;
        e.add( p - Eigen::Vector3d::Ones() );
        e.add( p + Eigen::Vector3d::Ones() );
        double minDistanceSquare = std::numeric_limits< double >::max();
        for( std::size_t i = 0; i < m_datasets.size(); ++i )
        {
            if( !m_datasets[i]->visible() || ( writableOnly && !m_datasets[i]->writable() ) ) { continue; }
            Dataset::Points m = m_datasets[i]->points().find( e.min(), e.max() );
            std::cerr << " found " << m.size() << " points " << std::endl;
            for( Dataset::Points::ConstEnumerator en = m.begin(); !en.end(); ++en )
            {
                double norm = ( en.key() - p ).squaredNorm();
                if( norm < minDistanceSquare )
                {
                    minDistanceSquare = norm;
                    result = std::make_pair( en.key(), en.value().id );
                }
            }
            if( minDistanceSquare <= 0.01 )
            {
                std::cerr << " found point " << result->first << " , id " << result->second << std::endl;
                return result;
            }
        }
    }
    
    return boost::optional< std::pair< Eigen::Vector3d, comma::UInt32 > >();
}

void Viewer::handleId( comma::UInt32 id ) { m_id = id; }

} } } // namespace snark { namespace graphics { namespace View {
