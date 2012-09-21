// This file is part of rur, a generic and flexible library 
// for robotics research.
//
// Copyright (C) 2011 The University of Sydney
//
// rur is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// rur is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
// for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with rur. If not, see <http://www.gnu.org/licenses/>.

#ifndef RUR_GRAPHICS_EXTENTS_HEADER_GUARD_
#define RUR_GRAPHICS_EXTENTS_HEADER_GUARD_

#include <boost/array.hpp>
#include <boost/optional.hpp>
#include <comma/math/compare.h>
#include <comma/visiting/visit.h>

namespace rur { namespace graphics {

/// a utility class to find extents of a set of array-like objects (e.g. points)
template < typename P >
class extents
{
public:
    /// extents pair type
    typedef std::pair< P, P > pair;
    
    /// constructor from min and max
    extents( const P& min, const P& max );
    
    /// constructor from a container
    template < typename It >
    extents( It begin, It end );
    
    /// constructor from min/max pair
    extents( const pair& extents );
    
    /// default constructor
    extents();
    
    /// copy constructor
    extents( const extents& rhs );
    
    /// add point
    const pair& add( const P& p );
    
    /// add extents
    const pair& add( const extents< P >& rhs );
    
    /// return extents, throw, if no points have been added yet
    const pair& operator()() const { return *extents_; }

    /// return min
    const P& min() const { return extents_->first; }

    /// return max
    const P& max() const { return extents_->second; }
    
    /// return box size, i.e. max - min
    P boxSize() const { return extents_->second - extents_->first; }

    /// return number of points added
    std::size_t size() const { return size_; }
    
    /// return true, if extents contain given value
    bool has( const P& p ) const;
    
private:
    boost::optional< pair > extents_;
    std::size_t size_;
};

/// output operator
template < typename Stream, typename P >
Stream& operator<<( Stream& lhs, const extents< P >& rhs )
{
    lhs << rhs().first << "," << rhs().second;
    return lhs;
}

template < typename T > struct extents_traits // quick and dirty
{
    enum { size = 1 };
    T zero() { return T( 0 ); }
    T one() { return T( 1 ); }
    T identity() { return T( 1 ); }
};

template < typename T, std::size_t Size > class extents_traits< boost::array< T, Size > >
{
    public:
        enum { size = Size };
        static const boost::array< T, Size >& zero() { static boost::array< T, Size > z = zero_(); return z; }
        
    private:
        static boost::array< T, Size > zero_()
        {
            boost::array< T, Size > z;
            for( std::size_t i = 0; i < Size; ++i ) { z[i] = extents_traits< T >::zero(); }
            return z;
        }
};


template < typename P >
extents< P >::extents() : size_( 0 ) {}

template < typename P >
extents< P >::extents( const extents< P >& rhs ) { this->operator=( rhs ); }

template < typename P >
extents< P >::extents( const P& min, const P& max )
    : size_( 0 ) /// @todo what does size need to be?
{
    add( min );
    add( max );
}

template < typename P >
extents< P >::extents( const typename extents< P >::pair& extents )
    : extents_( extents )
    , size_( 0 ) /// @todo what does size need to be?
{
}

template < typename P >
const typename extents< P >::pair& extents< P >::add( const P& p )
{
    if( !extents_ )
    {
        extents_ = std::make_pair( p, p );
    }
    else
    {
        for( unsigned int i = 0; i < extents_traits< P >::size; ++i )
        {
            if( comma::math::less( p[i], extents_->first[i] ) ) { extents_->first[i] = p[i]; }
            else if( comma::math::less( extents_->second[i], p[i] ) ) { extents_->second[i] = p[i]; }
        }
    }
    ++size_;
    return *extents_;
}

template < typename P >
const typename extents< P >::pair& extents< P >::add( const extents< P >& rhs )
{
    add( rhs.min() );
    add( rhs.max() );
    size_ += rhs.size_;
    return *extents_;
}

template < typename P >
bool extents< P >::has( const P& p ) const
{
    extents< P > e( *this );
    e.add( p );
    return comma::math::equal( this->min(), e.min() ) && comma::math::equal( this->max(), e.max() );}

template < typename P >
template < typename It >
extents< P >::extents( It begin, It end )
{
    for( It it = begin; it != end; ++it ) { add( *it ); }
}  

} } // namespace rur { namespace graphics {

namespace comma { namespace visiting {

template < typename T > struct Traits< extents< T > >
{
    template < typename Key, class Visitor >
    static void visit( const Key&, const extents< T >& p, Visitor& v ) // very quick and dirty, very non-generic; need to think how to do it better
    {
        if( p.size() == 0 ) // real frigging quick and dirty, just to make things working again and think later
        {
            T t;
            v.apply( "min", t );
            v.apply( "max", t );
        }
        else
        {
            v.apply( "min", p.min() );
            v.apply( "max", p.max() );
        }
    }

    template < typename Key, class Visitor >
    static void visit( Key, extents< T >& p, Visitor& v ) // very quick and dirty, very non-generic; need to think how to do it better
    {
        extents< T > e;
        T t;
        v.apply( "min", t );
        e.add( t );
        v.apply( "max", t );
        e.add( t );
        p = e;
    }
};

} } // namespace comma { namespace visiting {

#endif // #ifndef RUR_GRAPHICS_EXTENTS_HEADER_GUARD_
