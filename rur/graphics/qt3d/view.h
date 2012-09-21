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

#ifndef RUR_GRAPHICS_GL_VIEW_H_
#define RUR_GRAPHICS_GL_VIEW_H_

#include <boost/optional.hpp>
#include <boost/thread.hpp>
#include <rur/graphics/exception.h>
#include <Eigen/Core>
#include <Qt3D/qglview.h>
#include <QMouseEvent>
#include "./coordinates.h"

namespace rur { namespace graphics { namespace Qt3D {

/// base class for 3d viewers with mouse navigation    
class view : public QGLView
{
    Q_OBJECT
public:
    view( const QColor4ub& background_color, double fov,bool z_up, bool orthographic = false );
    virtual ~view() {}

private slots:
    void hide_coordinates() { m_show_coordinates = false; update(); }

protected:
    void updateZFar();
    void updateView( const QVector3D& min, const QVector3D& max );
    void lookAtCenter();
    void draw_coordinates( QGLPainter* painter );
    void mousePressEvent( QMouseEvent *e );
    void mouseReleaseEvent( QMouseEvent *e );
    void mouseMoveEvent( QMouseEvent *e );
    void wheelEvent( QWheelEvent *e );
    QVector3D unproject( float x, float y, float depth );
    boost::optional< QVector3D > getPoint( const QPoint& point2d );
    void mouseDoubleClickEvent( QMouseEvent *e );
    
    const QColor4ub m_background_color;
    QVector3D m_sceneCenter;
    bool m_z_up;
    boost::optional< Eigen::Vector3d > m_offset;
    
private:
    boost::optional< QPoint > m_startPan;
    boost::optional< QPoint > m_startRotate;
    double m_sceneRadius;
    QVector3D m_revolve;
    boost::optional< coordinates > m_coordinates;
    bool m_show_coordinates;
};

} } } // namespace rur { namespace graphics { namespace GL {

#endif /*RUR_GRAPHICS_GL_VIEW_H_*/
