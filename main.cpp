//author: luna aphelion
//contact: luna-aphelion@outlook.com

/*Copyright(C) 2022 luna aphelion

// if editing this code feel free to add your name to the lines above

This program is free software : you can redistribute itand /or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.If not, see < https://www.gnu.org/licenses/>.
*/

#include <curl/curl.h>          
#include <curlpp/cURLpp.hpp>    
#include <curlpp/Options.hpp>   
#include <curlpp/Easy.hpp>      
#include <json/json.h>          
#include <string>               
#include <sstream>   
#include <fstream>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include "discord_webhook.h"

//config varibles
//todo move the run_frequency var to the config.txt
std::string player_name;
std::string players_uuid;
std::string webhook_url;
std::string api_key;
int run_frequency = 3600; //how often you want the program to send a message in seconds default = 3600 (one hour) (remember to change the message sent to reflect this varible)
bool sendmessages = true;
bool pingrole;
std::string roleid;
int pingMin;
bool debug = false;
bool printedUrl = false;
std::string humanReadableFreq;
int profileNum;
std::string admin_id;

int last_run = 0;

void send_message(std::string message, bool exit){
		DiscordWebhook webhook(webhook_url.c_str());
		webhook.send_message(message.c_str());
		std::string hacky_workaround = webhook_url; // hackiest work around award for 2022 goes to luna aphelion
		std::cout << "sent message: " << message << std::endl;
		if (exit){
			std::exit(0);
		}
}

void generate_message()
{
	// set up varibles
	std::string api_json;
	std::string url = "https://api.hypixel.net/skyblock/profiles?key=" + api_key +"&uuid=" + players_uuid;
	curlpp::Easy request;
	std::stringstream ss(api_json);
	curlpp::options::WriteStream ws(&ss);
	Json::Value root;
	Json::Reader reader;

	if (debug & printedUrl) {
		std::cout << "webhook url" << url << std::endl;;
		printedUrl = true;
	}

	// get the json data from the hypixel server
	request.setOpt(new curlpp::options::Url(url));
	request.setOpt(ws);
	request.perform();
	ss << request;
	ss >> api_json;

	// parse the json data and find and print the deaths varible
	reader.parse(api_json, root);
	std::string current_deathcount_string = root["profiles"][profileNum]["members"][players_uuid]["stats"]["deaths"].asString();
	// calculates how many deaths the player has had since there last death and stores an updated death count
	if (debug) {
		std::cout << "stoi str: " << current_deathcount_string << std::endl << "success:" << root["success"] << std::endl;
	}
	int current_deathcount;
	try {
		current_deathcount = std::stoi(current_deathcount_string);
	}
	catch (const std::invalid_argument& ia){
		std::string error_message = "std::invalid_argument detected, shutting down stoi str: " + current_deathcount_string + admin_id;
		send_message(error_message, true);
		std::ofstream os("api_json");
		os << api_json;
		std::cout << "line num: 102";
	}
	std::ifstream data_in("data.txt");
	if (!data_in) {
		std::cout << "data.txt does not exist, creating" << std::endl;
	}
	int old_deathcount;
	data_in >> old_deathcount;
	int deaths_since_last_run = current_deathcount - old_deathcount;
	std::string deaths_since_last_run_str = std::to_string(deaths_since_last_run);
	std::string message;
	message = player_name + " has died " + deaths_since_last_run_str + " times in the last " + humanReadableFreq + " total death count: " + current_deathcount_string;
	std::ofstream os("data.txt");
	if (pingrole && deaths_since_last_run > pingMin) {
		message = message + " <@&" + roleid + ">";
	}
	os << current_deathcount;
	os.close();

	//sends the message string to a discord webhook
	if (deaths_since_last_run > 0){
	//	DiscordWebhook webhook(webhook_url.c_str());
	//	webhook.send_message(message.c_str());
	//	std::cout << "sent message: " << message << std::endl;
	send_message(message, false);
	} else{
		std::cout << "player has not died in the last hour, not sending a message" << std::endl;
	}
}

