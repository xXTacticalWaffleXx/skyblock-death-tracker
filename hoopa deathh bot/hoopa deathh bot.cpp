//author: luna aphelion
//contact: luna-aphelion@outlook.com

/*Copyright(C) 2022 luna aphelion

This program is free software : you can redistribute itand /or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
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
#include "discord_webhook.h"

//config varibles
std::string player_name = "ohoopa";
std::string players_uuid = "cdae37235d294582bce572a570c19345";
std::string webhook_url = "";
std::string api_key = "";
int run_frequency = 3600; //how often you want the program to send a message in seconds default = 3600 (one hour) (remember to change the message sent to reflect this varible)

int last_run = 0;

void send_message()
{
	// set up varibles
	std::string api_json;
	std::string url = "https://api.hypixel.net/skyblock/profiles?key=" + api_key +"&uuid=" + players_uuid;
	curlpp::Easy request;
	std::stringstream ss(api_json);
	curlpp::options::WriteStream ws(&ss);
	Json::Value root;
	Json::Reader reader;

	// get the json data from the hypixel server
	request.setOpt(new curlpp::options::Url(url));
	request.setOpt(ws);
	request.perform();
	ss << request;
	ss >> api_json;

	// parse the json data and find and print the deaths varible
	reader.parse(api_json, root);
	std::string current_deathcount_string = root["profiles"][2]["members"][players_uuid]["stats"]["deaths"].asString();

	// calculates how many deaths the player has had since there last death and stores an updated death count
	int current_deathcount = stoi(current_deathcount_string);
	std::ifstream data_in("data.txt");
	if (!data_in) {
		std::cout << "shits buggered cunt";
	}
	int old_deathcount;
	data_in >> old_deathcount;
	int deaths_since_last_run = current_deathcount - old_deathcount;
	std::string deaths_since_last_run_str = std::to_string(deaths_since_last_run);
	std::string message;
	message = player_name + " has died " + deaths_since_last_run_str + " times in the last hour " + "total death count: " + current_deathcount_string;
	std::ofstream os("data.txt");
	os << current_deathcount;
	os.close();
	std::cout << message;

	//sends the message string to a discord webhook
	DiscordWebhook webhook(webhook_url.c_str());
	webhook.send_message(message.c_str());
}

int main() {
	while (true) {
		time_t now = time(0);
		if (now > last_run + run_frequency) {
			send_message();
			last_run = now;
		}
	}
}