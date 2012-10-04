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

#ifndef SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_SHAPE_READER_H_
#define SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_SHAPE_READER_H_

#ifdef WIN32
#include <winsock2.h>

//#include <windows.h>
#endif

#include "./Reader.h"
#include "./ShapeWithId.h"

namespace snark { namespace graphics { namespace View {

template< typename S >    
class ShapeReader : public Reader
{
    public:
        ShapeReader( QGLView& viewer, comma::csv::options& options, std::size_t size, coloured* c, unsigned int pointSize, const std::string& label );

        void start();
        void update( const Eigen::Vector3d& offset );
        const Eigen::Vector3d& somePoint() const;
        bool readOnce();
        void render( QGLPainter *painter = NULL );
        bool empty() const;

    private:        
        typedef std::deque< ShapeWithId< S > > DequeType;
        DequeType m_deque;
        mutable boost::mutex m_mutex;
        boost::scoped_ptr< comma::csv::input_stream< ShapeWithId< S > > > m_stream;
        qt3d::vertex_buffer m_buffer;
        std::vector< std::pair< QVector3D, std::string > > m_labels;
        unsigned int m_labelIndex;
        unsigned int m_labelSize;
};


template< typename S >    
ShapeReader< S >::ShapeReader( QGLView& viewer, comma::csv::options& options, std::size_t size, coloured* c, unsigned int pointSize, const std::string& label  ):
    Reader( viewer, options, size, c, pointSize, label ),
    m_buffer( size * Shapetraits< S >::size ),
    m_labels( size ),
    m_labelIndex( 0 ),
    m_labelSize( 0 )
{
}

template< typename S >
inline void ShapeReader< S >::start()
{
    m_extents = snark::graphics::extents< Eigen::Vector3f >();
    m_thread.reset( new boost::thread( boost::bind( &Reader::read, boost::ref( *this ) ) ) );
}

template< typename S >
inline void ShapeReader< S >::update( const Eigen::Vector3d& offset )
{
    boost::mutex::scoped_lock lock( m_mutex );
    for( typename DequeType::iterator it = m_deque.begin(); it != m_deque.end(); ++it )
    {
        Shapetraits< S >::update( it->shape, offset, it->color, it->block, m_buffer, m_extents );
    }
    m_deque.clear();
    updatePoint( offset );
}

template< typename S >
inline bool ShapeReader< S >::empty() const
{
    boost::mutex::scoped_lock lock( m_mutex );
    return m_deque.empty();
}

template< typename S >
inline const Eigen::Vector3d& ShapeReader< S >::somePoint() const
{
    boost::mutex::scoped_lock lock( m_mutex );
    return Shapetraits< S >::somePoint( m_deque.front().shape );
}

template< typename S >
inline void ShapeReader< S >::render( QGLPainter* painter )
{
    painter->setStandardEffect(QGL::FlatPerVertexColor);
    painter->clearAttributes();
    painter->setVertexAttribute(QGL::Position, m_buffer.points() );
    painter->setVertexAttribute(QGL::Color, m_buffer.color() );

    Shapetraits< S >::draw( painter, m_buffer.size(), m_buffer.index() );
    for( unsigned int i = 0; i < m_labelSize; i++ )
    {
        drawLabel( painter, m_labels[ i ].first, m_labels[ i ].second );
    }
    if( !m_label.empty() )
    {
        drawLabel( painter, m_translation );
    }
}

template< typename S >
inline bool ShapeReader< S >::readOnce()
{
    try
    {
        if( !m_stream ) // quick and dirty: handle named pipes
        {
            if( !m_istream() )
            {
#ifndef WIN32
                // HACK poll on blocking pipe
                ::usleep( 1000 );
#endif
                return true;                
            }
            m_stream.reset( new comma::csv::input_stream< ShapeWithId< S > >( *m_istream(), options ) );
        }
        const ShapeWithId< S >* p = m_stream->read();
        if( p == NULL )
        {
            m_shutdown = true;
            return false;            
        }
        ShapeWithId< S > v = *p;
        Eigen::Vector3d centre = Shapetraits< S >::centre( v.shape );
        if( !v.label.empty() )
        {
            m_labels[ m_labelIndex ] = std::make_pair( QVector3D( centre.x(), centre.y(), centre.z() ) , v.label );
            m_labelIndex++;
            if( m_labelSize < m_labels.size() )
            {
                m_labelSize++;
            }
            if( m_labelIndex > m_labels.size() )
            {
                m_labelIndex = 0;
            }
        }
        v.color = m_colored->color( centre, p->id, p->scalar, p->color );
        boost::mutex::scoped_lock lock( m_mutex );
        m_deque.push_back( v );
        m_point = Shapetraits< S >::somePoint( v.shape );
        m_color = v.color;
        return true;
    }
    catch( std::exception& ex ) { std::cerr << "view-points: " << ex.what() << std::endl; }
    catch( ... ) { std::cerr << "view-points: unknown exception" << std::endl; }
    return false;
}

} } } // namespace snark { namespace graphics { namespace View {

#endif // SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_SHAPE_READER_H_
