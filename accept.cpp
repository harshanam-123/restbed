#include <string>
#include <memory>
#include <cstdlib>
#include <restbed>
#include <iostream>


using namespace std;
using namespace restbed;

void get_xml_method_handler( const shared_ptr< Session > session )
{
    const multimap< string, string > headers
    {
        { "Content-Length", "30" },
        { "Content-Type", "application/xml" }
    };
    
    session->close( 200, "<hello><world></world></hello>", headers );
}

void get_json_method_handler( const shared_ptr< Session > session )
{

    cout << "came to json handler"<< endl ;
    const multimap< string, string > headers
    {
       
        { "Content-Type", "application/json" }
    };
    
    session->close( 200, "{ \"Hello\": \", World!\" }", headers );
}

void failed_filter_validation_handler( const shared_ptr< Session > session )
{
cout << "came here"<< endl ;
    session->close( 400 );
}

int main( const int, const char** )
{
    auto resource = make_shared< Resource >( );
    resource->set_path( "/history" );
    resource->set_failed_filter_validation_handler( failed_filter_validation_handler );
    resource->set_method_handler( "GET", { { "Accept", "application/xml" }, { "Content-Type", "application/xml" } }, &get_xml_method_handler );
    resource->set_method_handler( "GET", { {"Accept","*/*"}, { "Content-Type", "application/json" } }, &get_json_method_handler );
    
    auto settings = make_shared< Settings >( );
    settings->set_port( 8000 );
    settings->set_default_header( "Connection", "close" );
    
    Service service;
    service.publish( resource );
    service.start( settings );
    
    return EXIT_SUCCESS;
}
//clang++ -o accept accept.cpp -l restbed
// ./accept