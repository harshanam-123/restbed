#include <memory>
#include <future>
#include <cstdio>
#include <cstdlib>
#include <restbed>
#include <iostream>
using namespace std;
using namespace restbed;
void print( const shared_ptr< Response >& response )
{
    fprintf( stderr, "*** Response ***\n" );
    fprintf( stderr, "Status Code:    %i\n", response->get_status_code( ) );
    fprintf( stderr, "Status Message: %s\n", response->get_status_message( ).data( ) );
    fprintf( stderr, "HTTP Version:   %.1f\n", response->get_version( ) );
    fprintf( stderr, "HTTP Protocol:  %s\n", response->get_protocol( ).data( ) );
    for ( const auto header : response->get_headers( ) )
    {
        fprintf( stderr, "Header '%s' > '%s'\n", header.first.data( ), header.second.data( ) );
    }
    const string& cLength = "Content-Length";
    const string& valCon = "0";
    auto length = response->get_header( cLength, valCon );
    Http::fetch( length, response );
    fprintf( stderr, "Body: %.*s...\n\n", 8000, response->get_body( ).data( ) );
    //fprintf( stderr, response->get_body( ).data( ) );
    cout << response->get_body( ).data( ) << endl;
}
    int main( const int, const char** )
{
    auto request = make_shared< Request >( Uri( "http://localhost:8000/history/" ) );
    request->set_header( "Accept", "*/*" );
    request->set_header( "Host", "http://localhost:8000" );
    request->set_header( "Access-Control-Allow-Origin", "*" );
    auto response = Http::sync( request );
    print( response );
    auto future = Http::async( request, [ ]( const shared_ptr< Request >, const shared_ptr< Response > response )
    {
        fprintf( stderr, "Printing async response\n" );
        print( response );
    } );
   future.wait( );
    return EXIT_SUCCESS;


    
}