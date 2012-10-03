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

#ifndef RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_COLOURED_H_
#define RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_COLOURED_H_

#include <string>
#include <Qt3D/qcolor4ub.h>
#include "./PointWithId.h"

namespace snark { namespace graphics { namespace View {

struct coloured
{
    virtual ~coloured() {}
    virtual QColor4ub color( const Eigen::Vector3d& point
                             , comma::UInt32 id
                             , double scalar
                             , const QColor4ub& c ) const = 0;

};

class Fixed : public coloured
{
    public:
        Fixed( const std::string& name );
        QColor4ub color( const Eigen::Vector3d& point
                         , comma::UInt32 id
                         , double scalar
                         , const QColor4ub& c ) const;

    private:
        QColor4ub m_color;
};

struct ByHeight : public coloured // todo: refactor and merge with byscalar
{
    ByHeight( double from
            , double to
            , const QColor4ub& from_color = QColor4ub( 255, 0, 0 )
            , const QColor4ub& to_color = QColor4ub( 0, 0, 255 )
            , bool cyclic = false
            , bool linear = true
            , bool sharp = false );
    
    double from, to, sum, diff, middle;
    QColor4ub from_color, to_color, average_color;
    bool cyclic, linear, sharp;
    
    QColor4ub color( const Eigen::Vector3d& point
                     , comma::UInt32 id
                     , double scalar
                     , const QColor4ub& c ) const;
};

struct ByScalar : public coloured
{
    ByScalar( double from = 0
            , double to = 1
            , const QColor4ub& from_color = QColor4ub( 255, 0, 0 )
            , const QColor4ub& to_color = QColor4ub( 0, 0, 255 ) );
    double from, to, diff;
    QColor4ub from_color;
    QColor4ub to_color;
    QColor4ub color( const Eigen::Vector3d& point
                     , comma::UInt32 id
                     , double scalar
                     , const QColor4ub& c ) const;
};

class ById : public coloured
{
    public:
        ById( const QColor4ub& backgroundcolour );

        ById( const QColor4ub& backgroundcolour
            , double from
            , double to );
        QColor4ub color( const Eigen::Vector3d& point
                         , comma::UInt32 id
                         , double scalar
                         , const QColor4ub& c ) const;
    private:
        const QColor4ub m_background;
        bool m_hasScalar;
        double m_from;
        double m_diff;
};

struct ByRGB : public coloured
{
    QColor4ub color( const Eigen::Vector3d& point
                     , comma::UInt32 id
                     , double scalar
                     , const QColor4ub& c ) const;
};

coloured* colourFromString( const std::string& s, const std::string& fields, const QColor4ub& backgroundcolour );

} } } // namespace snark { namespace graphics { namespace View {

#endif /*RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_COLOURED_H_*/
