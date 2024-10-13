CC = gcc
CFLAGS = -Wall -Wextra

all: launch_services

launch_services: launch_services.c
	$(CC) $(CFLAGS) -o launch_services launch_services.c

clean:
	rm -f launch_services
