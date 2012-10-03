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

#ifndef RUR_GRAPHICS_IMPL_RENDERABLE_H_
#define RUR_GRAPHICS_IMPL_RENDERABLE_H_

#include <snark/graphics/traits.h>

namespace snark { namespace graphics { namespace impl {

template < typename T >
class renderable
{
    public:
        typedef T ValueType;

        renderable( std::size_t capacity ) : m_storage( capacity ) {}

        void render() { m_storage.render(); }

        void unmap() { m_storage.unmap(); } // todo: quick hack, since it's opengl-specific; improve semantics

    protected:
        typename graphics::traits< T >::storage_type m_storage;
};

} } } // namespace snark { namespace graphics { namespace impl {

#endif // RUR_GRAPHICS_IMPL_RENDERABLE_H_
