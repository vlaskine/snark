// This file is part of snark, a generic and flexible library 
// for robotics research.
//
// Copyright (C) 2011 The University of Sydney
//
// snark is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// snark is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
// for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with snark. If not, see <http://www.gnu.org/licenses/>.

#ifndef RUR_GRAPHICS_POINT_POINT_H_
#define RUR_GRAPHICS_POINT_POINT_H_

#include <string.h>
#include <algorithm>
#include <boost/array.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/static_assert.hpp>
#include <comma/math/compare.h>

namespace snark { namespace graphics {

/// @todo tear down this file; use Eigen instead...
    
/// multidimensional point allocated contiguosly in static memory 
/// @deprecated use Eigen instead    
template < typename T, unsigned int D > class point;

/// cartesian point
/// @deprecated use Eigen instead
template < typename T > struct point_xyz;
    
/// orientation as roll,pitch,yaw
/// @deprecated use Eigen instead
template < typename T > struct orientation;

/// coordinates + roll,pitch,yaw
/// @deprecated use Eigen instead
template < typename T, template < class > class P > struct position;


template < typename T, unsigned int D >
class point : public boost::array< T, D >
{
    public:
        BOOST_STATIC_ASSERT( D > 0 );
    
        /// type
        typedef T type;
    
        /// dimension
        enum { dimension = D };
        
        /// underlying vector type
        typedef boost::array< T, D > vector;
        
        /// operators
        const point& operator-() const
        {
            point p( *this );
            std::for_each( p.begin(), p.end(), boost::lambda::_1 = -boost::lambda::_1 );
            return p;
        }
        
        const point& operator+=( const point& rhs )
        {
            for( unsigned int i = 0; i < D; ++i ) { this->operator[](i) += rhs[i]; }
            return *this;
        }
        
        const point& operator-=( const point& rhs )
        {
            for( unsigned int i = 0; i < D; ++i ) { this->operator[](i) -= rhs[i]; }
            return *this;
        }
        
        template < typename S >
        const point& operator*=( S rhs ) { std::for_each( this->begin(), this->end(), boost::lambda::_1 *= rhs ); return *this; }
        
        template < typename S >
        const point& operator/=( S rhs ) { std::for_each( this->begin(), this->end(), boost::lambda::_1 /= rhs ); return *this; }
        
        /// comparison operators
        bool operator==( const point& rhs ) const
        { 
            for( unsigned int i = 0; i < D; ++i )
            {
                if( !comma::math::equal( this->operator[](i), rhs[i] ) ) { return false; }
            }
            return true;
        }
        bool operator!=( const point& rhs ) const { return !operator==( rhs ); }
        
        /// convert
        /// @todo a bit too generic, perhaps?
        template < typename P >
        P as() const
        {
            P r;
            unsigned int d = static_cast< unsigned int >( dimension ) < static_cast< unsigned int >( P::dimension )
                           ? static_cast< unsigned int >( dimension )
                           : static_cast< unsigned int >( P::dimension );
            for( unsigned i = 0; i < d; ++i ) { r[i] = static_cast< typename P::type >( this->operator[](i) ); }
            return r;
        }
        
        /// convert, lower case alias; rename everywhere, when we have time
        template < typename P >
        P as() const { return As< P >(); }
            
        /// constructors
        point() { std::for_each( this->begin(), this->end(), boost::lambda::_1 = 0 ); }
        point( const point& rhs ) : boost::array< T, D >( static_cast< const boost::array< T, D >& >( rhs ) ) {}
        //point( const point& rhs ) : boost::array< T, D >( reinterpret_cast< const boost::array< T, D >& >( rhs ) ) {}
        //point( const boost::array< T, D >& rhs ) : boost::array< T, D >( rhs ) {}
        point( T t0 ) { BOOST_STATIC_ASSERT( D == 1 ); this->operator[](0) = t0; }
        point( T t0, T t1 ) { BOOST_STATIC_ASSERT( D == 2 ); this->operator[](0) = t0; this->operator[](1) = t1; }
        point( T t0, T t1, T t2 ) { BOOST_STATIC_ASSERT( D == 3 ); this->operator[](0) = t0; this->operator[](1) = t1; this->operator[](2) = t2; }
        point( T t0, T t1, T t2, T t3 ) { BOOST_STATIC_ASSERT( D == 4 ); this->operator[](0) = t0; this->operator[](1) = t1; this->operator[](2) = t2; this->operator[](3) = t3; }
};

template < typename T, unsigned int D >
inline point< T, D > operator+( const point< T, D >& lhs, const point< T, D >& rhs ) { point< T, D > p( lhs ); p += rhs; return p; }
template < typename T, unsigned int D >
inline point< T, D > operator-( const point< T, D >& lhs, const point< T, D >& rhs ) { point< T, D > p( lhs ); p -= rhs; return p; }
template < typename T, unsigned int D, typename S >
inline point< T, D > operator*( const point< T, D >& lhs, S rhs ) { point< T, D > p( lhs ); p *= rhs; return p; }
template < typename T, unsigned int D, typename S >
inline point< T, D > operator/( const point< T, D >& lhs, S rhs ) { point< T, D > p( lhs ); p /= rhs; return p; }

template < typename T >
struct point_xyz : public point< T, 3 >
{
    /// constructors
    point_xyz() : point< T, 3 >() {}
    point_xyz( const point_xyz& rhs ) : point< T, 3 >( rhs ) {}
    point_xyz( long double x0, long double y0, long double z0 ) { x() = static_cast< T >( x0 ); y() = static_cast< T >( y0 ); z() = static_cast< T >( z0 ); }

