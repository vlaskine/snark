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

#ifndef SNARK_GRAPHICS_IMPL_ROTATION_MATRIX_H_
#define SNARK_GRAPHICS_IMPL_ROTATION_MATRIX_H_

#include <comma/base/exception.h>
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace snark {

/// rotation matrix (a quick and dirty class, to refactor at some point)
class rotation_matrix 
{
public:
    rotation_matrix( const ::Eigen::Matrix3d& rotation =  ::Eigen::Matrix3d::Identity() );
    rotation_matrix( const ::Eigen::Quaterniond& quaternion );
    rotation_matrix( const ::Eigen::Vector3d& rpy );

    const ::Eigen::Matrix3d& rotation() const;
    ::Eigen::Quaterniond quaternion() const;
    ::Eigen::Vector3d roll_pitch_yaw() const;
    static ::Eigen::Vector3d roll_pitch_yaw( const ::Eigen::Matrix3d& m );
    static ::Eigen::Matrix3d rotation( const ::Eigen::Vector3d& rpy );
    static ::Eigen::Matrix3d rotation( double roll, double pitch, double yaw );

    /// convert to requested type
    template< typename Output >
    Output convert() const;

private:
    ::Eigen::Matrix3d m_rotation;
    
};

} // namespace snark {

#endif // SNARK_GRAPHICS_IMPL_ROTATION_MATRIX_H_
