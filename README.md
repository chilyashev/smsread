smsread
=======

A simple application for getting all the SMSes stored on the Huawei mobile modems.
I made it, because my provider only has a Windows application, and I need to check the messages from time to time :)


Building and running
====================

0. Get the code:
	`$ git clone https://github.com/chilyashev/smsread.git`

1. make:  
```
 $ make  

 Cleaning...
 Making smsread...
 g++ -Wall -o bin/smsread main.cpp
```
	
2. If all went well, run as **root**:  
`# ./bin/smsread`  

You should see something like this:

```
 # ./bin/smsread 
 Sending stuff...
 
 Message #0 From: +000001234567 on "13/01/06 at 21:06:06+08"
 Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus tincidunt quam nec leo accumsan a blandit  
 dui bibendum. Duis id nunc a dolor aliquet sodales. Mauris pellentesque lacinia vestibulum...

```


TODO
====

 * Make the reading from the device better (i.e smarter, faster). 
 * Parse the response in a smarter way
