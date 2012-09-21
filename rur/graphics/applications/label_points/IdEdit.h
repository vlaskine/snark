// This file is part of rur, a generic and flexible library
// for robotics research.
//
// Copyright (C) 2011 The University of Sydney
//
// rur is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// rur is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public
// License along with rur. If not, see <http://www.gnu.org/licenses/>.

#ifndef RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_IDEDIT_H_
#define RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_IDEDIT_H_

#include <boost/optional.hpp>
#include <qobject.h>
#include <qlineedit.h>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <comma/Base/Types.h>

namespace rur { namespace graphics { namespace View {

class Viewer;

class IdEdit : public QFrame
{
    Q_OBJECT

    public:
        IdEdit( Viewer* viewer );

    signals:
        void valueChanged( comma::UInt32 );

    public slots:
        void setValue( comma::UInt32 );
        void handleReturnPressed();

    private:
        Viewer* m_viewer;
        boost::optional< comma::UInt32 > m_id;
        QLineEdit* m_edit;
        QLabel* m_size;
};

} } } // namespace rur { namespace graphics { namespace View {

#endif // RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_IDEDIT_H_