    /// coordinates
    T& x() { return this->operator[]( 0 ); }
    T& y() { return this->operator[]( 1 ); }
    T& z() { return this->operator[]( 2 ); }
    T x() const { return this->operator[]( 0 ); }
    T y() const { return this->operator[]( 1 ); }
    T z() const { return this->operator[]( 2 ); }

    /// operations
    bool operator==( const point_xyz& rhs ) const { return point< T, 3 >::operator==( rhs ); }
    bool operator!=( const point_xyz& rhs ) const { return !operator==( rhs ); }
    const point_xyz& operator+=( const point_xyz& rhs ) { point< T, 3 >::operator+=( rhs ); return *this; }
    const point_xyz& operator-=( const point_xyz& rhs ) { point< T, 3 >::operator-=( rhs ); return *this; }
    const point_xyz& operator*=( T rhs ) { point< T, 3 >::operator*=( rhs ); return *this; }
    const point_xyz& operator/=( T rhs ) { point< T, 3 >::operator/=( rhs ); return *this; }
};

/// operators
template < typename T >
inline point_xyz< T > operator+( const point_xyz< T >& lhs, const point_xyz< T >& rhs ) { point_xyz< T > p( lhs ); p += rhs; return p; }
template < typename T >
inline point_xyz< T > operator-( const point_xyz< T >& lhs, const point_xyz< T >& rhs ) { point_xyz< T > p( lhs ); p -= rhs; return p; }
template < typename T, typename S >
inline point_xyz< T > operator*( const point_xyz< T >& lhs, S rhs ) { point_xyz< T > p( lhs ); p *= static_cast< T >( rhs ); return p; }
template < typename T, typename S >
inline point_xyz< T > operator/( const point_xyz< T >& lhs, S rhs ) { point_xyz< T > p( lhs ); p /= static_cast< T >( rhs ); return p; }

template < typename T >
struct orientation : public point< T, 3 >
{
    /// constructors
    orientation() : point< T, 3 >() {}
    orientation( const orientation& rhs ) : point< T, 3 >( rhs ) {}
    orientation( long double r, long double p, long double y ) { roll( r ); pitch( p ); yaw( y ); }

    /// return coordinates
    T roll() const { return this->operator[]( roll_index ); }
    T pitch() const { return this->operator[]( pitch_index ); }
    T yaw() const { return this->operator[]( yaw_index ); }

    /// set coordinates
    void roll( long double t );
    void pitch( long double t );
    void yaw( long double t );

    /// operations
    bool operator==( const orientation& rhs ) const { return point< T, 3 >::operator==( rhs ); }
    bool operator!=( const orientation& rhs ) const { return !operator==( rhs ); }
    const orientation& operator+=( const orientation& rhs ) { point< T, 3 >::operator+=( rhs ); return *this; }
    const orientation& operator-=( const orientation& rhs ) { point< T, 3 >::operator-=( rhs ); return *this; }
    const orientation& operator*=( T rhs ) { point< T, 3 >::operator*=( rhs ); return *this; }
    const orientation& operator/=( T rhs ) { point< T, 3 >::operator/=( rhs ); return *this; }

    /// quick and dirty
    enum { roll_index = 0, pitch_index = 1, yaw_index = 2 };
};

/// operators
template < typename T >
inline orientation< T > operator+( const orientation< T >& lhs, const orientation< T >& rhs ) { orientation< T > p( lhs ); p += rhs; return p; }
template < typename T >
inline orientation< T > operator-( const orientation< T >& lhs, const orientation< T >& rhs ) { orientation< T > p( lhs ); p -= rhs; return p; }
template < typename T, typename S >
inline orientation< T > operator*( const orientation< T >& lhs, S rhs ) { orientation< T > p( lhs ); p *= rhs; return p; }
template < typename T, typename S >
inline orientation< T > operator/( const orientation< T >& lhs, S rhs ) { orientation< T > p( lhs ); p /= rhs; return p; }

static const long double pi_ = 3.14159265358979323846l;

template < typename T >
inline void orientation< T >::yaw( long double t )
{
    long double y( comma::math::mod( static_cast< long double >( t ), pi_ * 2 ) );
    if( comma::math::less( pi_, y ) ) { y -= ( pi_ * 2 ); }
    else if( comma::math::less( y, -pi_ ) ) { y += ( pi_ * 2 ); }
    this->operator[]( yaw_index ) = static_cast< T >( y );
}

template < typename T >
inline void orientation< T >::pitch( long double t )
{
    long double p( comma::math::mod( static_cast< long double >( t ), pi_ * 2 ) );
    if( comma::math::less( pi_, p ) ) { p -= pi_ * 2; }
    if( comma::math::less( p, 0 ) )
    {
        if( comma::math::less( p, -pi_ / 2 ) )
        {
            p = -pi_ + p;
            yaw( -yaw() );
            // does roll change sign?
        }
    }
    else
    {
        if( comma::math::less( pi_ / 2, p ) )
        {
            p = pi_ - p;
            yaw( -yaw() );
            // does roll change sign?
        }
    }
    this->operator[]( pitch_index ) = static_cast< T >( p );
}

template < typename T >
inline void orientation< T >::roll( long double t )
{
    long double r( comma::math::mod( static_cast< long double >( t ), pi_ * 2 ) );
    if( comma::math::less( pi_, r ) ) { r -= ( pi_ * 2 ); }
    else if( comma::math::less( r, -pi_ ) ) { r += ( pi_ * 2 ); }
    this->operator[]( roll_index ) = static_cast< T >( r );
}

template < typename T, template < class > class P = point_xyz >
struct position
{
    typedef P< T > point;
    
