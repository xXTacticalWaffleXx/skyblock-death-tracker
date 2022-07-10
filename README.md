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

next to that binary make a config.txt file (an example can be found in the root of this repo)

run the binary

## build instructions
download vs 2019

get vcpkg at vcpkg.io (when installing remember to use the no telemetry option)

run vcpkg intergrate install to link installed libraries with vs 2019 automatically

using vcpkg install libcurl, curlpp and jsoncpp

open the sln file in the root of this repo with vs 2019

change compilation mode to release

press the compile button

find ur exe

host it

if you build this repo on plain msvc or gcc please make a pull request with those instructions

feel free to contact me on

discord: tactical waffle#1560

email: tactical-waffle@outlook.com
