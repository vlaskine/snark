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

#include <comma/Application/CommandLineOptions.h>
#include <comma/Base/Types.h>
#include <comma/Csv/Options.h>
#include <comma/Csv/Stream.h>
#include <comma/NameValue/Parser.h>
#include <comma/String/String.h>
#include <rur/graphics/Applications/ViewPointsQt/MainWindow.h>
#include <rur/graphics/Applications/ViewPointsQt/Viewer.h>
#include <rur/graphics/Applications/ViewPointsQt/ShapeReader.h>
#include <rur/graphics/Applications/ViewPointsQt/ModelReader.h>
#include <rur/graphics/Applications/ViewPointsQt/TextureReader.h>
#include <QApplication>

void usage()
{
    std::cerr << std::endl;
    std::cerr << "view 3D point clouds:" << std::endl;
    std::cerr << "view points from given files/streams and stdin" << std::endl;
    std::cerr << "(see examples below for a quick start)" << std::endl;
    std::cerr << std::endl;
    std::cerr << "usage: view-points [<options>] [<filenames>]" << std::endl;
    std::cerr << std::endl;
    std::cerr << "<options>" << std::endl;
    std::cerr << "    --size <size> : render last <size> points (or other shapes)" << std::endl;
    std::cerr << "                    default 2000000 for points, for 200000 for other shapes" << std::endl;
    std::cerr << "    --background-colour <colour> : e.g. #ff0000, default: #000000 (black)" << std::endl;
    std::cerr << "    --camera=\"<options>\"" << std::endl;
    std::cerr << "          <options>: [<fov>];[<type>]" << std::endl;
    std::cerr << "          <fov>: field of view in degrees, default 45 degrees" << std::endl;
    std::cerr << "          <type>: orthographic | perspective" << std::endl;
    std::cerr << "              default: perspective" << std::endl;
    std::cerr << "    --fov=<fov>: set camera field of view in degrees" << std::endl;
    std::cerr << "    --orthographic: use orthographic projection instead of perspective" << std::endl;
    std::cerr << "    --camera-position=\"<options>\"" << std::endl;
    std::cerr << "          <options>: <position>|<stream>" << std::endl;
    std::cerr << "          <position>: <x>,<y>,<z>,<roll>,<pitch>,<yaw>" << std::endl;
    std::cerr << "          <stream>: position csv stream with options; default fields: x,y,z,roll,pitch,yaw" << std::endl;
    
    std::cerr << "    --colour,--color,-c <how>: how to colour points" << std::endl;
    std::cerr << "        <how>:" << std::endl;
    std::cerr << "            <min>:<max>: if scalar field present, stretch" << std::endl;
    std::cerr << "                         by scalar (see section <fields>)" << std::endl;
    std::cerr << "            white, black, red, green, blue, yellow, cyan, magenta, grey, pink, sky, salad: fixed colour" << std::endl;
    std::cerr << "            if there is \"id\" field in --fields, colour by id, with or without scalar (see section <fields>)" << std::endl;
    std::cerr << "            [<min>:<max>][,<from-colour>:<to-colour>][,cyclic][,sharp][,quadratic] (in any order)" << std::endl;
    std::cerr << "                stretch colours by elevation" << std::endl;
    std::cerr << "                <min>:<max>: from-to in metres, e.g. -3:10" << std::endl;
    std::cerr << "                <from-colour>:<to-colour>: from which to which colour" << std::endl;
    std::cerr << "                cyclic: if present, colour cyclically with <min> and <max>" << std::endl;
    std::cerr << "                        meaning the height of the corresponding stip" << std::endl;
    std::cerr << "                sharp: if present (for cyclic only), do not make smooth borders" << std::endl;
    std::cerr << "                       between the colours" << std::endl;
    std::cerr << "                quadratic: if present (for cyclic only), parabolic stretching, quick and dirty" << std::endl;
    std::cerr << "                            default: stretch colours linearly" << std::endl;
    std::cerr << "                e.g.: cat data.csv | view-points --colour=blue:yellow,1:2,cyclic" << std::endl;
    std::cerr << "            default: stretched by elevation from cyan to magenta from 0:1" << std::endl;
    std::cerr << "    --label <label>: text label displayed next to the latest point" << std::endl;
    std::cerr << "    --point-size <point size>: default: 1" << std::endl;
    std::cerr << "    --image-size <width>,<height>: image size in meters when displaying images in the scene" << std::endl;
    std::cerr << "    --shape <shape>: \"point\", \"extents\", \"line\", \"label\"; default \"point\"" << std::endl;
    std::cerr << "                     \"ellipse\": e.g. --shape=ellipse --fields=,,centre,orientation,minor,major," << std::endl;
    std::cerr << "                                  default orientation: in x,y plane" << std::endl;
    std::cerr << "                     \"extents\": e.g. --shape=extents --fields=,,min,max,,," << std::endl;
    std::cerr << "                     \"line\": e.g. --shape=line --fields=,,first,second,,," << std::endl;
    std::cerr << "                     \"label\": e.g. --shape=label --fields=,x,y,z,,,label" << std::endl;
    std::cerr << "    --z-is-up : z-axis is pointing up, default: pointing down ( north-east-down system )" << std::endl;
    std::cerr << comma::Csv::Options::usage() << std::endl;
    std::cerr << std::endl;
    std::cerr << "    fields:" << std::endl;
    std::cerr << "        default: x,y,z" << std::endl;
    std::cerr << "        x,y,z: coordinates (%d in binary)" << std::endl;
    std::cerr << "        id: if present, colour by id (%ui in binary)" << std::endl;
    std::cerr << "        block: if present, clear screen once block id changes (%ui in binary)" << std::endl;
    std::cerr << "        r,g,b: if present, specify RGB colour (0-255; %uc in binary)" << std::endl;
    std::cerr << "        a: if present, specifies colour transparency (0-255, %uc in binary); default 255" << std::endl;
    std::cerr << "        scalar: if present, colour by scalar" << std::endl;
    std::cerr << "                  use --colour=<from>:<to>[:<from colour>:<to colour>]" << std::endl;
    std::cerr << "                  default: 0:1:red:blue" << std::endl;
    std::cerr << "                  todo: implement for shapes (currently works only for points)" << std::endl;
    std::cerr << "        label: text label (currenly implemented for ascii only)" << std::endl;
    std::cerr << "        roll,pitch,yaw: if present, show orientation" << std::endl;
    std::cerr << std::endl;
    std::cerr << "    most of the options can be set for individual files (see examples)" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Examples:" << std::endl;
    std::cerr << "    view-points scan.csv" << std::endl;
    std::cerr << "    cat $(ls *.csv) | view-points --size=200000" << std::endl;
    std::cerr << "    cat $(ls *.bin) | view-points --size=200000 --binary \"%d%d%d\"" << std::endl;
    std::cerr << "    view-points --colour merry $(ls labeled.*.csv)" << std::endl;
    std::cerr << "    cat file.csv | view-points --fields=\"x,y,z,r,g,b\"" << std::endl;
    std::cerr << "    view-points \"raw.csv;colour=0:20\" \"partitioned.csv;fields=x,y,z,id\";point-size=2" << std::endl;
    std::cerr << "    echo \"0,0,0\" | ./bin/view-points-qt --shape /usr/local/etc/segway.shrimp.obj --z-is-up --orthographic" << std::endl;
    std::cerr << std::endl;
    exit( -1 );
}

