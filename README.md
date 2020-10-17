# Program Assignment 2
This directory includes the source code, input file (query.txt), database file (Verification_Database.txt).
The results of the communications are printed to the terminal and can be seen in the run time.

## Compile the files
Use the following code in terminal to compile the files:

	make

## Run the code
After files compiled there are two executable files: client and server.
Open two terminal to run the files. Run server first:

	./server

Then run client next:

	./client

The client will asking for running mode. Input 0 to let client send the phone number and technology that you type. The client will receive ack from server if server is running, or resend the message up to 3 times and then exit.

Input 1 to let the client send 4 messages automatically, which are saved in query.txt. It will send a correct one, one without paid,  one with wrong technology, and one with wrong phone number.