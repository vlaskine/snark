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

#ifndef SNARK_GRAPHICS_APPLICATIONS_LABELPOINTS_POINTMAP_H_
#define SNARK_GRAPHICS_APPLICATIONS_LABELPOINTS_POINTMAP_H_

#include <map>
#include <sstream>
#include <string>
#include <boost/optional.hpp>
#include <boost/static_assert.hpp>
#include <comma/math/compare.h>

namespace snark {

namespace impl {

template< typename K, typename T, std::size_t D, typename Compare = std::less< T > >
struct MultidimensionalMapimpl : public std::map< K, MultidimensionalMapimpl< K, T, D - 1, Compare >, Compare >
{
    typedef MultidimensionalMapimpl< K, T, D - 1, Compare > BaseValueType;
    typedef std::map< K, BaseValueType, Compare > Base;
    
    template < typename P >
    std::vector< T* > find( const P& p )
    {
        typename Base::iterator it = this->Base::find( p[ p.size() - D ] );
        if( it == this->Base::end() ) { return std::vector< T* >(); }
        return it->second.find( p );
    }    

    template < typename P >
    void erase( const P& p )
    {
        typename Base::iterator it = this->Base::find( p[ p.size() - D ] );
        if( it == this->Base::end() ) { return; }
        it->second.erase( p );
        if( it->second.empty() ) { this->Base::erase( it ); }
    }

    template < typename P >
    void insert( const P& p, const T& t ) { this->Base::operator[]( p[ p.size() - D ] ).insert( p, t ); }
    
    template < typename P >
    void find( MultidimensionalMapimpl& m, const P& lower, const P& upper ) const
    {
        typename Base::const_iterator i = this->Base::lower_bound( lower[ lower.size() - D ] );
        typename Base::const_iterator j = this->Base::upper_bound( upper[ upper.size() - D ] );
        for( ; i != j; ++i )
        { 
            i->second.find( m[ i->first ], lower, upper );
            if( m[ i->first ].empty() ) { m.MultidimensionalMapimpl::Base::erase( i->first ); } // quick and dirty, fix later
        }
    }
    
    std::size_t size() const
    {
        std::size_t s = 0;
        for( typename Base::const_iterator i = this->Base::begin(); i != this->Base::end(); ++i ) { s += i->second.size(); }
        return s;
    }  
    
    struct Enumerator // too lame to be called iterator
    {
        void operator++()
        {
            if( it == map->end() ) { return; }
            if( en.it != en.map->end() ) { ++en; if( en.it != en.map->end() ) { return; } }
            ++it;
            if( it == map->end() ) { return; }
            it->second.setBegin( en );
        }
        template < typename P >
        void get( P& key, T*& value )
        {
            if( it == map->end() ) { value = NULL; return; }
            key[ key.size() - D ] = it->first;
            en.get( key, value );
        }
        typename Base::iterator it;
        typename BaseValueType::Enumerator en;
        MultidimensionalMapimpl* map;
    };
    
    void setBegin( Enumerator& e )
    { 
        e.map = const_cast< MultidimensionalMapimpl* >( this );
        e.it = e.map->Base::begin();
        if( e.it != this->Base::end() ) { e.it->second.setBegin( e.en ); }
    }
    
    void toString( std::ostringstream& oss, const std::string& indent = "" ) const
    {
        oss << indent << "[" << D << "]: size = " << this->Base::size() << ":" << std::endl;
        for( typename Base::const_iterator it = this->Base::begin(); it != this->Base::end(); ++it )
        {
            oss << indent << "    " << it->first << ":" << std::endl;
            it->second.toString( oss, indent + "        " );
        }
    }    
};

template< typename K, typename T, typename Compare >
struct MultidimensionalMapimpl< K, T, 1, Compare > : public std::multimap< K, T, Compare > // public std::multimap< K, T, Compare >
{
    typedef std::multimap< K, T, Compare > Base; // typedef std::multimap< K, T, Compare > Base;

    template < typename P >
    std::vector< T* > find( const P& p )
    {
        std::vector< T* > v;
        typename Base::iterator it = this->Base::lower_bound( p[ p.size() - 1 ] );
        typename Base::iterator upper = this->Base::upper_bound( p[ p.size() - 1 ] );
        for( ; it != this->Base::end() && it != upper; ++it ) { v.push_back( &it->second ); }
        return v;
    }
    
    template < typename P >
    void erase( const P& p ) { this->Base::erase( p[ p.size() - 1 ] ); }

