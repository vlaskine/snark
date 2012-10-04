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

#ifndef SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_VERTEX_BUFFER_H_
#define SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_VERTEX_BUFFER_H_

#include <Qt3D/qvector3darray.h>
#include <Qt3D/qcolor4ub.h>

namespace snark { namespace graphics { namespace qt3d {

/// circular double buffer for vertices and color
class vertex_buffer
{
    public:
        vertex_buffer( std::size_t size );

        void addVertex( const QVector3D& point, const QColor4ub& color, unsigned int block = 0 );

        const QVector3DArray& points() const;
        const QArray<QColor4ub>& color() const;
        const unsigned int size() const;
        const unsigned int index() const;

    protected:
        QVector3DArray m_points;
        QArray<QColor4ub> m_color;
        unsigned int m_readIndex;
        unsigned int m_writeIndex;
        unsigned int m_readSize;
        unsigned int m_writeSize;
        unsigned int m_bufferSize;
        unsigned int m_block;
};

} } } // namespace snark { namespace graphics { namespace qt3d {

#endif /*SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_VERTEX_BUFFER_H_*/
