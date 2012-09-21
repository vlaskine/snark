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

#ifndef RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_DATASET_H_
#define RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_DATASET_H_

#include <deque>
#include <comma/Base/Types.h>
#include <comma/Csv/Options.h>
#include <comma/Math/Extents.h>
#include <rur/graphics/Vector.h>
#include <rur/graphics/Qt3D/VertexBuffer.h>
#include "./PointMap.h"
#include "./PointWithId.h"
#include <Qt3D/qglpainter.h>

namespace rur { namespace graphics { namespace View {

class BasicDataset
{
    public:
        struct Data
        {
            comma::UInt32 id;
            std::size_t index;
            Data() {}
            Data( const Data& rhs ) { operator=( rhs ); }
            Data( comma::UInt32 id, std::size_t index ) : id( id ), index( index ) {}
        };
        typedef PointMap< Eigen::Vector3d, Data > Points;
        typedef std::map< comma::UInt32, Points > Partitions;    
        BasicDataset();
        BasicDataset( const Eigen::Vector3d& offset );
        const Points& points() const;
        const Partitions& partitions() const;
        const Eigen::Vector3d& offset() const;
        const Extents< Eigen::Vector3d >& extents() const;
        void init();
        void draw( QGLPainter* painter ) const;
        void visible( bool visible );
        bool visible() const;
        void clear();
        void insert( const Points& m );
        void erase( const Points& m );
    
    protected:
        bool m_visible;
        Points m_points;
        Partitions m_partitions;
        boost::scoped_ptr< Qt3D::VertexBuffer > m_vertices;
        boost::optional< Eigen::Vector3d > m_offset;
        Extents< Eigen::Vector3d > m_extents;
        void insert( const Eigen::Vector3d& p, const Data& data );
};

class Dataset : public BasicDataset
{
    public:
        Dataset( const std::string& filename, const Csv::Options& options, bool relabelDuplicated );
        Dataset( const std::string& filename, const Csv::Options& options, const Eigen::Vector3d& offset, bool relabelDuplicated );
        void save();
        void saveAs( const std::string& f );
        void load();
        void backup();
        void label( const Eigen::Vector3d& p, comma::UInt32 id );
        void label( const Points& p, comma::UInt32 id );
        void writable( bool enabled );
        bool writable() const;
        bool modified() const;
        void commit();
        BasicDataset& selection();
        const BasicDataset& selection() const;
        const std::string& filename() const;
        const Csv::Options& options() const;
        bool valid() const;
        static void repair( const Csv::Options& options );
    
    private:
        void insert( const Points& m );
        void erase( const Points& m );
        void clear();
        std::size_t labelimpl( const Eigen::Vector3d& p, comma::UInt32 id );
        void labelDuplicated();
        //typedef std::deque< std::pair< PointWithId, std::vector< std::string > > > Deque;
        typedef std::deque< std::pair< PointWithId, std::string > > Deque;
        Deque m_deque;
        std::string m_filename;
        const Csv::Options m_options;
        boost::scoped_ptr< BasicDataset > m_selection;
        bool m_writable;
        bool m_modified;
        bool m_valid;
};

} } } // namespace rur { namespace graphics { namespace View {

#endif // RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_DATASET_H_
