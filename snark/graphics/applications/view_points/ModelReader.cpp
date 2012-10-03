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
#include <comma/Math/rotation_matrix.h>
#include "./ModelReader.h"
#include "./Texture.h"

namespace snark { namespace graphics { namespace View {

/// constructor
/// @param viewer reference to the viewer
/// @param options csv options for the position input
/// @param file model filename
/// @param z_up z axis is pointing up in model coordinates
/// @param c color used for the label
/// @param label text displayed as label
ModelReader::ModelReader( QGLView& viewer, comma::csv::Options& options, const std::string& file, bool z_up, snark::graphics::View::coloured* c, const std::string& label )
    : Reader( viewer, options, 1, c, 1, label, QVector3D( 0, 1, 1 ) ), // TODO make offset configurable ?
      m_file( file ),
      m_z_up( z_up )
{
}

void ModelReader::start()
{
    if( m_file.substr( m_file.size() - 3, 3 ) == "ply" )
    {
        m_plyLoader = PlyLoader( m_file );
    }
    
    if( !m_plyLoader )
    {
        m_scene = QGLAbstractScene::loadScene( QLatin1String( m_file.c_str() ) );
    }
    m_extents = comma::Extents< Eigen::Vector3f >();

    m_thread.reset( new boost::thread( boost::bind( &Reader::read, boost::ref( *this ) ) ) );
}

void ModelReader::update( const Eigen::Vector3d& offset )
{
    updatePoint( offset );
}

bool ModelReader::empty() const
{
    return !m_point;
}

const Eigen::Vector3d& ModelReader::somePoint() const
{
    boost::mutex::scoped_lock lock( m_mutex );
    return *m_point;
}

void ModelReader::render( QGLPainter* painter )
{
    painter->modelViewMatrix().push();
    painter->modelViewMatrix().translate( m_translation );
    painter->modelViewMatrix().rotate( m_quaternion );
    if( !m_z_up )
    {
        painter->modelViewMatrix().rotate( 180, 1, 0, 0 );
    }
    if( m_plyLoader )
    {
        m_plyLoader->draw( painter );
    }
    else
    {
        QGLSceneNode* node = m_scene->mainNode();
    //     painter->setStandardEffect( QGL::LitMaterial ); // no effect ?
        node->draw(painter);
    }
    painter->modelViewMatrix().pop();
    if( !m_label.empty() )
    {
        drawLabel( painter, m_translation );
    }
}

bool ModelReader::readOnce()
{
    if( !m_stream ) // quick and dirty: handle named pipes
    {
        if( !m_istream() ) { return true; }
        m_stream.reset( new csv::InputStream< PointWithId >( *m_istream(), options ) );
    }
    const PointWithId* p = m_stream->read();
    if( p == NULL ) { m_shutdown = true; return false; }
    boost::mutex::scoped_lock lock( m_mutex );
    m_point = p->point;
    m_orientation = p->orientation;
    m_color = m_colored->color( p->point, p->id, p->scalar, p->color );
    return true;
}


} } } // namespace snark { namespace graphics { namespace View {
    