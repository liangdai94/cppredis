#!/usr/bin/python

""" rediscpp.py - A client for the Redis daemon.
"""

__author__ = "Dailiang"
__copyright__ = "Copyright 2021, Dailiang"
__license__ = "GPL"
__version__ = "0.1"
__date__ = "$LastChangedDate: 2021-06-22 21:42:55 GMT$"


import socket

class RedisError(Exception): pass
class ConnectionError(RedisError): pass

class Redis(object):
	"""The main Rediscpp client.
	"""

	def __init__(self, host=None, port=None, timeout=None, db=None):
	    self.host = host or 'localhost'
	    self.port = port or 9898
	    if timeout:
	        socket.setdefaulttimeout(timeout)
	    self._sock = None
	    self._fp = None
	    self.db = db


	def connect(self):
		if isinstance(self._sock, socket.socket):
            return
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect((self.host, self.port))
        except socket.error, e:
            raise ConnectionError("Error %s connecting to %s:%s. %s." % (e.args[0], self.host, self.port, e.args[1]))
        else:
            self._sock = sock
            self._fp = self._sock.makefile('r')
            if self.db:
                self.select(self.db)

	def disconnect(self):
		if isinstance(self._sock, socket.socket):
            try:
                self._sock.close()
            except socket.error:
                pass
        self._sock = None
        self._fp = None

	def _write(self, s):
        """
			send to socket
        """
        try:
            self._sock.sendall(s)
        except socket.error, e:
            if e.args[0] == 32:
                # broken pipe
                self.disconnect()
            raise ConnectionError("Error %s while writing to socket. %s." % tuple(e.args))
            
    def _read(self):
    	"""
    		recv from socket
    	"""
        try:
            return self._fp.readline()
        except socket.error, e:
            if e.args and e.args[0] == errno.EAGAIN:
                return
            self.disconnect()
            raise ConnectionError("Error %s while reading from socket. %s." % tuple(e.args))
        if not data:
            self.disconnect()
            raise ConnectionError("Socket connection closed when reading.")
        return data

	def get(self, key):
		self.connect()
		self._write('get %s\r\n' % (key))
        return self._read().strip()

	def set(self, key, value):
		self.connect()
		self._write('set %s %s\r\n' % (key, value))
        return self._read().strip()

if __name__ == '__main__':
	import doctest
	doctest.testmod()


