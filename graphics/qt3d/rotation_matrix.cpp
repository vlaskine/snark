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

#include "./rotation_matrix.h"

namespace snark {

/// constructor from rotation matrix    
rotation_matrix::rotation_matrix( const Eigen::Matrix3d& rotation ):
    m_rotation( rotation )
{

}

/// constructor from quaternion
rotation_matrix::rotation_matrix(const Eigen::Quaterniond& quaternion):
    m_rotation( quaternion.normalized() )
{

}

/// constructor from roll pitch yaw angles
rotation_matrix::rotation_matrix( const Eigen::Vector3d& rpy )
    : m_rotation( rotation( rpy ) )
{
}

/// get rotation matrix
const Eigen::Matrix3d& rotation_matrix::rotation() const
{
    return m_rotation;
}

/// convert to quaternion
Eigen::Quaterniond rotation_matrix::quaternion() const
{
    return Eigen::Quaterniond( m_rotation );
}

/// convert to roll pitch yaw
Eigen::Vector3d rotation_matrix::roll_pitch_yaw() const
{
    return roll_pitch_yaw( m_rotation );
}

Eigen::Vector3d rotation_matrix::roll_pitch_yaw( const ::Eigen::Matrix3d& m )
{
    Eigen::Vector3d rpy;
    rpy << std::atan2( m(2,1) , m(2,2) ),
           std::asin( -m(2,0) ),
           std::atan2( m(1,0), m(0,0) );
    return rpy;
}

::Eigen::Matrix3d rotation_matrix::rotation( const ::Eigen::Vector3d& rpy )
{
    return rotation( rpy.x(), rpy.y(), rpy.z() );
}

::Eigen::Matrix3d rotation_matrix::rotation( double roll, double pitch, double yaw )
{
    ::Eigen::Matrix3d m;
    const double sr = std::sin( roll );
    const double cr = std::cos( roll );
    const double sp = std::sin( pitch );
    const double cp = std::cos( pitch );
    const double sy = std::sin( yaw );
    const double cy = std::cos( yaw );
    const double spcy = sp*cy;
    const double spsy = sp*sy;
    m << cp*cy, -cr*sy+sr*spcy,  sr*sy+cr*spcy,
         cp*sy,  cr*cy+sr*spsy, -sr*cy+cr*spsy,
           -sp,          sr*cp,          cr*cp;
    return m;
}

// template< typename Output >
// Output rotation_matrix::convert() const
// {
//     SNARK_THROW( Exception, " conversion to this type not implemented " );
// }

template<>
Eigen::Quaterniond rotation_matrix::convert< Eigen::Quaterniond >() const
{
    return quaternion();
}

template<>
Eigen::Vector3d rotation_matrix::convert< Eigen::Vector3d >() const
{
    return roll_pitch_yaw();
}

} // namespace snark {

