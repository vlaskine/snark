// This file is part of rur, a generic and flexible library
// for robotics research.
//
// Copyright (C) 2011 The University of Sydney
//
// rur is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// rur is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public
// License along with rur. If not, see <http://www.gnu.org/licenses/>.

#ifndef RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_VIEWER_H_
#define RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_VIEWER_H_

#ifdef WIN32
#include <WinSock2.h>
#define BOOST_FILESYSTEM_VERSION 3
#endif

#include <cmath>
#include <iostream>
#include <sstream>
#include <limits>
#include <math.h>
#include <boost/filesystem/operations.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/optional.hpp>
#include <comma/String/String.h>
#include <rur/graphics/Vector.h>
#include <comma/Math/Pi.h>

#include <rur/graphics/Qt3D/View.h>

#include "./Dataset.h"
#include "./PointWithId.h"
#include "./Tools.h"

namespace rur { namespace graphics { namespace View {

class Viewer : public Qt3D::View
{
    Q_OBJECT

    public:
        Tools::Navigate navigate;
        Tools::PickId pickId;
        Tools::SelectPartition selectPartition;
        Tools::SelectId selectId;
        Tools::SelectClip selectClip; 
        Tools::Fill fill;

        Viewer( const std::vector< Csv::Options >& options
              , bool labelDuplicated
              , const QColor4ub& background_color
              , bool orthographic = false, double fieldOfView = comma::Math::pi / 4 );

        void show( std::size_t i, bool visible ); // quick and dirty
        void setWritable( std::size_t i, bool writable ); // quick and dirty
        void save();
        void reload();
        const std::vector< boost::shared_ptr< Dataset > >& datasets() const;
        Dataset& dataset( std::size_t index ); // quick and dirty
        const Dataset& dataset( std::size_t index ) const; // quick and dirty

    public slots:
        void handleId( comma::UInt32 id );

    signals:
        void initialized();

    protected:
        void saveStateToFile();
        void initializeGL( QGLPainter *painter );
        void paintGL( QGLPainter *painter );
        void mousePressEvent( QMouseEvent* e );
        void mouseReleaseEvent( QMouseEvent* e );
        void mouseMoveEvent( QMouseEvent* e );
        boost::optional< std::pair< Eigen::Vector3d, comma::UInt32 > > pointSelection( const QPoint& point, bool writableOnly = false );

    private:
        friend class Tools::Tool; // quick and dirty
        friend class Tools::PickId; // quick and dirty
        friend struct Tools::SelectPartition; // quick and dirty
        friend struct Tools::SelectId; // quick and dirty
        friend class Tools::SelectClip; // quick and dirty
        friend struct Tools::Fill; // quick and dirty
        void setCamera();
                
        Tools::Tool* m_currentTool;
        std::vector< boost::shared_ptr< Dataset > > m_datasets;
        boost::optional< comma::UInt32 > m_id;
        const QColor4ub m_background_color;
        std::vector< Csv::Options > m_options;
        bool m_labelDuplicated;
        bool m_orthographic;
        double m_fieldOfView;
        boost::optional< QPoint > m_startPan;
        boost::optional< QPoint > m_startRotate;
        double m_sceneRadius;
};

} } } // namespace rur { namespace graphics { namespace View {

#endif // RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_VIEWER_H_
