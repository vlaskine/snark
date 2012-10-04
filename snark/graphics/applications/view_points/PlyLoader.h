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

#ifndef SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_PLY_LOADER_H_
#define SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_PLY_LOADER_H_

#include <Qt3D/qgeometrydata.h>
#include <Qt3D/qglbuilder.h>
#include <Qt3D/qglpainter.h>

namespace snark { namespace graphics { namespace View {

/// loader for simple ply models
class PlyLoader
{
public:
    PlyLoader( const std::string& file );
    void draw( QGLPainter* painter );
private:
    QGLSceneNode* m_sceneNode;
    QGLVertexBundle m_vertices;
};

} } } // namespace snark { namespace graphics { namespace View {

#endif /*SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_PLY_LOADER_H_*/
