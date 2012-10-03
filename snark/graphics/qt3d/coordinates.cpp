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

#include "./coordinates.h"

namespace snark { namespace graphics { namespace Qt3D {

coordinates::coordinates( float size, float thickness ):
    m_node( new QGLSceneNode() )
{
    QGLBuilder builder;
    builder << QGLCylinder( thickness, thickness, size, 12 );
    QGLSceneNode* cylinder = builder.finalizedSceneNode();
    cylinder->setPosition( QVector3D( 0, 0, 0.5 * size ) );
    cylinder->setEffect( QGL::LitMaterial );

    QGLBuilder arrowBuilder;
    arrowBuilder << QGLCylinder( 0.01, 2 * thickness, size / 3, 12 );
    QGLSceneNode* arrow = arrowBuilder.finalizedSceneNode();
    arrow->setPosition( QVector3D( 0, 0, size ) );
    arrow->setEffect( QGL::LitMaterial );
    
    
    QGLSceneNode* x = new QGLSceneNode( m_node );
    x->addNode(cylinder);
    QMatrix4x4 matrix;
    QQuaternion q = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 90.0f);
    matrix.rotate( q );
    x->setLocalTransform( matrix );
    QGLSceneNode* xArrow = new QGLSceneNode( m_node );
    xArrow->addNode( arrow );
    xArrow->setLocalTransform( matrix );
    QGLMaterial* xMaterial = new QGLMaterial;
    xMaterial->setDiffuseColor( QColor( 255, 0, 0, 128 ) );
    x->setMaterial( xMaterial );
    xArrow->setMaterial( xMaterial );
    
    QGLSceneNode* y = new QGLSceneNode( m_node );
    y->addNode(cylinder);
    q = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, -90.0f);
    matrix.rotate( q );
    y->setLocalTransform( matrix );
    QGLSceneNode* yArrow = new QGLSceneNode( m_node );
    yArrow->addNode( arrow );
    yArrow->setLocalTransform( matrix );
    QGLMaterial* yMaterial = new QGLMaterial;
    yMaterial->setDiffuseColor(Qt::green);
    y->setMaterial( yMaterial );
    QGLMaterial* yArrowMaterial = new QGLMaterial;
    yArrowMaterial->setDiffuseColor( QColor( 0, 255, 0, 128 ) );
    yArrow->setMaterial( yArrowMaterial );
    
    QGLSceneNode* z = new QGLSceneNode( m_node );
    z->addNode(cylinder);
    QGLSceneNode* zArrow = new QGLSceneNode( m_node );
    zArrow->addNode( arrow );
    QGLMaterial* zMaterial = new QGLMaterial;
    zMaterial->setDiffuseColor(Qt::blue);
    z->setMaterial( zMaterial );
    QGLMaterial* zArrowMaterial = new QGLMaterial;
    zArrowMaterial->setDiffuseColor( QColor( 0, 0, 255, 128 ) );
    zArrow->setMaterial( zArrowMaterial );
}

QGLSceneNode* coordinates::node() const
{
    return m_node;
}


} } } // namespace snark { namespace graphics

