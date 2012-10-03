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

#ifndef RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_VIEWER_H_
#define RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_VIEWER_H_

#ifdef WIN32
#include <winsock2.h>

//#include <windows.h>
#endif

#include <boost/optional.hpp>
#include <boost/thread.hpp>
#include <snark/graphics/Qt3D/View.h>
#include "./CameraReader.h"
#include "./Reader.h"

namespace snark { namespace graphics { namespace View {

class Viewer : public Qt3D::View
{
    Q_OBJECT
public:
    std::vector< boost::shared_ptr< Reader > > readers;
    Viewer( const QColor4ub& background_color, double fov, bool z_up, bool orthographic = false,
            boost::optional< comma::csv::Options > cameracsv = boost::optional< comma::csv::Options >(),
            boost::optional< Eigen::Vector3d > cameraposition = boost::optional< Eigen::Vector3d >(),
            boost::optional< Eigen::Vector3d > cameraorientation = boost::optional< Eigen::Vector3d >()
          );
    void shutdown();

private slots:
    void read();

private:
    
    void initializeGL( QGLPainter *painter );
    void paintGL( QGLPainter *painter );
    void setCameraposition( const Eigen::Vector3d& position, const Eigen::Vector3d& orientation );
    
    bool m_shutdown;
    bool m_lookAt;
    boost::scoped_ptr< CameraReader > m_cameraReader;
    boost::optional< Eigen::Vector3d > m_cameraposition;
    boost::optional< Eigen::Vector3d > m_cameraorientation;
    bool m_cameraFixed;
};

} } } // namespace snark { namespace graphics { namespace View {

#endif /*RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_VIEWER_H_*/