int main(int argc, char** argv) {


	//if editing this program feel free to add your own name and email to this message
	std::cout << "this program is free software licenced under the gnu AGPL a copy of which can be found at gnu.org/licences" << std::endl
						<< "copyright Luna Aphelion (luna-aphelion@outlook.com) 2022" << std::endl;

	//import settings from config file
	std::ifstream in("config.txt");
	if (!in) {
		std::ofstream os("config.txt");
		os << "uuid" << std::endl
			<< "discord webhook url" << std::endl
			<< "hypixel api key" << std::endl
			<< "players ign" << std::endl
			<< "role id (optional, if this line is present the program will ping a role if the death count since the last execution exceeds the set ammount in line 6)" << std::endl
			<< "minimum deaths for ping (the code will ping the role mentioned above if the player has died more this ammount of times)" << std::endl
			<< "debug code toggle (true/false)" << std::endl
			<< "time between api polls in seconds default is one hour (3600 seconds)" << std::endl
			<< "human readable time between polls (i.e. hour, 1 month, 2 days)" << std::endl
			<< "number of the profile you want to check, to find this number poll the api in a browser and look at the response (more indepth instructions comming soon)"
			<< std::endl
			<< "makesure that you only have the relevent text in the file (remove this line and the text saying what to put where)";

		std::cout << "config.txt doesnt exist, creating and exiting, please be sure to edit it with actual information";

		os.close();
		return 1;
	} 
	// Read the next line from File until it reaches the end.
	std::vector <std::string> settings;
	std::string str;
	while (getline(in, str)) {
		// Line contains string of length > 0 then save it in vector
		if (str.size() > 0)
			settings.push_back(str);
	}
	in.close(); 

	if (settings.size() < 9) {
		std::cout << "you need to set every line in your config.txt" << std::endl;
		return 0;
	}

	players_uuid	= settings.at(0);
	webhook_url		= settings.at(1);
	api_key			= settings.at(2);
	player_name		= settings.at(3);

	//setting the debug var before everything else because i need to reference it in the code below
	if (settings.at(6) == "false") {
		debug = false;
	}
	else if (settings.at(6) == "true") {
		debug = true;
	}

	if (settings.at(4) == "" && settings.at(5) != "") {
		std::cout << "you need to add a role id to your config.txt\n";
		return 1;
	}
	if (settings.at(4)!= "") {
		roleid = (settings.at(4));
		pingrole = true;
		if (settings.at(5) != "") {
			if (debug) {
				std::cout << "stoi str: " << settings.at(5) << std::endl;
			}
			try{
			pingMin = stoi(settings.at(5));
			}
			catch (const std::invalid_argument& ia){
			std::string error_message = "std::invalid_argument detected, shutting down stoi str: " + settings.at(5) + admin_id;
			send_message(error_message, true);
			std::cout << settings.at(5);
			std::cout << "line num: 206";
			}
		}
		else {
			std::cout << "you need to add a minimum death count for pings to your config.txt\n";
			return 1;
		}
	}


	if (debug) {
		std::cout << "stoi str: " << settings.at(7) << std::endl;
	}
	try{
		profileNum = stoi(settings.at(7));
	}
	catch (const std::invalid_argument& ia){
		std::string error_message = "std::invalid_argument detected, shutting down stoi str: " + settings.at(7) + admin_id;
		send_message(error_message, true);
		std::cout << settings.at(7);
		std::cout << "line num: 227";
	}

	admin_id = settings.at(8);

	if (players_uuid == "") { std::cout << "your config.txt does not have a uuid, please put the uuid of the player whos deaths you want to track on line 1"; return 0; }
	if (webhook_url == "")  { std::cout << "your config.txt does not have a webhook url, please put a webhook url of the channel where you want this channel to send the messages on line 2"; return 0;}
	if (api_key == "")		{ std::cout << "your config.txt does not have an api key, please put a valid hypixel api key on line 3"; return 0;}
	if (player_name == "")  { std::cout << "your config.txt does not have a player name, please put the name of the player whos deaths you want to track on line 4"; return 0;}
	
	if (argc > 1){
		std::string arg = argv[1];
		if (arg == "-m"){
			std::cout << "what message do you want to send" << std::endl 
				<< "if you didnt mean to use this option use the ctrl+c interupt" << std::endl;
			std::string user_message;
			getline(std::cin, user_message);
			send_message(user_message, true);
		}
	}

	while (true) {
		time_t now = time(0);
		if (now > last_run + run_frequency) {
			generate_message();
			last_run = now;
		}
	}
}
