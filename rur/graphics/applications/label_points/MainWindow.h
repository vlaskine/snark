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

#ifndef RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_MAINWINDOW_H
#define RUR_GRAPHICS_APPLICATIONS_LABELPOINTS_MAINWINDOW_H

#include <string>
#include <QMainWindow>
#include <QGridLayout>
#include <QMenu>
#include <QCheckBox>
#include <comma/Base/Types.h>
#include "./Actions.h"
#include "./IdEdit.h"
#include "./Viewer.h"

namespace snark { namespace graphics { namespace View {

class CheckBox;
    
class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow( const std::string& title, comma::graphics::View::Viewer* viewer );
    
    public slots:
        void viewerInitialized();
    
    private:
        Viewer& m_viewer;
        QActionGroup m_paintToolGroup;
        IdEdit* m_idEdit;
        QMenu* m_viewMenu;
        QMenu* m_writableMenu;
        QFrame* m_fileFrame;
        QGridLayout* m_fileLayout;
        bool m_fileFrameVisible;
        typedef std::map< std::string, std::vector< std::pair< CheckBox*, CheckBox* > > > FileGroupMap;
        FileGroupMap m_fileGroups; // quick and dirty
    
        void closeEvent( QCloseEvent* event );
        void saveAs();
        void updateFileFrame();
        void newId() const;
        void toggleFileFrame( bool shown );
        void makeFileGroups();
        void showFileGroup( std::string name, bool shown );
        void setWritableFileGroup( std::string name, bool writable );
        void keyPressEvent(QKeyEvent *e);
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

} } } // namespace snark { namespace graphics { namespace View {

#endif
