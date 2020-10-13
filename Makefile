.SUFFIXES:.c .o

CC=gcc

SRCS1=client.c message.c
OBJS1=$(SRCS1:.c=.o)
EXEC1=client

SRCS2=server.c message.c
OBJS2=$(SRCS2:.c=.o)
EXEC2=server

start: $(OBJS1) $(OBJS2)
	$(CC) -o $(EXEC1) $(OBJS1)
	$(CC) -o $(EXEC2) $(OBJS2)
	@echo "-----------------------------OK-----------------------"

.c.o:
	$(CC) -o $@ -c $< -std=c99

clean:
	rm -rf $(EXEC1) $(OBJS1)
	rm -rf $(EXEC2) $(OBJS2)