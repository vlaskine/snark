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

#include <Eigen/Core>
#include <snark/graphics/qt3d/rotation_matrix.h>
#include "./TextureReader.h"
#include "./Texture.h"

namespace snark { namespace graphics { namespace View {

/// constructor
/// @param viewer reference to the viewer
/// @param options csv options for the position input
/// @param file image filename
/// @param width image width in meters to be displayed in the scene
/// @param height image height in meters to be displayed in the scene
TextureReader::TextureReader( QGLView& viewer, comma::csv::options& options, const std::string& file, double width, double height )
    : Reader( viewer, options, 1, NULL, 1, "", QVector3D( 0, 1, 1 ) ), 
      m_file( file ),
      m_image( file.c_str() )
{
    m_texture.setImage( m_image );
    m_material.setTexture( &m_texture );

    QVector3D a( 0, 0, 0 );
    QVector3D b( width, 0, 0 );
    QVector3D c( width, height, 0 );
    QVector3D d( 0, height, 0 );
    QVector2D ta( 0, 0 );
    QVector2D tb( 1, 0 );
    QVector2D tc( 1, 1 );
    QVector2D td( 0, 1 );
    m_geometry.appendVertex( a, b, c, d );
    m_geometry.appendTexCoord(ta, tb, tc, td);
    m_builder.addQuads( m_geometry );
    m_node = m_builder.finalizedSceneNode();
    m_node->setMaterial( &m_material );
}

void TextureReader::start()
{        
    m_extents = snark::graphics::extents< Eigen::Vector3f >();

    m_thread.reset( new boost::thread( boost::bind( &Reader::read, boost::ref( *this ) ) ) );
}

void TextureReader::update( const Eigen::Vector3d& offset )
{
    updatePoint( offset );
}

bool TextureReader::empty() const
{
    return !m_point;
}

const Eigen::Vector3d& TextureReader::somePoint() const
{
    boost::mutex::scoped_lock lock( m_mutex );
    return *m_point;
}

void TextureReader::render( QGLPainter* painter )
{
    painter->setStandardEffect( QGL::FlatReplaceTexture2D );
    painter->modelViewMatrix().push();
    painter->modelViewMatrix().translate( m_translation );
    painter->modelViewMatrix().rotate( m_quaternion );
    
    m_node->draw(painter);
    painter->modelViewMatrix().pop();
}

bool TextureReader::readOnce()
{
    if( !m_stream ) // quick and dirty: handle named pipes
    {
        if( !m_istream() ) { return true; }
        m_stream.reset( new comma::csv::input_stream< PointWithId >( *m_istream(), options ) );
    }
    const PointWithId* p = m_stream->read();
    if( p == NULL ) { m_shutdown = true; return false; }
    boost::mutex::scoped_lock lock( m_mutex );
    m_point = p->point;
    m_orientation = p->orientation;
    return true;
}


} } } // namespace snark { namespace graphics { namespace View {
    
