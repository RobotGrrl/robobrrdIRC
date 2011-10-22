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
import serial

class Arduino():
	
	def __init__(self, path='/dev/tty.usbserial', baud=9600):
		self.ser = serial.Serial(path, baud)	
	
	def send(self, data):
		self.ser.write(data)
		
	def read(self, bytes):
		while (1):
			if (self.ser.inWaiting() > bytes-1):
				return self.ser.read(bytes)
	
	def flush():
		self.ser.flushInput()
		self.ser.flushInput()