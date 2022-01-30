#include <memory>
#include <chrono>
#include <cstdlib>
#include <restbed>
#include <iostream>
#include "json.hpp"
#include <sstream>


using namespace std;
using namespace restbed;
using namespace nlohmann;

 
using json = nlohmann::json;


     auto name ="harshana";
     auto address ="fgsgs";

   

    json j = json{{"name", "harsha"}, {"address", address}};

    json history1= {
  {"pi", 3.141},
  {"happy", true},
  {"name", "Niels"},
  {"nothing", nullptr},
  {"answer", {
    {"everything", 42}
  }},
  {"list", {1, 0, 2}},
  {"object", {
    {"currency", "USD"},
    {"value", 42.99}
  }}
};

   json history ={

       {
     { "title", "to the vimantha car"},
      {"category", "Fuel"},
      {"cost", "4500"},
      {"date", "1/26/2022, 2:52:55 AM"},
      {"budget_name", "At open day event of company Abc"},
      {"id", 1}
    },
    {
      {"title", "To the singer -sanuka"},
      {"category", "Entertaintment"},
      {"cost", "60000"},
      {"date", "1/26/2022, 2:54:23 AM"},
      {"budget_name", "At open day event of company Abc"},
      {"id",2}
    },
   } ;


   json expenses ={

 {
   { "title", "Electricity Bill"},
    {"description", "fefsdf dscs sdsf"},
    {"allocation", NULL},
    {"current_allocation", "1000"},
    {"id", 1}
    },
    {
     { "title", "Water Bill"},
      {"description", "fefsdf dscs sdsf"},
      {"allocation", NULL},
      {"current_allocation", "1000"},
      {"id", 2}
    },

   };

   json income = {

        {
     { "title", "Salary"},
      {"description", "fefsdf dscs sdsf"},
      {"allocation", NULL},
      {"current_allocation", "1000"},
      {"id", 1}
    },
    {
      {"title", "private business"},
      {"description", "fefsdf dscs sdsf"},
      {"allocation", NULL},
      {"current_allocation", "1000"},
      {"id", 2}
    },
   };
     
    

void get_handler (const shared_ptr< Session > session){


   
    const auto& request = session->get_request();
    
    const auto operation = request->get_path_parameter("operation");
    const auto req_header =  request->get_header("Host","u");
    cout << req_header << endl ;
    cout << operation << endl ;
   

   
     ostringstream str_stream;
     auto result =123 ;
        str_stream << result;
       // json jsonResult = {
      //      {"result", str_stream.str()}
      //  };

  //  auto content = jsonResult.dump();
 
  auto content_history = history.dump();
  auto content_expenses = expenses.dump();
  auto content_income = income.dump();

    if(operation == "history") {

    session->close(OK, content_history, 
            {{"Content-Length", to_string(content_history.size())}});


    }else if(operation == "expenses") {

    session->close(OK, content_expenses, 
            {{"Content-Length", to_string(content_expenses.size())}});

    }else if(operation == "income") {

    session->close(OK, content_income, 
            {{"Content-Length", to_string(content_income.size())}});

    
    }else if( operation =="budget"){

        session->close( OK, "Hello, history!", { { "Content-Length", "13" }, { "Connection", "close" } } );
    }





}

void service_ready_handler( Service& )
{
    fprintf( stderr, "Hey! The service is up and running." );
}

void get_json_method_handler( const shared_ptr< Session > session )
{
  cout << "inside json "<< endl ;
    const multimap< string, string > headers
    {
        { "Content-Length", "23" },
        { "Content-Type", "application/json" }
    };
    
    session->close( 200, "{ \"Hello\": \", World!\" }", headers );
}

void failed_filter_validation_handler( const shared_ptr< Session > session )
{
   
    cout << "error json "<< endl ;
    session->close( 400 );
}
int main(const int, const char**){

    auto resource = make_shared< Resource >() ;
   resource-> set_path(
     "/{operation:history|transaction|expenses|income}"); 
      
   resource->set_method_handler("GET" ,
                [&](const auto session){
                   get_handler(session);
               });

    resource->set_method_handler( "POST", 
   
    [&](const auto session){
    get_json_method_handler(session);
    });

    resource->set_failed_filter_validation_handler( failed_filter_validation_handler );
    


    auto settings = make_shared< Settings >();
    settings->set_port(8000);
    //settings->set_bind_address( "localhost" );
    settings->set_default_header("connection" , "close");
    settings->set_default_header("Access-Control-Allow-Origin", "*" );
    cout << resource << endl ;
  

    auto service = make_shared< Service >( );
    service->publish( resource );
    // service->set_ready_handler( service_ready_handler );

    cout << "came the code here " << endl ;   
    service->start( settings );
    cout << "came the code after  start" << endl ;   
    return EXIT_SUCCESS;



}

//   clang++ -o 01_test 01_test.cpp -l restbed -l jsoncpp
//  ./01_test.cpp