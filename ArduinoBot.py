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

from ircbot import SingleServerIRCBot
from irclib import nm_to_n, nm_to_h, irc_lower, ip_numstr_to_quad, ip_quad_to_numstr

import inspect 
import Commands
from Arduinos import Arduino

class ArduinoBot(SingleServerIRCBot):
    
	def __init__(self, arduino, channel, nickname, server, port=6667):
		SingleServerIRCBot.__init__(self, [(server, port)], nickname, nickname)
		self.channel = channel
		self.arduino = arduino

	def on_nicknameinuse(self, c, e):
		c.nick(c.get_nickname() + "_")

	def on_welcome(self, c, e):
		c.join(self.channel)

	def on_privmsg(self, c, e):
		self.do_command(e, e.arguments()[0])

 	def on_pubmsg(self, c, e):
  		"""
      	This function is called eveytime a public message is sent to the channel
		and ArduinoIRCBot hears it. 
		you can get the source and message with (nick, message) = split_event(e)    
 		"""
		(nick, message) = split_event(e)
		try:
			cmd = message.split(':')
			if cmd[0].lstrip().rstrip() == c.get_nickname():
				if Commands.DEBUG:
					print "%s sent me %s" % (nick, message)
				response = self.do_command(nick, cmd[1].lstrip().rstrip()) ## make sure no spaces before or after cmd
				if Commands.DEBUG:
					print 'response = %s\n' % response
				self.send_pubmsg(c, response)
				arduino.flush()
				arduino.flushInput()
				arduino.flushOutput()
		except:
			pass #ignore, message not for ArduinoBot, could log convo here   
		return

	def send_pubmsg(self, c, message):
		"""Sends a public message to the forum"""
 		c.privmsg(self.channel, message)

	def do_command(self, source, cmd):
   		"""
        	This is where we will handle the so-called 'commands' to the Arduino
 		"""
 		#c = self.connection
		
		try:
			cmd = cmd.split(' ')
			if Commands.DEBUG:
				print "cmd: %s" % cmd
			if len(cmd) > 1:
				return getModuleCallables()[cmd[0]](self.arduino, cmd[1:])
			else:
				return getModuleCallables()[cmd[0]](self.arduino)
		except:
			return ""#I don't care about that function"
		
		
	
def getModuleCallables():
	reload(Commands)
	if Commands.DEBUG:
		print "Callables: %s" % dict(inspect.getmembers(Commands, callable))
	return dict(inspect.getmembers(Commands, callable))

def split_event(e):
	"""Splits an event into the source (nickname) and message"""
	return (nm_to_n(e.source()), e.arguments()[0])      
