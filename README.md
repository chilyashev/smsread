smsread
=======

A simple application for getting all the SMSes stored on the Huawei mobile modems.
I made it, because my provider only has a Windows application, and I need to check the messages from time to time :)


Building and running
====================

0. Get the code:

	$ git clone https://github.com/chilyashev/smsread.git

1. make:

	$ make

	Cleaning...
	Making smsread...
	g++ -Wall -o bin/smsread main.cpp
	
2. If all went well, run as **root**:
	**#** bin/smsread