// quick and dirty, todo: a proper structure, as well as a visitor for command line options
boost::shared_ptr< comma::graphics::View::Reader > makeReader( QGLView& viewer
                                                           , const comma::CommandLineOptions& options
                                                           , const comma::Csv::Options& csvOptions
                                                           , const std::string& properties = "" )
{
    QColor4ub backgroundcolour( QColor( QString( options.value< std::string >( "--background-colour", "#000000" ).c_str() ) ) );
    comma::Csv::Options csv = csvOptions;
    std::string shape = options.value< std::string >( "--shape", "point" );
    std::size_t size = options.value< std::size_t >( "--size", shape == "point" ? 2000000 : 200000 );
    unsigned int pointSize = options.value( "--point-size", 1u );
    std::string colour = options.exists( "--colour" ) ? options.value< std::string >( "--colour" ) : options.value< std::string >( "-c", "-10:10" );
    std::string label = options.value< std::string >( "--label", "" );
    if( properties != "" )
    {
        comma::NameValue::Parser nameValue( "filename", ';', '=', false );
        csv = nameValue.get( properties, csvOptions );
        comma::NameValue::Map m( properties, "filename", ';', '=' );
        size = m.value( "size", size );
        pointSize = m.value( "point-size", pointSize );
        shape = m.value( "shape", shape );
        if( m.exists( "colour" ) ) { colour = m.value( "colour", colour ); }
        else if( m.exists( "color" ) ) { colour = m.value( "color", colour ); }
        label = m.value( "label", label );
    }
    comma::graphics::View::coloured* coloured = comma::graphics::View::colourFromString( colour, csv.fields, backgroundcolour );
    if( shape == "point" )
    {
        if( csv.fields == "" ) { csv.fields="x,y,z"; }
        std::vector< std::string > v = comma::split( csv.fields, ',' );
        bool has_orientation = false;
        for( unsigned int i = 0; !has_orientation && i < v.size(); ++i ) { has_orientation = v[i] == "roll" || v[i] == "pitch" || v[i] == "yaw"; }
        return boost::shared_ptr< comma::graphics::View::Reader >( new comma::graphics::View::ShapeReader< Eigen::Vector3d >( viewer, csv, size, coloured, pointSize, label ) );
    }
    if( shape == "label" )
    {
        if( csv.fields == "" ) { csv.fields="x,y,z,label"; }
        // TODO
    }
    else if( shape == "ellipse" )
    {
        if( csv.fields == "" ) { csv.fields="centre,orientation,major,minor"; }
        std::vector< std::string > v = comma::split( csv.fields, ',' );
        for( std::size_t i = 0; i < v.size(); ++i )
        {
            if( v[i] == "x" || v[i] == "y" || v[i] == "z" ) { v[i] = "centre/" + v[i]; }
            else if( v[i] == "roll" || v[i] == "pitch" || v[i] == "yaw" ) { v[i] = "orientation/" + v[i]; }
        }
        csv.fields = comma::join( v, ',' );
    }
    else if( shape == "extents" )
    {
        if( csv.fields == "" ) { csv.fields="min,max"; }
    }
    else if( shape == "line" )
    {
        if( csv.fields == "" ) { csv.fields="first,second"; }
    }
    else
    {
        std::vector< std::string > v = comma::split( shape, '.' );
        if( v.size() < 2 )
        {
            COMMA_THROW( comma::graphics::exception, "expected shape, got \"" << shape << "\"" ); 
        }
        if( csv.fields == "" ) { csv.fields="point,orientation"; }
        if( v[1] == "png" || v[1] == "jpg" || v[1] == "jpeg" || v[1] == "bmp" || v[1] == "gif" )
        {
            std::string size = options.value< std::string >( "--image-size", "3,3" );
            std::vector< std::string > sizeVector = comma::split( size, ',' );
            if( v.size() != 2 )
            {
                COMMA_THROW( comma::graphics::exception, "expected image size as width,height" );
            }
            double width = boost::lexical_cast< double >( sizeVector[0] );
            double height = boost::lexical_cast< double >( sizeVector[0] );            
            return boost::shared_ptr< comma::graphics::View::Reader >( new comma::graphics::View::TextureReader( viewer, csv, shape, width, height ) );
        }
        else
        {
            return boost::shared_ptr< comma::graphics::View::Reader >( new comma::graphics::View::ModelReader( viewer, csv, shape, options.exists( "--z-is-up" ), coloured, label ) );
        }
    }
    std::vector< std::string > v = comma::split( csv.fields, ',' );
    for( std::size_t i = 0; i < v.size(); ++i )
    {
        if(    v[i] != "id"
            && v[i] != "block"
            && v[i] != "colour"
            && v[i] != "label"
            && v[i] != "scalar"
            && v[i] != "" ) { v[i] = "shape/" + v[i]; } }
    csv.fields = comma::join( v, ',' );
    csv.fullXPath = true;
    if( shape == "extents" )
    {
        return boost::shared_ptr< comma::graphics::View::Reader >( new comma::graphics::View::ShapeReader< comma::Extents< Eigen::Vector3d > >( viewer, csv, size, coloured, pointSize, label ) );
    }
    else if( shape == "line" )
    {
        return boost::shared_ptr< comma::graphics::View::Reader >( new comma::graphics::View::ShapeReader< std::pair< Eigen::Vector3d, Eigen::Vector3d > >( viewer, csv, size, coloured, pointSize, label ) );
    }
    else if( shape == "ellipse" )
    {
        return boost::shared_ptr< comma::graphics::View::Reader >( new comma::graphics::View::ShapeReader< comma::graphics::View::Ellipse< 25 > >( viewer, csv, size, coloured, pointSize, label ) );
    }
    COMMA_THROW( comma::graphics::exception, "expected shape, got \"" << shape << "\"" ); // never here
}

