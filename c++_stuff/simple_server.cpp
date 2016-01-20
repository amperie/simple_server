
#include <iostream>
#include <map>
#include <string>
#include <boost/asio.hpp>
#include <utility>
#include <curl/curl.h>
#include <unistd.h>
#include "/opt/appd/appdynamics-sdk-native/sdk_lib/appdynamics.h"

using namespace std;
using boost::asio::ip::tcp;

const char APP_NAME[] = "C++SDK_Lab";
const char TIER_NAME[] = "C++Server";
const char NODE_NAME[] = "C++ServerNode";
const char CONTROLLER_HOST[] = "";
const int CONTROLLER_PORT = 8090;
const char CONTROLLER_ACCOUNT[] = "customer1";
const char CONTROLLER_ACCESS_KEY[] = "";
const int CONTROLLER_USE_SSL = 0;
const char third_tier[] = "ThirdTier"; 

const string third_tier_URL = "http://localhost:8080/back_tier/back_tier";

void execute_sql (string SQL)
{
	
	//Pretend to do something
	usleep(100000);
	cout << "SQL call: " << SQL << "\n";
}

pair<string,string> leftright_string (string str, string delimiter)
{
	int pos = str.find(delimiter);
	string left = str.substr(0,pos);
	string right = str.substr(pos+delimiter.size(),str.size());
	pair<string,string> retVal (left,right);
	return retVal;
}	

map<string,string> parse_request (string request)
{ 
	//Parse the request that comes in
	//Requests that come in have the format:
	//TYPE=<type>|DATA=<data>|OTHER_FIELDS...|
	//This function parses the incoming request into a map
	map<string,string> retVal;
	string token;

	while (( request.find("|")) != string::npos)
	{
		token = leftright_string(request,"|").first;
		retVal.insert(leftright_string(token,"="));
		request = leftright_string(request,"|").second;
	}
	
	return retVal;
}

string dispatch_request (map<string,string> request)
{
	string retVal="";
	string key = "TYPE";
	string reqType = request[key];
	CURL *curl;
	CURLcode res;
	struct curl_slist *headers = NULL;
	
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, third_tier_URL.c_str());
	headers = curl_slist_append(headers, "ComesFrom: C++ Server");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	string corrH = request["singularityheader"];

	appd_bt_handle hBT = appd_bt_begin(reqType.c_str(),corrH.c_str());
	appd_bt_add_user_data(hBT,"SomeData","SomeVALUE");


	if (reqType=="HEARTBEAT"){
		retVal="beating";
		usleep(100000);
		return retVal;}
	if (reqType=="UPDATE"){
		appd_exitcall_handle hEX = appd_exitcall_begin(hBT,"ThirdTier");
		const char* cHD = appd_exitcall_get_correlation_header(hEX);
		
		string tmpStr(cHD);
                tmpStr = "singularityheader: " + tmpStr;
	
		headers = curl_slist_append(headers, tmpStr.c_str());
		res=curl_easy_perform(curl);
		execute_sql("Update this_n_that_table where dude=0");
		appd_exitcall_end(hEX);
		retVal="updating";}
	if (reqType=="TEST"){
		res=curl_easy_perform(curl);
		usleep(200000);
		retVal="testing";}
	if (res != CURLE_OK){
		retVal="HTTP Call Return Code: ";
		retVal.append (curl_easy_strerror(res));
	}

	cout << curl_easy_strerror(res);
	curl_easy_cleanup(curl);
	curl_slist_free_all(headers);
	appd_bt_end(hBT);

	return retVal;
}

string process_request (string request)
{
	map<string,string> parsed = parse_request(request);
	return dispatch_request(parsed);
}

void start_server ()
{

	boost::system::error_code error;
	boost::asio::io_service io_service;
	
	tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(),4555));
	string received_request;
	for (;;)
	{
		char data[1024];
		boost::system::error_code error;
		boost::asio::ip::tcp::socket sock(io_service);
		a.accept(sock);
		size_t length = sock.read_some(boost::asio::buffer(data),error);
		if (error == boost::asio::error::eof)
			break;
		data[length]='\0';
		received_request=string(data);
		cout << "Received: " << received_request << "\n";
		string retVal = process_request(received_request);
		cout << "Result: " << retVal << "\n";
	}

}

int main(int argc, char* argv[])
{
	appd_config cfg;
    	appd_config_init(&cfg);
    	cfg.app_name = (char*)APP_NAME;
    	cfg.tier_name = (char*)TIER_NAME;
    	cfg.node_name = (char*)NODE_NAME;
    	cfg.controller.host = (char*)CONTROLLER_HOST;
    	cfg.controller.port = CONTROLLER_PORT;
    	cfg.controller.account = (char*)CONTROLLER_ACCOUNT;
    	cfg.controller.access_key = (char*)CONTROLLER_ACCESS_KEY;
    	cfg.controller.use_ssl = CONTROLLER_USE_SSL;
	appd_sdk_init(&cfg);

	
        appd_backend_declare(APPD_BACKEND_HTTP, "ThirdTier");
        appd_backend_set_identifying_property("ThirdTier","Name","ThirdTierProperty");
        appd_backend_add("ThirdTier");

	
	start_server();
}


