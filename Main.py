## Copyright (c) 2009 Benjamin Eckel        
##
## This is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
## 
## This is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## with this; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#! /usr/bin/env python
import Commands 

from ArduinoBot import *
from Arduinos import Arduino     

def main():

	bot = ArduinoBot(Arduino(Commands.USB_PATH, Commands.BAUD), Commands.CHANNEL, Commands.NICK, Commands.SERVER, Commands.PORT)
	bot.start()

if __name__ == "__main__":
	main()


        
