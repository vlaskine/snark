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

#include <limits>
#include <map>
#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include <QtGui>
#include <QDialog>
#include <QLabel>
#include <comma/Base/Types.h>
#include "./Actions.h"
#include "./MainWindow.h"
#include "./Viewer.h"

namespace snark { namespace graphics { namespace View {

MainWindow::MainWindow( const std::string& title, Viewer* viewer )
    : m_viewer( *viewer )
    , m_paintToolGroup( NULL )
    , m_fileFrameVisible( true )
{
    Actions::Action* saveAction = new Actions::Action( "Save", boost::bind( &Viewer::save, boost::ref( m_viewer ) ) );
    saveAction->setIcon( QIcon::fromTheme("document-save", style()->standardIcon( QStyle::SP_DialogSaveButton ) ) );
    saveAction->setShortcuts( QKeySequence::Save );
    Actions::Action* saveAsAction = new Actions::Action( "SaveAs...", boost::bind( &MainWindow::saveAs, this ) );
    Actions::Action* reloadAction = new Actions::Action( "Reload", boost::bind( &Viewer::reload, viewer ) );

    Actions::ToggleAction* navigateSceneAction = new Actions::ToggleAction( QIcon::fromTheme("edit-select", Icons::pointer() ) , "navigate scene", boost::bind( &Tools::Navigate::toggle, boost::ref( viewer->navigate ), _1 ), "Ctrl+Q" );
    Actions::ToggleAction* selectPointsAction = new Actions::ToggleAction( QIcon::fromTheme("zoom-select", Icons::select()), "select points", boost::bind( &Tools::SelectClip::toggle, boost::ref( viewer->selectClip ), _1 ), "Ctrl+W" );
    Actions::ToggleAction* selectPartitionAction = new Actions::ToggleAction( QIcon::fromTheme("tools-wizard", Icons::fuzzy()), "select partition", boost::bind( &Tools::SelectPartition::toggle, boost::ref( viewer->selectPartition ), _1 ), "Ctrl+E" );
    Actions::ToggleAction* selectIdAction = new Actions::ToggleAction( QIcon::fromTheme("tools-wizard", Icons::fuzzy()), "select id", boost::bind( &Tools::SelectPartition::toggle, boost::ref( viewer->selectId ), _1 ), "Ctrl+R" );
    Actions::ToggleAction* pipetteAction = new Actions::ToggleAction( QIcon::fromTheme("color-picker", Icons::pipette()), "pick id", boost::bind( &Tools::PickId::toggle, boost::ref( viewer->pickId ), _1 ), "Ctrl+T" );
    Actions::ToggleAction* bucketAction = new Actions::ToggleAction( QIcon::fromTheme("fill-color", Icons::bucket()), "set id", boost::bind( &Tools::Fill::toggle, boost::ref( viewer->fill ), _1 ), "Ctrl+Y" );

    selectPointsAction->setToolTip( "select points, click and drag<br>"
                                    "hold Ctrl to add to current selection<br>"
                                    "hold Shift to remove from current selection" );
    selectPartitionAction->setToolTip( "select partition<br>"
                                    "hold Ctrl to add to current selection<br>"
                                    "hold Shift to remove from current selection" );
    selectIdAction->setToolTip( "select all partitions with the same id<br>"
                                "hold Ctrl to add to current selection<br>"
                                "hold Shift to remove from current selection" );
    bucketAction->setToolTip( "set id of current selection<br>"
                            "if nothing selected, set id of the point under cursor" );

    navigateSceneAction->setCheckable( true );
    navigateSceneAction->setChecked( true );

    Actions::Action* shakeColorsAction = new Actions::Action( "Shake colors", boost::bind( &Tools::PickId::shakeColors, boost::ref( viewer->pickId ) ) );

    QMenu* fileMenu = menuBar()->addMenu( "File" );
    fileMenu->addAction( saveAction );
    fileMenu->addAction( saveAsAction );
    fileMenu->addAction( reloadAction );
    fileMenu->addSeparator();
    fileMenu->addAction( shakeColorsAction );
    menuBar()->addMenu( fileMenu );

    QToolBar* fileToolBar = addToolBar( "File" );
    fileToolBar->addAction( saveAction );

    m_fileFrame = new QFrame;
    m_fileFrame->setFrameStyle( QFrame::Plain | QFrame::NoFrame );
    m_fileFrame->setFixedWidth( 300 );
    m_fileFrame->setContentsMargins( 0, 0, 0, 0 );
    m_fileLayout = new QGridLayout;
    m_fileLayout->setSpacing( 0 );
    m_fileLayout->setContentsMargins( 5, 5, 5, 0 );

    QLabel* filenameLabel = new QLabel( "<b>filename</b>" );
    QLabel* visibleLabel = new QLabel( "<b>view</b>" );
    QLabel* writableLabel = new QLabel( "<b>r/w</b>" );
    visibleLabel->setToolTip( "check to make file writable" );
    writableLabel->setToolTip( "check to make file writable" );
    m_fileLayout->addWidget( filenameLabel, 0, 0, Qt::AlignLeft | Qt::AlignTop );
    m_fileLayout->addWidget( visibleLabel, 0, 1, Qt::AlignRight | Qt::AlignTop );
    m_fileLayout->addWidget( writableLabel, 0, 2, Qt::AlignRight | Qt::AlignTop );
    m_fileLayout->setRowStretch( 0, 0 );
    m_fileLayout->setColumnStretch( 0, 10 ); // quick and dirty
    m_fileLayout->setColumnStretch( 1, 1 );
    m_fileLayout->setColumnStretch( 2, 1 );
    m_fileFrame->setLayout( m_fileLayout );

    QFrame* frame = new QFrame;
    frame->setFrameStyle( QFrame::Plain | QFrame::NoFrame );
    frame->setContentsMargins( 0, 0, 0, 0 );
    QGridLayout* layout = new QGridLayout;
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->setSpacing( 0 );
    layout->addWidget( m_fileFrame, 0, 0 );
    layout->addWidget( viewer, 0, 1 );
    layout->setColumnStretch( 0, 0 );
    layout->setColumnStretch( 1, 1 );
    frame->setLayout( layout );
    setCentralWidget( frame );
    resize( 640, 480 );

    m_viewMenu = menuBar()->addMenu( "View" );
    Actions::ToggleAction* action = new Actions::ToggleAction( "File Panel", boost::bind( &MainWindow::toggleFileFrame, this, _1 ) );
    action->setChecked( m_fileFrameVisible );
    m_viewMenu->addAction( action );
    connect( viewer, SIGNAL( initialized() ), this, SLOT( viewerInitialized() ) );
    updateFileFrame();

    m_paintToolGroup.addAction( selectPointsAction);
    m_paintToolGroup.addAction( navigateSceneAction );
    m_paintToolGroup.addAction( selectPartitionAction );
    m_paintToolGroup.addAction( selectIdAction );
    m_paintToolGroup.addAction( pipetteAction );
    m_paintToolGroup.addAction( bucketAction );

    QToolBar* paintToolBar = addToolBar( "Tools" );
    paintToolBar->addAction(navigateSceneAction);
    paintToolBar->addAction( selectPointsAction );
    paintToolBar->addAction( selectPartitionAction );
    paintToolBar->addAction( selectIdAction );
    paintToolBar->addAction( pipetteAction );
    paintToolBar->addAction( bucketAction );

    m_idEdit = new IdEdit( viewer );
    paintToolBar->addWidget( m_idEdit );
    connect( &viewer->pickId, SIGNAL( valueChanged( comma::uint32 ) ), m_idEdit, SLOT( setValue( comma::uint32 ) ) );
    connect( m_idEdit, SIGNAL( valueChanged( comma::uint32 ) ), viewer, SLOT( handleId( comma::uint32 ) ) );
    Actions::Action* newIdAction = new Actions::Action( "newId", boost::bind( &MainWindow::newId, this ) );
    newIdAction->setShortcut( QKeySequence( "Ctrl+N" ) );
    addAction( newIdAction );

    QMenu* toolsMenu = menuBar()->addMenu( "Tools" );
    toolsMenu->addAction( navigateSceneAction );
    toolsMenu->addAction( selectPointsAction );
    toolsMenu->addAction( selectPartitionAction );
    toolsMenu->addAction( selectIdAction );
    toolsMenu->addAction( pipetteAction );
    toolsMenu->addAction( bucketAction );

    setWindowTitle( title.c_str() );
}

void MainWindow::newId() const // quick and dirty (eventually move it to viewer, maybe)
{
    boost::optional< comma::uint32 > id;
    for( std::size_t i = 0; i < m_viewer.datasets().size(); ++i )
    {
        if(    m_viewer.datasets()[i]->visible()
            && !m_viewer.datasets()[i]->partitions().empty()
            && ( !id || m_viewer.datasets()[i]->partitions().rbegin()->first > *id ) )
        {
            id = m_viewer.datasets()[i]->partitions().rbegin()->first;
        }
    }
    id = id ? *id + 1 : 0;
    m_viewer.handleId( *id );
    m_idEdit->setValue( *id );
}

CheckBox::CheckBox( boost::function< void( bool ) > f ) : m_f( f )
{
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( action( bool ) ) );
}

void CheckBox::action( bool checked ) {    m_f( checked ); }

void MainWindow::viewerInitialized() { updateFileFrame(); }

void MainWindow::showFileGroup( std::string name, bool shown )
{
    FileGroupMap::iterator it = m_fileGroups.find( name );
    if( it == m_fileGroups.end() ) { std::cerr << "label-points: warning: file group \"" << name << "\" not found" << std::endl; }
    for( std::size_t i = 0; i < it->second.size(); ++i ) { it->second[i].first->setCheckState( shown ? Qt::Checked : Qt::Unchecked ); }
}

void MainWindow::setWritableFileGroup( std::string name, bool writable )
{
    FileGroupMap::iterator it = m_fileGroups.find( name );
    if( it == m_fileGroups.end() ) { std::cerr << "label-points: warning: file group \"" << name << "\" not found" << std::endl; }
    for( std::size_t i = 0; i < it->second.size(); ++i ) { it->second[i].second->setCheckState( writable ? Qt::Checked : Qt::Unchecked ); }
}

void MainWindow::updateFileFrame()
{
    for( std::size_t i = 0; i < m_viewer.datasets().size(); ++i ) // quick and dirty: clean
    {
        for( unsigned int k = 0; k < 3; ++k )
        {
            if( m_fileLayout->itemAtposition( i + 1, k ) == NULL ) { continue; }
            QWidget* widget = m_fileLayout->itemAtposition( i + 1, k )->widget();
            m_fileLayout->removeWidget( widget );
            delete widget;
        }
    }
    bool sameFields = true;
    std::string fields;
    for( std::size_t i = 0; sameFields && i < m_viewer.datasets().size(); ++i )
    {
        if( i == 0 ) { fields = m_viewer.datasets()[0]->options().fields; } // quick and dirty
        else { sameFields = m_viewer.datasets()[i]->options().fields == fields; }
    }
    m_fileGroups.clear();
    for( std::size_t i = 0; i < m_viewer.datasets().size(); ++i )
    {
        static const std::size_t maxLength = 30; // arbitrary
        std::string filename = m_viewer.datasets()[i]->filename();
        if( filename.length() > maxLength )
        {
            // quick and dirty to avoid boost::filesystem 2 vs boost::filesystem 3 clash
            //std::string leaf = boost::filesystem::path( m_viewer.datasets()[i]->filename() ).leaf();
            #ifdef WIN32
            std::string leaf = comma::split( m_viewer.datasets()[i]->filename(), '\\' ).back();
            #else
            std::string leaf = comma::split( m_viewer.datasets()[i]->filename(), '/' ).back();
            #endif
            filename = leaf.length() >= maxLength ? leaf : std::string( "..." ) + filename.substr( filename.length() - maxLength );
        }
        if( !sameFields ) { filename += ": \"" + m_viewer.datasets()[i]->options().fields + "\""; }
        m_fileLayout->addWidget( new QLabel( filename.c_str() ), i + 1, 0, Qt::AlignLeft | Qt::AlignTop );
        CheckBox* viewBox = new CheckBox( boost::bind( &Viewer::show, boost::ref( m_viewer ), i, _1 ) );
        viewBox->setCheckState( Qt::Checked );
        viewBox->setToolTip( ( std::string( "check to make " ) + filename + " visible" ).c_str() );
        m_fileLayout->addWidget( viewBox, i + 1, 1, Qt::AlignRight | Qt::AlignTop );
        CheckBox* writeBox = new CheckBox( boost::bind( &Viewer::setWritable, boost::ref( m_viewer ), i, _1 ) );
        writeBox->setToolTip( ( std::string( "check to make " ) + filename + " writable" ).c_str() );
        if( i == 0 ) { writeBox->setCheckState( Qt::Checked ); }
        m_fileLayout->addWidget( writeBox, i + 1, 2, Qt::AlignRight | Qt::AlignTop );
        m_fileLayout->setRowStretch( i + 1, i + 1 == m_viewer.datasets().size() ? 1 : 0 );
        m_fileGroups[ m_viewer.datasets()[i]->options().fields ].push_back( std::make_pair( viewBox, writeBox ) );
    }
    std::size_t i = 1 + m_viewer.datasets().size();
    QLabel* filenameLabel = new QLabel( "<b>groups</b>" );
    m_fileLayout->addWidget( filenameLabel, i++, 0, Qt::AlignLeft | Qt::AlignTop );
    for( FileGroupMap::const_iterator it = m_fileGroups.begin(); it != m_fileGroups.end(); ++it, ++i )
    {
        m_fileLayout->addWidget( new QLabel( ( "\"" + it->first + "\"" ).c_str() ), i, 0, Qt::AlignLeft | Qt::AlignTop );
        CheckBox* viewBox = new CheckBox( boost::bind( &MainWindow::showFileGroup, this, it->first, _1 ) );
        viewBox->setToolTip( ( std::string( "check to make files with fields \"" ) + it->first + "\" visible" ).c_str() );
        m_fileLayout->addWidget( viewBox, i, 1, Qt::AlignRight | Qt::AlignTop );
        CheckBox* writeBox = new CheckBox( boost::bind( &MainWindow::setWritableFileGroup, this, it->first, _1 ) );
        writeBox->setToolTip( ( std::string( "check to make files with fields \"" ) + it->first + "\" writable" ).c_str() );
        m_fileLayout->addWidget( writeBox, i, 2, Qt::AlignRight | Qt::AlignTop );
        m_fileLayout->setRowStretch( i, i + 1 == m_viewer.datasets().size() + fields.size() ? 1 : 0 );
    }
}

void MainWindow::closeEvent( QCloseEvent *event )
{
    for( std::size_t i = 0; i < m_viewer.datasets().size(); ++i )
    {
        if( !m_viewer.datasets()[i]->modified() ) { continue; }
        if( m_viewer.datasets()[i]->writable() )
        {
            QMessageBox dialog( this );
            dialog.setText( ( m_viewer.datasets()[i]->filename() + " was modified. Save changes?" ).c_str() );
            dialog.setStandardButtons( QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
            dialog.setDefaultButton( QMessageBox::Save );
            dialog.setEscapeButton( QMessageBox::Discard );
            dialog.setModal( true );
            dialog.exec();
            switch( dialog.buttonRole( dialog.clickedButton() ) )
            {
                case QMessageBox::AcceptRole:
                    m_viewer.dataset( i ).save();
                    break;

                case QMessageBox::RejectRole:
                default:
                    event->ignore();
                    return;

                case QMessageBox::DestructiveRole:
                    break;
            }
        }
        else
        {
            QMessageBox dialog( this );
            dialog.setText( ( std::string( "read-only " ) + m_viewer.datasets()[i]->filename() + " was modified. Discard changes?" ).c_str() );
            dialog.setStandardButtons( QMessageBox::Discard | QMessageBox::Cancel );
            dialog.setEscapeButton( QMessageBox::Discard );
            dialog.setDefaultButton( QMessageBox::Cancel );
            dialog.setModal( true );
            dialog.exec();
            switch( dialog.buttonRole( dialog.clickedButton() ) )
            {
                case QMessageBox::RejectRole:
                default:
                    event->ignore();
                    return;

                case QMessageBox::DestructiveRole:
                    break;
            }
        }
    }
    event->accept();
}

void MainWindow::saveAs()
{
    for( std::size_t i = 0; i < m_viewer.datasets().size(); ++i )
    {
        if( !m_viewer.datasets()[i]->writable() ) { continue; }
        QString filename = QFileDialog::getSaveFileName( this, "Choose a file", m_viewer.datasets()[i]->filename().c_str(), "Files( *.csv *.bin *.txt )" );
        m_viewer.dataset( i ).saveAs( filename.toStdString() );
    }
    updateFileFrame();
    //setWindowTitle( std::string( "label-points: " + filename ).c_str() ); // todo: make it prettier
}

void MainWindow::toggleFileFrame( bool visible )
{
    m_fileFrameVisible = visible;
    if( visible ) { m_fileFrame->show(); } else { m_fileFrame->hide(); }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

} } } // namespace snark { namespace graphics { namespace View {
