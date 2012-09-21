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

#ifndef RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_TEXTURE_H_
#define RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_TEXTURE_H_

#include <Qt3D/qgltexture2d.h>
#include <Qt3D/qglscenenode.h>
#include <Qt3D/qglbuilder.h>
#include <Qt3D/qglabstractscene.h>
#include <Qt3D/qglpainter.h>

namespace rur { namespace graphics { namespace View {

class Quad
{
public:
    Quad( const QImage& image );
    QGLSceneNode* node() const;

private:
    QGLBuilder m_builder;
    QGeometryData m_geometry;
    QGLTexture2D m_texture;
    QGLMaterial m_material;
    QGLSceneNode* m_sceneNode;
};

class Texture
{
public:
    Texture( const QString& string, const QColor4ub& color );

    void draw( QGLPainter* painter );
    
private:    
    QImage m_image;
    
};


} } } // namespace rur { namespace graphics

#endif /*RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_TEXTURE_H_*/
