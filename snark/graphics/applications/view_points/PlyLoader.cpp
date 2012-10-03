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

#include "./PlyLoader.h"
#include <fstream>
#include <cstring>
#include <iostream>
#include <snark/graphics/exception.h>
#include <boost/lexical_cast.hpp>

namespace snark { namespace graphics { namespace View {

PlyLoader::PlyLoader( const std::string& file )
{
    std::ifstream stream( file.c_str() );
    char line[255];
    stream.getline( line, 255 );
    if( std::strcmp(line, "ply" ) != 0 )
    {
        COMMA_THROW( exception, "expected ply file: " << file );
    }
    std::string vertex = "element vertex";
    std::string face = "element face";
    unsigned int numVertex = 0;
    unsigned int numFace = 0;
    bool hasAlpha = false;
    bool hasRGB = false;
    while( std::strcmp(line, "end_header" ) != 0 )
    {
        stream.getline( line, 255 );
        if( std::memcmp( line, &vertex[0], vertex.size() ) == 0 )
        {
            std::string num( line + vertex.size() + 1 );
            numVertex = boost::lexical_cast< unsigned int >( num );
        }
        else if( std::memcmp( line, &face[0], face.size() ) == 0 )
        {
            std::string num( line + face.size() + 1 );
            numFace = boost::lexical_cast< unsigned int >( num );
        }
        else if( std::strcmp(line, "property uchar red" ) == 0 )
        {
            hasRGB = true;
        }
        else if( std::strcmp(line, "property uchar alpha" ) == 0 )
        {
            hasAlpha = true;
        }
    }
    double x, y, z;
    unsigned int r, g, b;
    unsigned int a = 255;
    QGeometryData geometry;
    QArray< QVector3D > vertices;
    QArray< QColor4ub > colors;
    for( unsigned int i = 0; i < numVertex; i++ )
    {
        stream >> x >> y >> z;
        if( hasRGB )
        {
            stream >> r >> g >> b;
        }
        if( hasAlpha )
        {
            stream >> a;
        }
        if( numFace > 0 )
        {
            geometry.appendVertex( QVector3D( x, y, z ) );
            geometry.appendColor( QColor4ub( r, g, b, a ) );
        }
        else
        {
            vertices.append( QVector3D( x, y, z ) );
            colors.append( QColor4ub( r, g, b, a ) );
        }
    }
    if( numFace > 0 )
    {
        int vertexPerFace, v1, v2, v3;
        for( unsigned int i = 0; i < numFace; i++ )
        {
            stream >> vertexPerFace >> v1 >> v2 >> v3;
            if( vertexPerFace != 3 )
            {
                COMMA_THROW( exception, "only triangles supported" );
            }
            geometry.appendIndices( v1, v2, v3 );
        }
        QGLBuilder builder;
        builder.addTriangles( geometry );
        m_sceneNode = builder.finalizedSceneNode();
    }
    else
    {
        m_vertices.addAttribute( QGL::position, vertices );
        m_vertices.addAttribute( QGL::Color, colors );
        m_vertices.upload();
        m_sceneNode = NULL;
    }
    stream.close();
}

void PlyLoader::draw ( QGLPainter* painter )
{
    painter->setStandardEffect(QGL::FlatPerVertexColor);
    if( m_sceneNode != NULL )
    {
        m_sceneNode->draw( painter );
    }
    else
    {
        painter->clearAttributes();
        // use vbo buffers, will get uploaded to the GL server only once
        painter->setVertexBundle( m_vertices ); 
        painter->draw( QGL::Points, m_vertices.vertexCount() );
    }
}


} } }
