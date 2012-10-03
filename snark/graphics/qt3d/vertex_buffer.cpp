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

#include "./vertex_buffer.h"


namespace snark { namespace graphics { namespace Qt3D {

vertex_buffer::vertex_buffer ( std::size_t size ):
    m_readIndex( 0 ),
    m_writeIndex( 0 ),
    m_readSize( 0 ),
    m_writeSize( 0 ),
    m_bufferSize( size ),
    m_block( 0 )
{
    m_points.resize( 2 * size );
    m_color.resize( 2 * size );
}

void vertex_buffer::addVertex ( const QVector3D& point, const QColor4ub& color, unsigned int block )
{
    if( block != m_block )
    {
        m_block = block;

        m_writeIndex += m_bufferSize;
        m_writeIndex %= 2 * m_bufferSize;
        if( m_readIndex == m_writeIndex )
        {
            m_readIndex+= m_bufferSize;
            m_readIndex %= 2 * m_bufferSize;
            m_readSize = m_writeSize;
        }
        m_writeSize = 0;
    }
    m_points[ m_writeIndex + m_writeSize ] = point;
    m_color[ m_writeIndex + m_writeSize ] = color;
    m_writeSize++;
    if( block == 0 )
    {
        m_readSize++;
    }
    if( m_writeSize > m_bufferSize )
    {
        m_writeSize = 0;
        m_readSize = m_bufferSize;
    }
}

const QVector3DArray& vertex_buffer::points() const
{
    return m_points;
}

const QArray< QColor4ub >& vertex_buffer::color() const
{
    return m_color;
}

const unsigned int vertex_buffer::size() const
{
    return m_readSize;
}

const unsigned int vertex_buffer::index() const
{
    return m_readIndex;
}


    
} } } // namespace snark { namespace graphics { namespace View {
