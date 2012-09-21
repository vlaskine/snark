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

#ifndef RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_MAINWINDOW_H_
#define RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_MAINWINDOW_H_

#include <map>
#include <string>
#include <vector>
#include <QCheckBox>
#include <QMainWindow>
#include <comma/Base/Types.h>
#include "./Viewer.h"

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QFrame;
class QGridLayout;
class QMenu;
class QToolBar;
QT_END_NAMESPACE

namespace rur { namespace graphics { namespace View {

class CheckBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow( const std::string& title, comma::graphics::View::Viewer* viewer );
    
    private:
        QMenu* m_viewMenu;
        Viewer& m_viewer;
        QFrame* m_fileFrame;
        QGridLayout* m_fileLayout;
        bool m_fileFrameVisible;
        typedef std::map< std::string, std::vector< CheckBox* > > FileGroupMap;
        FileGroupMap m_fileGroups; // quick and dirty
    
        void closeEvent( QCloseEvent* event );
        void keyPressEvent( QKeyEvent *e );
        void updateFileFrame();
        void toggleFileFrame( bool shown );
        void makeFileGroups();
        void showFileGroup( std::string name, bool shown );
};

class CheckBox : public QCheckBox // quick and dirty
{
    Q_OBJECT
    
    public:
        CheckBox( boost::function< void( bool ) > f );
    
    public slots:
        void action( bool checked );
    
    private:
        boost::function< void( bool ) > m_f;
};

} } } // namespace rur { namespace graphics { namespace View {

#endif /*RUR_GRAPHICS_APPLICATIONS_VIEWPOINTS_MAINWINDOW_H_*/
