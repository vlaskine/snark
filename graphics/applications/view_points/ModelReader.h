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

#ifndef SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_MODEL_READER_H_
#define SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_MODEL_READER_H_


#include "./Reader.h"
#include "./PlyLoader.h"

class QGLAbstractScene;

namespace snark { namespace graphics { namespace View {

/// display 3d models ( obj or 3ds ), set its position from an input csv stream       
class ModelReader : public Reader
{
    public:
        ModelReader( QGLView& viewer, comma::csv::options& options, const std::string& file, bool z_up, coloured* c, const std::string& label );

        void start();
        void update( const Eigen::Vector3d& offset );
        const Eigen::Vector3d& somePoint() const;
        bool readOnce();
        void render( QGLPainter *painter );
        bool empty() const;

    protected:
        boost::scoped_ptr< comma::csv::input_stream< PointWithId > > m_stream;
        const std::string m_file;
        QGLAbstractScene* m_scene;
        bool m_z_up; // z-axis points up
        boost::optional< PlyLoader > m_plyLoader;
};

} } } // namespace snark { namespace graphics { namespace View {

#endif /*SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_MODEL_READER_H_*/
