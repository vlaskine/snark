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

#ifndef SNARK_GRAPHICS_COLOUR_HEADER_GUARD_
#define SNARK_GRAPHICS_COLOUR_HEADER_GUARD_

#include <boost/lexical_cast.hpp>
#include <comma/base/exception.h>
#include <comma/math/compare.h>
#include <comma/Point/Point.h>
#include <comma/visiting/traits.h>

namespace snark { namespace graphics {

/// generic colour traits
template < typename T >
struct colour_traits
{
    typedef T numeric_type;
    static numeric_type min() { return 0; }
    static numeric_type max() { return 1; }
};

/// colour traits for char
template <>
struct colour_traits< unsigned char >
{
    typedef unsigned int numeric_type;
    static numeric_type min() { return 0; }
    static numeric_type max() { return 255; }
};

/// RGB + transparency colour type
template < typename T >
class colour : public comma::Point< T, 4 >
{
    public:
        /// default constructor
        colour();

//         /// copy constructor
//         colour( const colour& rhs );

        /// constructor
        colour( T r, T g, T b, T a = colour_traits< T >::max() );

//         /// conversion constructor
//         template < typename S >
//         colour( const colour< S >& rhs );
// 
//         /// assignment
//         const colour< T >& operator=( const colour& rhs );
// 
//         /// conversion assignment
//         template < typename S >
//         const colour< T >& operator=( const colour< S >& rhs );

        /// convert
        template < typename S >
        S as() const;

        /// return red
        T red() const;

        /// return green
        T green() const;

        /// return blue
        T blue() const;

        /// return trasparency
        T alpha() const;

        /// @todo: return hue (HSV/HSB scheme)
        T hue() const;

        /// @todo: return saturation (HSV/HSB scheme)
        T saturation() const;

        /// @todo: return brightness (HSV/HSB scheme)
        T brightness() const;

        /// set red
        colour< T >& red( T t );

        /// set green
        colour< T >& green( T t );

        /// set blue
        colour< T >& blue( T t );

        /// set transparency
        colour< T >& alpha( T t );

        /// base class
        typedef comma::Point< T, 4 > Base;

        /// operators
        /// @todo implement proper colour ariphmetics
        bool operator==( const colour& rhs ) const { return comma::Point< T, 4 >::operator==( rhs ); }
        bool operator!=( const colour& rhs ) const { return !operator==( rhs ); }
        colour operator-() const { return colour( colour_traits< T >::max() - red(), colour_traits< T >::max() - green(), colour_traits< T >::max() - blue() ); }
        const colour& operator+=( const colour& rhs ) { red( red() + rhs.red() ); green( green() + rhs.green() ); blue( blue() + rhs.blue() ); alpha( ( alpha() + rhs.alpha() ) / 2 ); return *this; }
        const colour& operator*=( double d );
        const colour& operator-=( const colour& rhs ) { return operator+=( -rhs ); }
        const colour& operator/=( double d ) { return operator*=( 1 / d ); }
        colour operator+( const colour& rhs ) const { colour c( *this ); c += rhs; return c; }
        colour operator-( const colour& rhs ) const { colour c( *this ); c -= rhs; return c; }
        colour operator*( double d ) const { colour c( *this ); c *= d; return c; }
        colour operator/( double d ) const { colour c( *this ); c /= d; return c; }

        /// colour from string like 0x123456 or 0x12345678
        static colour< T > fromString( const std::string& rgba );

        /// const [] operator
        const T& operator[]( std::size_t i ) const { return comma::Point< T, 4 >::operator[]( i ); }

