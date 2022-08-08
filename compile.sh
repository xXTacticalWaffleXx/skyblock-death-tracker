#!/bin/bash

#author: luna aphelion
#contact: luna-aphelion@outlook.com
#
# this is a very simple script that just runs a single g++ command, its
# just here to save the user the trouble of typing the whole command for
# each update, i found it useful and hope you will too

#Copyright(C) 2022 luna aphelion
#
#This program is free software : you can redistribute itand /or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program.If not, see < https://www.gnu.org/licenses/>.

g++ -O2 main.cpp discord_webhook.cc -lcurl -ljsoncpp -lcurlpp
