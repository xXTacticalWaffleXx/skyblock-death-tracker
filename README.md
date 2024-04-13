this thing is broken now, i cannot be fucked to fix it

# skyblock death tracker


this program is meant to track a players deaths in hypixel skyblock using a discord webhook

prebuilt versions will come soon but for now you are free to compile it yourself

please remember this code is licenced under the agplv3 meaning that if you modify and distribute it you have to
A: give me credit 
B: give others the same rights as you 
C: hosting counts as distribution, meaning if you make a modified version say one that works on matrix you have to distribute that

there are more things but those are the most important

## usage instruction
get a binary in its own folder

next to that binary make a config.txt file (an example can be found in the root of this repo one will also be made if you run the program without a config.txt)

run the binary with no switches if you want the program to track a players death

run the binary with the -m switch if you want to send a message through the webhook etc to announce downtime or to make a message telling people the bot has started

## build instructions windows (vs 2019)
download vs 2019

get vcpkg at vcpkg.io (when installing remember to use the no telemetry option)

run vcpkg intergrate install to link installed libraries with vs 2019 automatically

using vcpkg install libcurl, curlpp and jsoncpp

create a new project for a command line c++ app (feel free to call it whatever you want)

create two more files discord_webhook.cc and discord_webhook.h, copy the contents of each of these files into the files in the vs 2019 project (copy the contents of main.cpp into the cpp file with the same name as your project)

change compilation mode to release

press the compile button

find ur exe

## build instructions on unix

install the libraries needed with your distros package manager 

in this case libcurl jsoncpp and curlpp (these might be called different things depending on your distro)

install g++ with your package manager

download and run download.sh and compile.sh (make sure you run them in a new folder just to make sure you dont over write any files)

if building fails because it cant access json/json.h try editing line 24 of hoopa deathh bot.cpp to say jsoncpp/json/json.h instead of json/json.h

if you build this repo on plain msvc please make a pull request with those instructions

compile.sh and download.sh are scripts that can be used to well, download and compile the code if you dont want to write out the whole compile command every time you update the program and the download script can be used to get the code if you dont want to fuck about with using git or downloading files manually

feel free to contact me on

discord: tactical waffle#1560

email: tactical-waffle@outlook.com
