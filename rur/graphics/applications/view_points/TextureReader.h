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

#ifndef RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_TEXTURE_READER_H_
#define RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_TEXTURE_READER_H_


#include "./Reader.h"
#include <Qt3D/qglbuilder.h>

namespace rur { namespace graphics { namespace View {

/// display an image as a texture, set its position from an input csv stream   
class TextureReader : public Reader
{
    public:
        TextureReader( QGLView& viewer, Csv::Options& options, const std::string& file, double width, double height );

        void start();
        void update( const Eigen::Vector3d& offset );
        const Eigen::Vector3d& somePoint() const;
        bool readOnce();
        void render( QGLPainter *painter );
        bool empty() const;

    protected:
        boost::scoped_ptr< Csv::InputStream< PointWithId > > m_stream;
        const std::string m_file;
        QGeometryData m_geometry;
        QGLBuilder m_builder;
        QGLSceneNode* m_node;
        QGLTexture2D m_texture;
        QImage m_image;
        QGLMaterial m_material;
};

} } } // namespace rur { namespace graphics { namespace View {

#endif /*RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_TEXTURE_READER_H_*/
