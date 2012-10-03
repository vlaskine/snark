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

#ifdef WIN32
#include <WinSock2.h>
#endif

#include <vector>
#include <boost/shared_ptr.hpp>
#include <qapplication.h>
#include <Qt3D/qcolor4ub.h>
#include <comma/application/command_line_options.h>
#include <comma/csv/options.h>
#include <comma/name_value/parser.h>
#include <comma/string/string.h>
#include "snark/graphics/applications/label_points/MainWindow.h"

void usage()
{
    std::cerr << std::endl;
    std::cerr << "simple tool for manual labeling of point clouds" << std::endl;
    std::cerr << std::endl;
    std::cerr << "usage: label-points [<options>] <filenames>" << std::endl;
    std::cerr << std::endl;
    std::cerr << "<options>" << std::endl;
    std::cerr << "    --background-colour <colour> : e.g. #ff0000, default: #000000 (black)" << std::endl;
    std::cerr << "    --fix-duplicated : if present, re-label with the same id all duplicated points" << std::endl;
    std::cerr << "    --orthographic : if present, use orthographic projection instead of perspective projection" << std::endl;
    std::cerr << "    --fov <fov> : set camera field of view to <fov>, in degrees. Only has effect for perspective projection. Default: 45 degrees" << std::endl;
    std::cerr << "    --repair : if present, repair and save files without bringing up gui;" << std::endl;
    std::cerr << "               currently only re-label duplicated points" << std::endl;
    std::cerr << comma::csv::options::usage() << std::endl;
    std::cerr << std::endl;
    std::cerr << "<fields>" << std::endl;
    std::cerr << "    required fields: x,y,z,id" << std::endl;
    std::cerr << "    default: x,y,z,id" << std::endl;
    std::cerr << std::endl;
    std::cerr << "examples" << std::endl;
    std::cerr << "    label-points scan.csv" << std::endl;
    std::cerr << "    label-points scan.csv --fields=,,,x,y,z,,id" << std::endl;
    std::cerr << "    label-points semantic_labels.csv partitions.csv" << std::endl;
    std::cerr << "    label-points 'scan.csv;fields=x,y,z,,id' 'scan.csv;fields=x,y,z,id'" << std::endl;
    std::cerr << std::endl;
    exit( -1 );
}

int main( int argc, char** argv )
{
    try
    {
        comma::command_line_options options( argc, argv );
        if( argc == 1 || options.exists( "--help" ) || options.exists( "-h" ) ) { usage(); }
        comma::csv::options csvOptions( argc, argv );
        QColor4ub backgroundcolour( QColor( QString( options.value< std::string >( "--background-colour", "#000000" ).c_str() ) ) );
        if( csvOptions.fields == "" ) { csvOptions.fields = "x,y,z,id"; }
        std::vector< std::string > files = options.unnamed( "--repair,--fix-duplicated",
                                                            "--binary,--bin,-b,--fields,--delimiter,-d,--background-colour,--precision,--orthographic,--fov" );
        std::vector< comma::csv::options > datasetcsvOptions;
        bool fixDuplicated = options.exists( "--fix-duplicated" );        
        for( std::size_t i = 0; i < files.size(); ++i )
        {
            datasetcsvOptions.push_back( comma::name_value::Parser( "filename" ).get( files[i], csvOptions ) );
        }
        if( options.exists( "--repair" ) ) // quick and dirty
        {
            for( std::size_t i = 0; i < datasetcsvOptions.size(); ++i ) { snark::graphics::View::Dataset::repair( datasetcsvOptions[i] ); }
            return 0;
        }
        else
        {
            QApplication application( argc, argv );
            bool orthographic = options.exists( "--orthographic" );
            double fieldOfView = options.value< double >( "--fov", 45 );
            boost::scoped_ptr< snark::graphics::View::Viewer > viewer( new snark::graphics::View::Viewer( datasetcsvOptions, fixDuplicated, backgroundcolour, orthographic, fieldOfView ) );
            snark::graphics::View::MainWindow mainWindow( comma::join( argv, argc, ' ' ), viewer.get() );
            mainWindow.show();
            /*return*/ application.exec();
            viewer.reset(); // HACK to prevent segfault on exit TODO fix properly or maybe it is a bug in Qt3D ?
            return 1;
        }
    }
    catch( std::exception& ex )
    {
        std::cerr << "label-points: " << ex.what() << std::endl;
    }
    catch( ... )
    {
        std::cerr << "label-points: unknown exception" << std::endl;
    }
}
