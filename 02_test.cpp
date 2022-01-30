#include <memory>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <restbed>
#include <iostream>

using namespace std;
using namespace restbed;

class CustomLogger : public Logger
{
    public:
        void stop( void )
        {
            return;
        }
        
        void start( const shared_ptr< const Settings >& )
        {
            return;
        }
        
        void log( const Level, const char* format, ... )
        {
            va_list arguments;
            va_start( arguments, format );
            
            vfprintf( stderr, format, arguments );
            fprintf( stderr, "\n" );
            
            va_end( arguments );
        }
        
        void log_if( bool expression, const Level level, const char* format, ... )
        {
            if ( expression )
            {
                va_list arguments;
                va_start( arguments, format );
                log( level, format, arguments );
                va_end( arguments );
            }
        }
};

void get_method_handler( const shared_ptr< Session > session )
{

    cout << "insddie"<< endl;
    session->close( OK, "Hello, World!", { { "Content-Length", "13" } } );
}

int main( const int, const char** )
{
    auto resource = make_shared< Resource >( );
    resource->set_path( "/history" );
    resource->set_method_handler( "POST", get_method_handler );
    
    auto settings = make_shared< Settings >( );
    settings->set_port( 8000 );
    settings->set_default_header( "Connection", "close" );
    
    Service service;
    service.publish( resource );
    //service.set_logger( make_shared< CustomLogger >( ) );
    
    service.start( settings );
    
    return EXIT_SUCCESS;
}