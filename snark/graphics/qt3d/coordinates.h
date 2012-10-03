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

#ifndef RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_COORDINATES_H_
#define RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_COORDINATES_H_

#include <Qt3D/qgltexture2d.h>
#include <Qt3D/qglscenenode.h>
#include <Qt3D/qglbuilder.h>
#include <Qt3D/qglpainter.h>
#include <Qt3D/qglcylinder.h>

namespace snark { namespace graphics { namespace Qt3D {

/// display a coordinate system    
class coordinates
{
public:
    coordinates( float size, float thickness );
    QGLSceneNode* node() const;

private:
//     QGLBuilder m_builder;
    QGLSceneNode* m_sceneNode;
    QGLSceneNode* m_node;
};



} } } // namespace snark { namespace graphics

#endif /*RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_COORDINATES_H_*/
