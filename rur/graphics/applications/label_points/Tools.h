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

#ifndef RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_TOOLS_H_
#define RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_TOOLS_H_

#include <boost/optional.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <qcursor.h>
#include <qevent.h>
#include <qobject.h>
#include <comma/Base/Types.h>
#include <comma/Math/Extents.h>
#include <Qt3D/qcolor4ub.h>
#include <Qt3D/qglpainter.h>
#include "./Icons.h"

namespace snark { namespace graphics { namespace View { class Viewer; } } }

namespace snark { namespace graphics { namespace View { namespace Tools {

QColor4ub colorFromId( comma::UInt32 id );

class Tool : public QObject
{   
    public:
        Tool( Viewer& viewer, QCursor* cursor );
        virtual ~Tool() {}
        void toggle( bool checked );
        virtual void onMousePress( QMouseEvent* e );
        virtual void onMouseRelease( QMouseEvent* e );
        virtual void onMouseMove( QMouseEvent* e );
        virtual void draw( QGLPainter* painter );
    
    protected:
        Viewer& m_viewer;
        boost::scoped_ptr< QCursor > m_cursor;
        virtual void init() {}
        virtual void reset() {}
};

struct Navigate : public Tool
{
    Navigate( Viewer& viewer );
};

class PickId : public Tool
{
    Q_OBJECT
    
    public:
        PickId( Viewer& viewer );
        void onMousePress( QMouseEvent* e );
        void shakeColors();
    
    signals:
        void valueChanged( comma::UInt32 id );
};

struct Fill : public Tool
{
    Fill( Viewer& viewer );
    void onMousePress( QMouseEvent* e );
};

struct SelectPartition : public Tool
{
    SelectPartition( Viewer& viewer );
    void onMousePress( QMouseEvent* e );
    void onMouseRelease( QMouseEvent* e );
    void onMouseMove( QMouseEvent* e );
};

struct SelectId : public Tool
{
    SelectId( Viewer& viewer );
    void onMousePress( QMouseEvent* e );
    void onMouseRelease( QMouseEvent* e );
    void onMouseMove( QMouseEvent* e );
};

class SelectClip : public Tool
{
    public:
        SelectClip( Viewer& viewer );
        void onMousePress( QMouseEvent* e );
        void onMouseRelease( QMouseEvent* e );
        void onMouseMove( QMouseEvent* e );
        void draw( QGLPainter* painter );
        
    private:
        boost::optional< QRect > m_rectangle;
        Eigen::Vector3d m_centre;
        Eigen::Vector3d m_radius;
};

} } } } // namespace snark { namespace graphics { namespace View { namespace Tools {

#endif // RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_TOOLS_H_
