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

#ifndef RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_READER_H_
#define RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_READER_H_

#include <deque>
#include <fstream>
#include <iostream>
#include <cmath>
#include <sstream>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <comma/Base/Types.h>
#include <comma/csv/Options.h>
#include <comma/csv/Stream.h>
#include <comma/Io/FileDescriptor.h>
#include <comma/Io/Stream.h>
#include <comma/Math/Extents.h>
#include <comma/Sync/Synchronized.h>
#include <snark/graphics/Queue.h>
#include "./coloured.h"
#include "./PointWithId.h"
#include <snark/graphics/Qt3D/vertex_buffer.h>
#include <Qt3D/qglview.h>

class QGLAbstractScene;

namespace snark { namespace graphics { namespace View {

class Viewer;

class Reader
{
    public:
        const std::size_t size;
        const unsigned int pointSize;
        const csv::Options options;

        Reader( QGLView& viewer, csv::Options& options, std::size_t size, coloured* c, unsigned int pointSize, const std::string& label, const QVector3D& offset = QVector3D( 0, 0, 0 ) );

        virtual ~Reader() {}

        virtual void start() = 0;
        virtual void update( const Eigen::Vector3d& offset ) = 0;
        virtual const Eigen::Vector3d& somePoint() const = 0;
        virtual bool readOnce() = 0;
        virtual void render( QGLPainter *painter ) = 0;
        virtual bool empty() const = 0;

        void show( bool s );
        bool show() const;
        bool isShutdown() const;
        void shutdown();
        void read();

    protected:
        void updatePoint( const Eigen::Vector3d& offset );
        void drawLabel( QGLPainter* painter, const QVector3D& position, const std::string& label );
        void drawLabel( QGLPainter* painter, const QVector3D& position );
        
        friend class Viewer;
        QGLView& m_viewer;
        boost::optional< comma::Extents< Eigen::Vector3f > > m_extents;
        boost::scoped_ptr< coloured > m_colored;
        bool m_shutdown;
        bool m_show;
        Io::IStream m_istream;
        boost::scoped_ptr< boost::thread > m_thread;
        mutable boost::mutex m_mutex;
        boost::optional< Eigen::Vector3d > m_point;
        boost::optional< Eigen::Vector3d > m_orientation;
        QColor4ub m_color;
        QVector3D m_translation;
        QQuaternion m_quaternion;
        std::string m_label;
        QVector3D m_offset;

    private:
        void drawText( QGLPainter *painter, const QString& string, const QColor4ub& color );
};
    
} } } // namespace snark { namespace graphics { namespace View {

#endif /*RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_READER_H_*/
