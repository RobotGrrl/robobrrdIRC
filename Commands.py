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
#
# module for configuration and commands
#
from Arduinos import Arduino
import time

# Program Settings
DEBUG = True

# IRC Settings
NICK = "ROBOBRRD3000"
SERVER = "irc.freenode.net"
CHANNEL = "#GeekStreams"
#SERVER = "chat1.ustream.tv"
#CHANNEL = "#robot-party"
PORT = 6667

# Arduino Settings
#USB_PATH = '/dev/tty.FireFly-514C-SPP' # bt
#BAUD = 115200
#USB_PATH = '/dev/tty.usbserial-A9007M4I' # wired MANOI
#BAUD = 9600

#USB_PATH = '/dev/tty.usbserial-A6T8R6YA' # wired YODA
#BAUD = 9600 

#USB_PATH = '/dev/tty.usbserial-FTE4Z66C' # xb
#BAUD = 9600

USB_PATH = '/dev/tty.usbserial-A40082TP' # adk
BAUD = 9600

# ---
# RoboBrrd

def flapwings(arduino):
    print "Flap Wings"
    arduino.send('*')
    arduino.send('FW')
    arduino.send('*')
    return ""

def dance1(arduino):
    print "Dance 1"
    arduino.send('*')
    arduino.send('D1')
    arduino.send('*')
    return ""

def dance2(arduino):
    print "Dance 2"
    arduino.send('*')
    arduino.send('D2')
    arduino.send('*')
    return ""

def blink(arduino):
    print "Blink"
    arduino.send('*')
    arduino.send('BL')
    arduino.send('*')
    return ""

def asymptote(arduino):
    print "Word winner!"
    arduino.send('*')
    arduino.send('WORD')
    arduino.send('*')
    return ""

def chirp1(arduino):
    print "Chirp1"
    arduino.send('*')
    arduino.send('C1')
    arduino.send('*')
    return ""

def ir(arduino):
    print "IR"
    arduino.send('*')
    arduino.send('IR')
    arduino.send('*')

    r = arduino.read(1)
    while(r != "&"):
        r = arduino.read(1)
        
    if(r == "&"):
        return arduino.read(4)
    else:
        return "oops"

def wave(arduino):
    print "Wave"
    arduino.send('*')
    arduino.send('WAV')
    arduino.send('*')
    return ""

def help(arduino):
	return ""

