

import sys
import subprocess
import time
import io






nsCreationTime = time.time_ns() # similar WIN32 ftCreationTime of process

def micros():
	return int((time.time_ns() - nsCreationTime) / 1E3)

def millis():
	return int((time.time_ns() - nsCreationTime) / 1E6)

def delay(ms):
	time.sleep(ms / 1000)






import msvcrt # https://docs.python.org/3/library/msvcrt.html

class Console_class:
	
	def __init__(self):
		self._connected = True
		self._peek = -1
	
	def connected(self):
		return self._connected;
	
	def available(self):
		return msvcrt.kbhit() # True if msvcrt.kbhit()==1 else False
	
	def peek(self):
		if (self._peek == -1) and (self.available() > 0):
			self._peek = ord(msvcrt.getwch())
		return self._peek
	
	def read(self):
		if (self._peek == -1) and (self.available() > 0):
			self._peek = ord(msvcrt.getwch())
		c = self._peek
		self._peek = -1
		return c

	def write(self, v):
		if (type(v) is str): sys.stdout.write(v)
		elif (type(v) is int): sys.stdout.write(chr(v))
		else: sys.stdout.write("<illegal value>")
	
	def writeln(self):
		sys.stdout.write("\r\n")

	def flush(self):
		sys.stdout.flush()
		
	def print(self, v):
		if (type(v) is str): sys.stdout.write(v)
		elif (type(v) is int): sys.stdout.write(str(v))
		else: sys.stdout.write("<illegal value>")

	def println(self, v=None):
		if (v != None): self.print(v)
		self.writeln()
		
Console = Console_class()

"""
delay(1000)

Console.print("abc")
Console.println()
Console.println("ijk")

Console.println(Console.available())
Console.println(Console.peek())
Console.println(Console.read())
Console.println(Console.read())
"""
"""
print(Console.available(), flush=True)
print(Console.peek(), flush=True)
print(Console.read(), flush=True)
print(Console.read(), flush=True)
"""
"""
sys.exit(0)
"""
"""
Console.println("éèà");
"""






#print(millis(), flush=True)
Console.println(millis())







def main(argv):
	# main process
	
	#print("hello", flush=True)
	Console.println("hello")
	Console.flush()


# https://docs.python.org/fr/3/library/__main__.html

if __name__ == "__main__":
	# execute only if run as a script
	main(sys.argv)


# https://stackoverflow.com/questions/49709309/prevent-python-prints-automatic-newline-conversion-to-crlf-on-windows
new_stdout = io.TextIOWrapper(sys.stdout.buffer, newline='\n', encoding=sys.stdout.encoding, errors=sys.stdout.errors); old_stdout, sys.stdout = sys.stdout, new_stdout


"""
# https://stackoverflow.com/questions/34960955/print-lf-with-python-3-to-windows-stdout
stdout = open(sys.__stdout__.fileno(), mode=sys.__stdout__.mode, buffering=1, encoding=sys.__stdout__.encoding, errors=sys.__stdout__.errors, newline='\n', closefd=False)
"""


import serial

ser = serial.Serial(port="COM3", baudrate=115200, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=1.0, xonxoff=False, rtscts=False, write_timeout=None, dsrdtr=False, inter_byte_timeout=None) 

#ser.open()


#print(millis(), flush=True)
Console.println(millis())
Console.flush()


ser.timeout = 0.1

#print(ser.read(768).decode(), end='')
Console.print(ser.read(768).decode())
Console.flush()


ser.timeout = 1.0


#print(millis(), flush=True)
Console.println(millis())
Console.flush()


ser.write(b"start\r\n")
ser.flush()


#print(millis(), flush=True)
Console.println(millis())
Console.flush()


#print(ser.read(10).decode(), end='')
#print(ser.read(1).decode(), end='', flush=True)
Console.print(ser.read(1).decode())
Console.flush()
ser.timeout = 0.1
#print(ser.read(9).decode(), end='', flush=True)
Console.print(ser.read(9).decode())
Console.flush()


#print(millis(), flush=True)
Console.println(millis())
Console.flush()


ser.write(b"print\r\n")
ser.flush()


#print(millis(), flush=True)
Console.println(millis())
Console.flush()


ser.timeout = 8.0


#print(ser.read(768).decode(), end='')
#print(ser.read(1).decode(), end='', flush=True)
Console.print(ser.read(1).decode())
Console.flush()
ser.timeout = 0.1
#print(ser.read(767).decode(), end='', flush=True)
Console.print(ser.read(767).decode())
Console.flush()


#print(millis(), flush=True)
Console.println(millis())
Console.flush()


ser.close()
