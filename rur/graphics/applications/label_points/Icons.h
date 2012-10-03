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

#ifndef RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_ICONS_H_
#define RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_ICONS_H_

#include <qicon.h>

namespace snark { namespace graphics { namespace View {

struct Icons
{    
    static QIcon save() { return QIcon( ":/Images/save.png" ); }
    static QIcon select() { return QIcon( ":/Images/select.png" ); }
    static QIcon fuzzy() { return QIcon( ":/Images/fuzzy-selection.png" ); }
    static QIcon fuzzyId() { return QIcon( ":/Images/fuzzy-selection-id.png" ); }
    static QIcon pipette() { return QIcon( ":/Images/pipette.png" ); }
    static QIcon bucket() { return QIcon( ":/Images/bucket.png" ); }
    static QIcon pointer() { return QIcon( ":/Images/pointer.png" ); }
};

} } } // namespace snark { namespace graphics { namespace View {

#endif // RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_ICONS_H_