    /// coordinates
    point coordinates;
    
    /// orientation
    graphics::orientation< T > orientation;
    
    /// constructors
    position() : coordinates( 0, 0, 0 ), orientation( 0, 0, 0 ) {}
    position( const position& rhs ) : coordinates( rhs.coordinates ), orientation( rhs.orientation ) {}
    position( const point& c ) : coordinates( c ), orientation( snark::orientation< T >() ) {}
    position( const point& c, const snark::orientation< T >& o ) : coordinates( c ), orientation( o ) {}
    
    /// comparison operators
    bool operator==( const position& rhs ) const { return coordinates == rhs.coordinates && orientation == rhs.orientation; }
    bool operator!=( const position& rhs ) const { return !operator==( rhs ); }
    
    /// operations (quick and dirty)
    /// @todo are they meaningful at all? currently used only for interpolation
    ///       between two navigation solutions
    position& operator+=( const position& rhs ) { coordinates += rhs.coordinates; orientation += rhs.orientation; return *this; }
    position& operator-=( const position& rhs ) { coordinates -= rhs.coordinates; orientation -= rhs.orientation; return *this; }
    position& operator*=( T rhs ) { coordinates *= rhs; orientation *= rhs; return *this; }
    position& operator/=( T rhs ) { coordinates /= rhs; orientation /= rhs; return *this; }
    position operator+( const position& rhs ) const { position p( *this ); p += rhs; return p; }
    position operator-( const position& rhs ) const { position p( *this ); p -= rhs; return p; }
    position operator*( T rhs ) const { position p( *this ); p *= rhs; return p; }
    position operator/( T rhs ) const { position p( *this ); p /= rhs; return p; }
};

} } // namespace snark { namespace graphics {

namespace snark { namespace visiting {

template < typename T > struct Traits< snark::graphics::point_xyz< T > >
{
    template < typename Key, class Visitor >
    static void visit( const Key&, const snark::graphics::point_xyz< T >& p, Visitor& v )
    {
        v.apply( "x", p.x() ); 
        v.apply( "y", p.y() ); 
        v.apply( "z", p.z() ); 
    }

    template < typename Key, class Visitor >
    static void visit( Key, snark::graphics::point_xyz< T >& p, Visitor& v )
    {
        v.apply( "x", p.x() );
        v.apply( "y", p.y() ); 
        v.apply( "z", p.z() ); 
    }
};

template < typename T > struct Traits< snark::graphics::orientation< T > >
{
    template < typename Key, class Visitor >
    static void visit( const Key&, const snark::graphics::orientation< T >& p, Visitor& v )
    {
        v.apply( "roll", p.roll() );
        v.apply( "pitch", p.pitch() );
        v.apply( "yaw", p.yaw() );

    }

    template < typename Key, class Visitor >
    static void visit( Key, snark::graphics::orientation< T >& p, Visitor& v )
    {
        v.apply( "roll", p[ orientation< T >::roll_index ] );
        v.apply( "pitch", p[ orientation< T >::pitch_index ] );
        v.apply( "yaw", p[ orientation< T >::yaw_index ] );
    }
};

template < typename T > struct Traits< snark::graphics::position< T > >
{
    template < typename Key, class Visitor >
    static void visit( const Key&, const snark::graphics::position< T >& p, Visitor& v )
    {
        v.apply( "coordinates", p.coordinates );
        v.apply( "orientation", p.orientation );
    }
    
    template < typename Key, class Visitor >
    static void visit( Key, snark::graphics::position< T >& p, Visitor& v )
    {
        v.apply( "coordinates", p.coordinates );
        v.apply( "orientation", p.orientation );
    }
};

} } // namespace snark { namespace visiting {

#endif /*RUR_GRAPHICS_POINT_POINT_H_*/
