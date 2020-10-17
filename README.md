# Program Assignment 1

## Compile the files
Use the following code in terminal to compile the files:

	make

## Run the code
After files compiled there are two executable files: client and server.
Open two terminal to run the files. Run server first:

	./server

Then run client next:

	./client

The client will asking for running mode. Input 0 to let client send the message you type. The message should not exceed 999 characters. The client will receive ack from server if server is running, or resend the message up to 3 times and then exit.

Input 1 to let the client send 10 messages automatically, and client will send error frames in the last 4 frames corresponding to the assignment requirement. 