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

#include "./Texture.h"

namespace rur { namespace graphics { namespace View {

Quad::Quad ( const QImage& image )
{
    QVector3D a( 0, 0, 0 );
    QVector3D b( image.width(), 0, 0 );
    QVector3D c( image.width(), image.height(), 0 );
    QVector3D d( 0, image.height(), 0 );
    QVector2D ta( 0, 0 );
    QVector2D tb( 1, 0 );
    QVector2D tc( 1, 1 );
    QVector2D td( 0, 1 );
    m_geometry.appendVertex( a, b, c, d );
    m_geometry.appendTexCoord(ta, tb, tc, td);
    m_builder.addQuads( m_geometry );
    m_sceneNode = m_builder.finalizedSceneNode();
    m_texture.setImage( image );
    m_material.setTexture( &m_texture );
    m_sceneNode->setMaterial( &m_material );
}


QGLSceneNode* Quad::node() const
{
    return m_sceneNode;
}


    
Texture::Texture ( const QString& string, const QColor4ub& color )
{
    QFont font( "helvetica", 64 );
    QFontMetrics metrics( font );
    QRect rect = metrics.boundingRect(string);

    m_image = QImage( rect.size(), QImage::Format_ARGB32 );
    m_image.fill( Qt::transparent );
    QPainter p2( &m_image );
    p2.setRenderHint(QPainter::Antialiasing);
    p2.setFont( font );
    p2.setPen( color.toColor() );
    p2.drawText( -rect.x(), -rect.y(), string );
    p2.end();
}

void Texture::draw ( QGLPainter* painter )
{
    Quad quad( m_image );
    painter->setStandardEffect( QGL::FlatReplaceTexture2D );
    glDepthMask(GL_FALSE);
//     glEnable(GL_BLEND);
    quad.node()->draw( painter );
//     glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}



} } } // namespace rur { namespace graphics