    private:
        T& operator[]( std::size_t i ) { return comma::Point< T, 4 >::operator[]( i ); }
};

namespace impl {

template < typename T >
inline void validate( T t )
{
    if( Math::less( t, colour_traits< T >::min() ) || Math::less( colour_traits< T >::max(), t ) )
    {
        COMMA_THROW( comma::exception, "expected value in [" << colour_traits< T >::min() << ", " << colour_traits< T >::max() << "], got " << t );
    }
}

template < typename T > T fromString( const std::string& s ) { return boost::lexical_cast< T >( s ); }

template <>
inline unsigned char fromString< unsigned char >( const std::string& s ) // todo: check value
{
    unsigned char high = static_cast< unsigned char >( s.at( 0 ) );
    unsigned char low = static_cast< unsigned char >( s.at( 1 ) );
    return 16 * ( '0' <= high && high <= '9' ? high - '0' : 'a' <= high && high <= 'f' ? high - 'a' - 10 : high - 'A' - 10 )
                + '0' <= low && low <= '9' ? low - '0' : 'a' <= low && low <= 'f' ? low - 'a' - 10 : low - 'A' - 10;
}

} // namespace impl {

template <>
inline colour< unsigned char > colour< unsigned char >::fromString( const std::string& rgba )
{
    if( ( rgba.length() != 8 && rgba.length() != 10 ) || rgba.at( 0 ) != '0' || ( rgba.at( 1 ) != 'x' && rgba.at( 1 ) != 'X' ) ) { COMMA_THROW( graphics::exception, "expected hex colour, got \"" << rgba << "\"" ); }
    colour< unsigned char > c( impl::fromString< unsigned char >( rgba.substr( 2 ) )
                            , impl::fromString< unsigned char >( rgba.substr( 4 ) )
                            , impl::fromString< unsigned char >( rgba.substr( 6 ) ) );
    if( rgba.length() == 10 ) { c.alpha( impl::fromString< unsigned char >( rgba.substr( 8 ) ) ); }
    return c;
}

template < typename T >
inline colour< T >::colour() {}

// template < typename T >
// inline colour< T >::colour( const colour& rhs ) { operator=( rhs ); }

template < typename T >
inline colour< T >::colour( T r, T g, T b, T a )
{
    red( r );
    green( g );
    blue( b );
    alpha( a );
}

// template < typename T >
// template < typename S >
// inline colour< T >::colour( const colour< S >& rhs ) { operator=( rhs ); }
// 
// template < typename T >
// inline const colour< T >& colour< T >::operator=( const colour& rhs )
// {
//     this->comma::Point< T, 4 >::operator=( rhs );
//     return *this;
// }
// 
// template < typename T >
// template < typename S >
// inline const colour< T >& colour< T >::operator=( const colour< S >& rhs )
// {
//     for( unsigned int i = 0; i < Base::Dimension; ++i )
//     {
//         this->operator[]( i ) = static_cast< T >( ( ( colour_traits< T >::max() - colour_traits< T >::min() ) * rhs.colour< S >::Base::operator[]( i ) ) / ( colour_traits< S >::max() - colour_traits< S >::min() ) );
//     }
//     return *this;
// }

template < typename T >
template < typename S >
inline S colour< T >::as() const
{
    typedef typename S::Type Type;
    static const Type fs = colour_traits< Type >::max() - colour_traits< Type >::min();
    static const Type ft = colour_traits< T >::max() - colour_traits< T >::min();
    return S( static_cast< Type >( ( float( this->red() ) / ft ) * fs )
            , static_cast< Type >( ( float( this->green() ) / ft ) * fs )
            , static_cast< Type >( ( float( this->blue() ) / ft ) * fs )
            , static_cast< Type >( ( float( this->alpha() ) / ft ) * fs ) );
}

template < typename T >
inline T colour< T >::red() const { return const_cast< colour* >( this )->operator[]( 0 ); } // a quick fix

template < typename T >
inline T colour< T >::green() const { return const_cast< colour* >( this )->operator[]( 1 ); }

template < typename T >
inline T colour< T >::blue() const { return const_cast< colour* >( this )->operator[]( 2 ); }

template < typename T >
inline T colour< T >::alpha() const { return const_cast< colour* >( this )->operator[]( 3 ); }

template < typename T >
inline colour< T >& colour< T >::red( T t )
{
    impl::validate( t );
    this->operator[]( 0 ) = t;
    return *this;
}

template < typename T >
inline colour< T >& colour< T >::green( T t )
{
    impl::validate( t );
    this->operator[]( 1 ) = t;
    return *this;
}

template < typename T >
inline colour< T >& colour< T >::blue( T t )
{
    impl::validate( t );
    this->operator[]( 2 ) = t;
    return *this;
}

template < typename T >
inline colour< T >& colour< T >::alpha( T t )
{
    impl::validate( t );
    this->operator[]( 3 ) = t;
    return *this;
}

template < typename T >
const colour< T >& colour< T >::operator*=( double d ) // todo: quick and dirty now
{
    float f = static_cast< float> ( d );
    red( red() * f );
    green( green() * f );
    blue( blue() * f );
    alpha( alpha() * f );
    return *this;
}

template < typename T >
T colour< T >::hue() const
{
    // todo
}

template < typename T >
T colour< T >::saturation() const
{
    // todo
}

template < typename T >
T colour< T >::brightness() const
{
    // todo
}

} } // namespace snark { namespace graphics {

namespace snark { namespace Visiting {

/// visiting traits
template < typename T > struct traits< snark::graphics::colour< T > >
{
    /// const visiting
    template < typename Key, class Visitor >
    static void visit( const Key&, const snark::graphics::colour< T >& p, Visitor& v )
    {
        v.apply( "r", p.red() );
        v.apply( "g", p.green() );
        v.apply( "b", p.blue() );
        v.apply( "a", p.alpha() );
    }

    /// visiting
    template < typename Key, class Visitor >
    static void visit( Key, snark::graphics::colour< T >& p, Visitor& v )
    {
        T r = p.red();
        T g = p.green();
        T b = p.blue();
        T a = p.alpha();
        v.apply( "r", r );
        v.apply( "g", g );
        v.apply( "b", b );
        v.apply( "a", a );
        p = snark::graphics::colour< T >( r, g, b, a );
    }
};

} } // namespace snark { namespace Visiting {

#endif /*SNARK_GRAPHICS_COLOUR_HEADER_GUARD_*/
