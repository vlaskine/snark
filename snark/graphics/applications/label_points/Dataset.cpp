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

#ifdef WIN32
#define BOOST_FILESYSTEM_VERSION 3
#endif

#include <fstream>
#include <boost/filesystem/operations.hpp>
#include <comma/csv/stream.h>
#include "./Dataset.h"
#include "./Tools.h"
#include <snark/graphics/exception.h>

namespace snark { namespace graphics { namespace View {

BasicDataset::BasicDataset() : m_visible( true ) {}

BasicDataset::BasicDataset( const Eigen::Vector3d& offset ) : m_visible( true ), m_offset( offset ) {}

const BasicDataset::Points& BasicDataset::points() const { return m_points; }

const BasicDataset::Partitions& BasicDataset::partitions() const { return m_partitions; }

const Eigen::Vector3d& BasicDataset::offset() const { return *m_offset; }

void BasicDataset::visible( bool visible ) { m_visible = visible; }

bool BasicDataset::visible() const { return m_visible; }

const Extents< Eigen::Vector3d >& BasicDataset::extents() const { return m_extents; }

void BasicDataset::clear()
{
    m_points.clear();
    m_partitions.clear();
    m_vertices.reset();
    m_extents = Extents< Eigen::Vector3d >();
}

void BasicDataset::init()
{
    m_vertices.reset();
    if( m_points.empty() ) { return; }
    m_vertices.reset( new Qt3D::vertex_buffer( m_points.size() ) );
    for( Points::ConstEnumerator en = m_points.begin(); !en.end(); ++en )
    {
        Eigen::Vector3d pointXYZ =  en.key() - *m_offset;
        QVector3D point( pointXYZ.x(), pointXYZ.y(), pointXYZ.z() );
        QColor4ub color = Tools::colorFromId( en.value().id );
        m_vertices->addVertex( point, color );
    }
}

void BasicDataset::draw( QGLPainter* painter ) const
{
    if( m_visible && m_vertices )
    {
        painter->setStandardEffect(QGL::FlatPerVertexColor);
        painter->clearAttributes();
        painter->setVertexAttribute(QGL::position, m_vertices->points() );
        painter->setVertexAttribute(QGL::Color, m_vertices->color() );
        painter->draw( QGL::Points, m_vertices->size(), m_vertices->index() );
    }
}

void BasicDataset::insert( const Eigen::Vector3d& p, const BasicDataset::Data& data )
{
    m_points.insert( p, data );
    m_partitions[ data.id ].insert( p, data );
}

void BasicDataset::insert( const BasicDataset::Points& m ) // quick and dirty
{
    if( m.empty() ) { return; }
    m_points.insert( m );
    for( Points::ConstEnumerator en = m.begin(); !en.end(); ++en )
    {
        m_partitions[ en.value().id ].insert( en.key(), en.value() );
        m_extents.add( en.key() ); // quick and dirty: erase will screw it, but no other way...
    }
    init();
}

void BasicDataset::erase( const BasicDataset::Points& m ) // quick and dirty
{
    if( m.empty() ) { return; }
    m_points.erase( m );
    for( Points::ConstEnumerator en = m.begin(); !en.end(); ++en )
    {
        Partitions::iterator it = m_partitions.find( en.value().id );
        if( it == m_partitions.end() ) { continue; }
        it->second.erase( en.key() );
        if( it->second.empty() ) { m_partitions.erase( it ); }
    }
    init();
} 

Dataset::Dataset( const std::string& filename, const csv::Options& options, bool relabelDuplicated )
    : m_filename( filename )
    , m_options( options )
    , m_writable( true )
    , m_modified( false )
{
    backup();
    load();
    if( relabelDuplicated ) { labelDuplicated(); init(); }
}

Dataset::Dataset( const std::string& filename
                , const csv::Options& options
                , const Eigen::Vector3d& offset
                , bool relabelDuplicated )
    : m_filename( filename )
    , m_options( options )
    , m_writable( true )
    , m_modified( false )
{
    m_offset = offset;
    backup();
    load();
    if( relabelDuplicated ) { labelDuplicated(); init(); }
}

void Dataset::backup()
{
    static const std::string tilda = m_filename + "~";
    if( boost::filesystem::exists( tilda ) ) { boost::filesystem::remove( tilda ); }
    boost::filesystem::copy_file( m_filename, tilda );
}

void Dataset::save()
{
    if( !m_modified ) { std::cerr << "label-points: no changes since last save in " << m_filename << std::endl; return; }
    std::ofstream ofs( m_filename.c_str(), m_options.binary() ? std::ios::binary | std::ios::out : std::ios::out );
    if( !ofs.good() ) { std::cerr << "label-points: error: failed to open " << m_filename << std::endl; return; }
    std::vector< std::string > v = comma::split( m_options.fields, ',' ); // quick and dirty
    for( std::size_t i = 0; i < v.size(); ++i ) { if( v[i] != "id" ) { v[i] = ""; } }
    std::string fields = join( v, ',' );
    boost::scoped_ptr< comma::csv::ascii_output_stream< PointWithId > > ascii;
    boost::scoped_ptr< comma::csv::binary_output_stream< PointWithId > > binary;
    if( m_options.binary() ) { binary.reset( new comma::csv::binary_output_stream< PointWithId >( ofs, m_options.format().string(), fields, false ) ); }
    else { ascii.reset( new comma::csv::ascii_output_stream< PointWithId >( ofs, fields, m_options.delimiter, false ) ); }
    std::size_t count = 0;
    for( std::size_t i = 0; i < m_deque.size(); ++i )
    {
        if( m_options.binary() ) { binary->write( m_deque[i].first, m_deque[i].second.c_str() ); }
        else { ascii->write( m_deque[i].first, m_deque[i].second ); }
        if( ++count % 10000 == 0 ) { std::cerr << "\rlabel-points: saved " << count << " lines to " << m_filename << "             "; }
    }
    commit();
    std::cerr << "\rlabel-points: saved " << count << " lines to " << m_filename << "             " << std::endl;
}

void Dataset::saveAs( const std::string& f )
{
    std::cerr << "label-points: saving " << m_filename << " as " << f << "..." << std::endl;
    m_filename = f;
    m_modified = true;
    save();
}

bool Dataset::valid() const { return m_valid; }

void Dataset::load()
{
    m_deque.clear();
    m_selection.reset();
    this->BasicDataset::clear();
    std::ifstream ifs( m_filename.c_str(), m_options.binary() ? std::ios::binary | std::ios::in : std::ios::in );
    if( !ifs.good() ) { COMMA_THROW( graphics::exception, "failed to open \"" << m_filename << "\"" ); }
    boost::scoped_ptr< csv::AsciiInputStream< PointWithId > > ascii;
    boost::scoped_ptr< csv::BinaryInputStream< PointWithId > > binary;
    if( m_options.binary() ) { binary.reset( new csv::BinaryInputStream< PointWithId >( ifs, m_options.format().string(), m_options.fields, false ) ); }
    else { ascii.reset( new csv::AsciiInputStream< PointWithId >( ifs, m_options.fields, m_options.delimiter, false ) ); }
    std::size_t count = 0;
    try
    {
        bool first = true;
        while( true )
        {
            const PointWithId* p = m_options.binary() ? binary->read() : ascii->read();
            if( p == NULL ) { break; }
            if( first && !m_offset ) { m_offset = p->point.x() > 1000 || p->point.y() > 1000 || p->point.z() > 1000 ? p->point : Eigen::Vector3d(); first = false; }
            BasicDataset::insert( p->point, Data( p->id, m_deque.size() ) );
            if( m_options.binary() )
            {
                m_deque.push_back( std::make_pair( *p, std::string( binary->last(), m_options.format().size() ) ) );
            }
            else
            {
                m_deque.push_back( std::make_pair( *p, comma::join( ascii->last(), m_options.delimiter ) ) );
            }
            m_extents.add( p->point );
            if( ++count % 10000 == 0 ) { std::cerr << "\rlabel-points: loaded " << count << " lines from " << m_filename << "             "; }
        }
        m_selection.reset( new BasicDataset( *m_offset ) );
        commit();
        std::cerr << "\rlabel-points: loaded " << count << " lines from " << m_filename << "             " << std::endl;
        m_valid = true;
        return;
    }
    catch( std::exception& ex ) { std::cerr << "label-points: " << m_filename << ": " << ex.what() << std::endl; }
    catch( ... ) { std::cerr << "label-points: " << m_filename << ": unknown exception" << std::endl; }
    m_valid = false;
}

std::size_t Dataset::labelimpl( const Eigen::Vector3d& p, comma::uint32 id )
{
    if( !m_writable ) { return 0; }
    std::vector< Data* > d = m_points.find( p );
    std::size_t count = 0;
    for( std::size_t i = 0; i < d.size(); ++i )
    {
        if( d[i]->id == id ) { continue; }
        ++count;
        m_partitions[ d[i]->id ].erase( p );
        d[i]->id = id;
        m_partitions[ id ].insert( p, *d[i] );
        m_deque[ d[i]->index ].first.id = id;
        m_modified = true;
    }
    return count;
}

void Dataset::labelDuplicated() // quick and dirty
{
    if( !m_writable ) { std::cerr << "label-points: will not re-label duplicated points in read-only " << m_filename << "..." << std::endl; return; }
    std::cerr << "label-points: re-labelling duplicated points in " << m_filename << "..." << std::endl;
    std::size_t count = 0;
    for( Points::ConstEnumerator en = m_points.begin(); !en.end(); ++en )    { count += labelimpl( en.key(), en.value().id ); }
    //init();
    std::cerr << "label-points: re-labelled " << count << " duplicated point(s)" << std::endl;
}

void Dataset::label( const Eigen::Vector3d& p, comma::uint32 id )
{
    labelimpl( p, id );
    init(); // todo: hideous! remove, once the bug (something wrong with m_vertices->set) fixed
}

void Dataset::label( const BasicDataset::Points& m, comma::uint32 id )
{
    if( !m_writable ) { return; }
    for( Points::ConstEnumerator en = m.begin(); !en.end(); ++en ) { labelimpl( en.key(), id ); }
    init(); // todo: remove, once the bug (something wrong with m_vertices->set) fixed
}

BasicDataset& Dataset::selection() { return *m_selection; }

const BasicDataset& Dataset::selection() const { return *m_selection; }

const std::string& Dataset::filename() const { return m_filename; }

const csv::Options& Dataset::options() const { return m_options; }

void Dataset::writable( bool enabled ) { m_writable = enabled; }

bool Dataset::writable() const { return m_writable; }

bool Dataset::modified() const { return m_modified; }

void Dataset::commit() { m_modified = false; }

void Dataset::repair( const csv::Options& options ) // quick and dirty
{
    std::cerr << "label-points: repairing " << options.filename << "..." << std::endl;
    Dataset dataset( options.filename, options, false );
    dataset.labelDuplicated();
    dataset.save();
    std::cerr << "label-points: repaired " << options.filename << std::endl;
}

} } } // namespace snark { namespace graphics { namespace View {