    template < typename P >
    void insert( const P& p, const T& t ) { this->Base::insert( std::make_pair( p[ p.size() - 1 ], t ) ); }

    template < typename P >
    void find( MultidimensionalMapimpl& m, const P& lower, const P& upper ) const
    {
        if( comma::math::less( upper[ lower.size() - 1 ], lower[ lower.size() - 1 ] ) ) { return; }
        typename Base::const_iterator l = this->Base::lower_bound( lower[ lower.size() - 1 ] );
        typename Base::const_iterator u = this->Base::upper_bound( upper[ upper.size() - 1 ] );
        if( l == this->Base::end() && u == this->Base::end() ) { return; }
        m.Base::insert( l, u );
    }
    
    struct Enumerator // too lame to be called iterator
    {
        void operator++() { if( it != map->end() ) { ++it; } }
        template < typename P >
        void get( P& key, T*& value )
        {
            if( it == map->end() ) { value = NULL; return; }
            key[ key.size() - 1 ] = it->first;
            value = &it->second;
        }
        typename Base::iterator it;
        MultidimensionalMapimpl* map;
    };
    
    void setBegin( Enumerator& e ) { e.map = const_cast< MultidimensionalMapimpl* >( this ); e.it = e.map->Base::begin(); }
    
    void toString( std::ostringstream& oss, const std::string& indent ) const
    {
        for( typename Base::const_iterator it = this->Base::begin(); it != this->Base::end(); ++it )
        {
            oss << indent << it->first << ":" << it->second << std::endl;
        }
    }
};

template < typename T > struct Less { bool operator()( const T& s, const T& t ) const { return comma::math::less( s, t ); } };

} // namespace impl {

template< typename P, typename T >
struct PointMap : public impl::MultidimensionalMapimpl< typename P::Scalar, T, P::RowsAtCompileTime, impl::Less< typename P::Scalar > >
{
    typedef typename impl::MultidimensionalMapimpl< typename P::Scalar, T, P::RowsAtCompileTime, impl::Less< typename P::Scalar > > Base;
    
    //const T* find( const P& p ) const { return const_cast< PointMap* >( this )->Base::find( p ); }
    //T* find( const P& p ) { return Base::find( p ); }
    //std::vector< const T* > find( const P& p ) const { return const_cast< PointMap* >( this )->Base::find( p ); }
    // a nasty hack violating constness: fix! well, later...
    std::vector< T* > find( const P& p ) const { return const_cast< PointMap* >( this )->Base::find( p ); }
    std::vector< T* > find( const P& p ) { return Base::find( p ); }
    void insert( const P& p, const T& t ) { Base::insert( p, t ); }
    void insert( const PointMap& m ) { for( PointMap::ConstEnumerator en = m.begin(); !en.end(); ++en ) { Base::insert( en.key(), en.value() ); } }
    void erase( const P& p ) { return Base::erase( p ); }
    void erase( const PointMap& m ) { for( PointMap::ConstEnumerator en = m.begin(); !en.end(); ++en ) { Base::erase( en.key() ); } }
    PointMap find( const P& lower, const P& upper ) const { PointMap m; Base::find( m, lower, upper ); return m; }
    std::size_t size() const { return Base::size(); }
    
    class ConstEnumerator // quick and dirty; too lame to be called iterator
    {
        public:
            ConstEnumerator& operator++() { ++m_enum; m_enum.get( m_key, m_value ); return *this; }
            bool end() const { return m_value == NULL; }
            const P& key() const { return m_key; }
            //T& value() { return *m_value; }
            const T& value() const { return *m_value; }
            
        protected:
            friend struct PointMap;
            P m_key;
            T* m_value;
            typename PointMap::Base::Enumerator m_enum;
    };
    
    class Enumerator : public ConstEnumerator // quick and dirty; too lame to be called iterator
    {
        public:
            T& value() { return *this->m_value; }
    };
    
    ConstEnumerator begin() const { ConstEnumerator e; const_cast< PointMap* >( this )->Base::setBegin( e.m_enum ); e.m_enum.get( e.m_key, e.m_value ); return e; }
    Enumerator begin() { Enumerator e; this->Base::setBegin( e.m_enum ); e.m_enum.get( e.m_key, e.m_value ); return e; }
    
    std::string toString() const // quick and dirty; for testing
    {
        std::ostringstream oss;
        this->Base::toString( oss );
        return oss.str();
    }
};

} // namespace snark {

#endif // SNARK_GRAPHICS_APPLICATIONS_LABELPOINTS_POINTMAP_H_