int main( int argc, char** argv ) 
{
    try
    {
        comma::CommandLineOptions options( argc, argv );
        if( options.exists( "--help" ) || options.exists( "-h" ) ) { usage(); }
        comma::Csv::Options csvOptions( argc, argv );
        std::vector< std::string > properties = options.unnamed( "--z-is-up,--orthographic"
                , "--binary,--bin,-b,--fields,--size,--delimiter,-d,--colour,-c,--point-size,--image-size,--background-colour,--shape,--label,--camera,--camera-position,--fov,--model,--full-xpath" );
        QColor4ub backgroundcolour( QColor( QString( options.value< std::string >( "--background-colour", "#000000" ).c_str() ) ) );
        boost::optional< comma::Csv::Options > cameraCsv; 
        boost::optional< Eigen::Vector3d > cameraposition;
        boost::optional< Eigen::Vector3d > cameraorientation;

        bool cameraOrthographic = options.exists( "--orthographic" );
        double fieldOfView = options.value< double >( "--fov" , 45 );
        if( options.exists( "--camera" ) )
        {
            std::string camera = options.value< std::string >( "--camera" );
            std::vector< std::string > v = comma::split( camera, ';' );
            for( std::size_t i = 0; i < v.size(); ++i )
            {
                if( v[i] == "orthographic" )
                {
                    cameraOrthographic = true;
                }
                else if( v[i] == "perspective" )
                {
                    cameraOrthographic = false;
                }
                else
                {
                    std::vector< std::string > vec = comma::split( v[i], '=' );
                    if( vec.size() == 2 && vec[0] == "fov" )
                    {
                        fieldOfView = boost::lexical_cast< double >( vec[1] );
                    }
                }
            }
        }

        QApplication application( argc, argv );
        bool z_up = options.exists( "--z-is-up" );
        if( options.exists( "--camera-position" ) )
        {
            std::string position = options.value< std::string >( "--camera-position" );
            comma::NameValue::Parser parser( "x,y,z,roll,pitch,yaw", ',', '=', false );
            comma::graphics::View::PointWithorientation pose;
            try
            {
                pose = parser.get< comma::graphics::View::PointWithorientation >( position );
                cameraposition = pose.point;
                cameraorientation = pose.orientation;
            }
            catch( ... ) {}
            if( !cameraposition )
            {
                comma::NameValue::Parser parser( "filename", ';', '=', false );
                try
                {
                    std::cerr << " parse " << position << std::endl;
                    cameraCsv = parser.get< comma::Csv::Options >( position );
                    cameraCsv->fullXPath = false;
                    if( cameraCsv->fields.empty() )
                    {
                        cameraCsv->fields = "x,y,z,roll,pitch,yaw";
                    }
                }
                catch( ... ) {}
            }
        }
        comma::graphics::View::Viewer* viewer = new comma::graphics::View::Viewer( backgroundcolour, fieldOfView, z_up, cameraOrthographic, cameraCsv, cameraposition, cameraorientation );
        bool stdinAdded = false;
        for( unsigned int i = 0; i < properties.size(); ++i )
        {
            if( comma::split( properties[i], ';' )[0] == "-" ) { stdinAdded = true; }
            viewer->readers.push_back( makeReader( *viewer, options, csvOptions, properties[i] ) );
        }
        if( !stdinAdded )
        {  
            csvOptions.filename = "-";
            viewer->readers.push_back( makeReader( *viewer, options, csvOptions ) );
        }
        comma::graphics::View::MainWindow mainWindow( comma::join( argv, argc, ' ' ), viewer );
        mainWindow.show();
        /*return*/ application.exec();
        delete viewer;
    }
    catch( std::exception& ex )
    {
        std::cerr << "view-points: " << ex.what() << std::endl;
    }
    catch( ... )
    {
        std::cerr << "view-points: unknown exception" << std::endl;
    }
}
