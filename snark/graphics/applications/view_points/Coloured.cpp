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

#include <algorithm>
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>
#include <qnamespace.h>
#include <comma/String/String.h>
#include "./coloured.h"

namespace snark { namespace graphics { namespace View {

static QColor4ub color_from_name( const std::string& name )
{
         if( name == "black" ) { return QColor4ub( Qt::black ); }
    else if( name == "white" ) { return QColor4ub( Qt::white ); }
    else if( name == "red" ) { return QColor4ub( Qt::red ); }
    else if( name == "green" ) { return QColor4ub( Qt::green ); }
    else if( name == "blue" ) { return QColor4ub( Qt::blue ); }
    else if( name == "yellow" ) { return QColor4ub( Qt::yellow ); }
    else if( name == "magenta" ) { return QColor4ub( Qt::magenta ); }
    else if( name == "cyan" ) { return QColor4ub( Qt::cyan ); }
    else if( name == "grey" ) { return QColor4ub( Qt::gray ); }
    else if( name == "pink" ) { return QColor4ub( 255, 128, 128 ); }
    else if( name == "orange" ) { return QColor4ub( 255, 128, 0 ); }
    else if( name == "salad" ) { return QColor4ub( 128, 255, 128 ); }
    else if( name == "sky" ) { return QColor4ub( 128, 128, 255 ); }
    else { COMMA_THROW( comma::exception, "expected color, got " << name ); } // todo: add arbitrary color
}
    
static QColor4ub multiply( const QColor4ub& color, double scalar )
{
    double red = scalar * color.redF();
    double green = scalar * color.greenF();
    double blue = scalar * color.blueF();
    double alpha = scalar * color.alphaF();
    return QColor4ub::fromRgbF( red, green, blue, alpha );
}

static unsigned int threshold( unsigned int i, unsigned int threshold )
{
    if( i > threshold )
    {
        return threshold;
    }
    else
    {
        return i;
    }
}

static QColor4ub add( const QColor4ub& a, const QColor4ub& b )
{
    unsigned int red = threshold( a.red() + b.red(), 255 );
    unsigned int green = threshold( a.green() + b.green(), 255 );
    unsigned int blue = threshold( a.blue() + b.blue(), 255 );
    unsigned int alpha = threshold( a.alpha() + b.alpha(), 255 );
    return QColor4ub( red, green, blue, alpha );
}


Fixed::Fixed( const std::string& name ) : m_color( color_from_name( name ) ) {}

QColor4ub Fixed::color( const Eigen::Vector3d&, comma::uint32, double, const QColor4ub& ) const { return m_color; }

ByHeight::ByHeight( double from
                  , double to
                  , const QColor4ub& from_color
                  , const QColor4ub& to_color
                  , bool cyclic
                  , bool linear
                  , bool sharp )
    : from( from )
    , to( to )
    , sum( to + from )
    , diff( to - from )
    , middle( from / sum )
    , from_color( from_color )
    , to_color( to_color )
    , cyclic( cyclic )
    , linear( linear )
    , sharp( sharp )
{
    average_color.setRed( ( from_color.red() / 2 ) + ( to_color.red() / 2 ) );
    average_color.setGreen( ( from_color.green() / 2 ) + ( to_color.green() / 2 ) );
    average_color.setBlue( ( from_color.blue() / 2 ) + ( to_color.blue() / 2 ) );
    average_color.setAlpha( ( from_color.alpha() / 2 ) + ( to_color.alpha() / 2 ) );
}

QColor4ub ByHeight::color( const Eigen::Vector3d& point, comma::uint32, double, const QColor4ub& ) const
{
    if( cyclic )
    {
        double v = std::fmod( point.z() - std::floor( point.z() / sum ) * sum, sum ) / sum;
        if( sharp ) { return v < middle ? from_color : to_color; }
        if( linear )
        {
            if( v < middle * 0.5 )
            {
                return add( multiply( average_color, 1 - v ), multiply( from_color, v ) );
            }
            else if( v < ( 1 + middle ) * 0.5 )
            {
                v = v - middle * 0.5;
                return add( multiply( from_color, ( 1 - v ) ), multiply( to_color, v ) );
            }
            else
            {
                v = v - ( 1 + middle ) * 0.5;
                return add( multiply( to_color, 1 - v ), multiply( average_color, v ) );
            }
        }
        else
        {
            if( v < middle )
            {
                v = v / middle - 0.5;
                v = v * v * 4;
                return add( multiply( from_color, 1 - v ), multiply( average_color, v ) );
            }
            else
            {
                v = ( v - middle ) / ( 1 - middle ) - 0.5;
                v = v * v * 4;
                return add( multiply( average_color, v ), multiply( to_color, 1 - v ) );
            }
        }
    }
    else
    {
        double v = ( point.z() - from ) / diff;
        v = ( v < 0 ? 0 : v > 1 ? 1 : v );
        return add( multiply( from_color, v ), multiply( to_color, 1 - v ) );
    }
}

ByScalar::ByScalar( double from, double to, const QColor4ub& from_color, const QColor4ub& to_color )
    : from( from ), to( to ), diff( to - from ), from_color( from_color ), to_color( to_color ) {}
QColor4ub ByScalar::color( const Eigen::Vector3d& point, comma::uint32, double scalar, const QColor4ub& ) const
{
    (void)point;
    double v = ( scalar - from ) / diff;
    v = ( v < 0 ? 0 : v > 1 ? 1 : v );
    return add( multiply( from_color, 1 - v ), multiply( to_color, v ) );
}

namespace impl {

static boost::array< unsigned int, 256 > colorInit()
{
    boost::array< unsigned int, 256 > a;
    for( std::size_t i = 0; i < 256; ++i ) { a[i] = i; }
    std::random_shuffle( a.begin(), a.end() );
    return a;
}
static unsigned char colorIndex = 23; // whatever
static boost::array< unsigned int, 256 > colorIndices = colorInit();

} // namespace impl {

ById::ById( const QColor4ub& backgroundcolour )
    : m_background( backgroundcolour )
    , m_hasScalar( false )
{
}
    
ById::ById( const QColor4ub& backgroundcolour
          , double from
          , double to )
    : m_background( backgroundcolour )
    , m_hasScalar( true )
    , m_from( from )
    , m_diff( to - from )
{
}

QColor4ub ById::color( const Eigen::Vector3d&, comma::uint32 id, double scalar, const QColor4ub& ) const // quick and dirty
{
    static const float b = 0.95f;
    unsigned char i = ( id & 0xff ) + ( ( id & 0xff00 ) >> 16 );
    const float h = b * float( impl::colorIndices[ i ] ) / 255;
    const float a = 0.3f * float( 255 - impl::colorIndices[ i ] ) / 255;
    QColor4ub color;
    switch( ( i * 13 + impl::colorIndex ) % 6 )
    {
        case 0 : color = QColor4ub::fromRgbF( b, h, a ); break;
        case 1 : color = QColor4ub::fromRgbF( a, b, h ); break;
        case 2 : color = QColor4ub::fromRgbF( h, a, b ); break;
        case 3 : color = QColor4ub::fromRgbF( b, a, h ); break;
        case 4 : color = QColor4ub::fromRgbF( h, b, a ); break;
        default: color = QColor4ub::fromRgbF( a, h, b ); break;
    }
    if( !m_hasScalar ) { return color; }
    double v = ( scalar - m_from ) / m_diff;
    v = ( v < 0 ? 0 : v > 1 ? 1 : v );
    return add( multiply( color, v ), multiply( m_background, 1 - v ) );
}

QColor4ub ByRGB::color( const Eigen::Vector3d& , comma::uint32, double, const QColor4ub& c ) const
{
    return c;
}

coloured* colourFromString( const std::string& s, const std::string& fields, const QColor4ub& backgroundcolour )
{
    std::vector< std::string > f = comma::split( fields, ',' );
    bool hasId = false;
    for( unsigned int i = 0; !hasId && i < f.size(); ++i ) { hasId = f[i] == "id"; } // quick and dirty
    bool hasScalar = false;
    for( unsigned int i = 0; !hasScalar && i < f.size(); ++i ) { hasScalar = f[i] == "scalar"; } // quick and dirty
    bool r = false;
    bool g = false;
    bool b = false;
    bool a = false;
    for( unsigned int i = 0; !( r && g && b ) && !a && i < f.size(); ++i ) // quick and dirty
    {
        if( f[i] == "r" ) { r = true; }
        if( f[i] == "g" ) { g = true; }
        if( f[i] == "b" ) { b = true; }
        if( f[i] == "a" ) { a = true; }
    }
    snark::graphics::View::coloured* c;
    try
    {
        if( hasId )
        {
            if( hasScalar )
            {
                double from = 0;
                double to = 1;
                if( s != "" )
                {
                    std::vector< std::string > v = comma::split( s, ':' );
                    if( v.size() != 2 ) { COMMA_THROW( comma::exception, "expected range (-5:20), got " << s ); }
                    from = boost::lexical_cast< double >( v[0] );
                    to = boost::lexical_cast< double >( v[1] );
                }
                c = new snark::graphics::View::ById( backgroundcolour, from, to );
            }
            else
            {
                c = new snark::graphics::View::ById( backgroundcolour );
            }
        }
        else if( ( r && g && b ) || a )
        {
            c = new snark::graphics::View::ByRGB;
        }
        else if( hasScalar )
        {
            QColor4ub from_color = color_from_name( "magenta" );
            QColor4ub to_color = color_from_name( "cyan" );
            double from = 0;
            double to = 1;
            if( s != "" )
            {
                std::vector< std::string > v = comma::split( s, ',' );
                switch( v.size() )
                {
                    case 1:
                    {
                        std::vector< std::string > w = comma::split( v[0], ':' );
                        if( w[0][0] >= 'a' && w[0][0] <= 'z' )
                        {
                            from_color = color_from_name( w[0] );
                            to_color = color_from_name( w[1] );
                        }
                        else
                        {
                            from = boost::lexical_cast< double >( w[0] );
                            to = boost::lexical_cast< double >( w[1] );
                        }
                        break;
                    }
                    case 2:
                    {
                        std::vector< std::string > w = comma::split( v[0], ':' );
                        from = boost::lexical_cast< double >( w[0] );
                        to = boost::lexical_cast< double >( w[1] );
                        w = comma::split( v[1], ':' );
                        from_color = color_from_name( w[0] );
                        to_color = color_from_name( w[1] );
                        break;
                    }
                    default:
                        COMMA_THROW( comma::exception, "expected range (e.g. -5:20,red:blue or 3:10), got " << s );
                }
            }
            c = new snark::graphics::View::ByScalar( from, to, from_color, to_color );
        }
        else
        {
            c = new snark::graphics::View::Fixed( s );
        }
    }
    catch( ... )
    {
        std::vector< std::string > v = comma::split( s, ',' );
        boost::optional< double > from;
        double to = 1;
        QColor4ub from_color = color_from_name( "red" );
        QColor4ub to_color = color_from_name( "yellow" );
        bool sharp = false;
        bool cyclic = false;
        bool linear = true;
        for( unsigned int i = 0; i < v.size(); ++i )
        {
            if( v[i].empty() ) { continue; }
            else if( v[i] == "sharp" ) { sharp = true; }
            else if( v[i] == "smooth" ) { sharp = false; }
            else if( v[i] == "cyclic" ) { cyclic = true; }
            else if( v[i] == "linear" ) { linear = true; }
            else if( v[i] == "quadratic" ) { linear = false; }
            else
            {
                std::vector< std::string > w = split( v[i], ':' );
                if( w[0][0] >= 'a' && w[0][0] <= 'z' )
                {
                    from_color = color_from_name( w[0] );
                    to_color = color_from_name( w[1] );
                }
                else
                {
                    from = boost::lexical_cast< double >( w[0] );
                    to = boost::lexical_cast< double >( w[ w.size() == 1 ? 0 : 1 ] );
                }
            }
        }
        if( !from ) { from = cyclic ? 1 : 0; }
        c = new snark::graphics::View::ByHeight( *from, to, from_color, to_color, cyclic, linear,sharp );
    }
    return c;
}

} } } // namespace snark { namespace graphics { namespace View {
