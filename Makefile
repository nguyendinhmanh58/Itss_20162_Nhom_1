CC=gcc
CFLAGS=-o
all:
	$(CC) $(CFLAGS) 1nd_floor 1nd_floor.c `pkg-config --cflags --libs gtk+-3.0` -g -w
	$(CC) $(CFLAGS) x_floor x_floor.c `pkg-config --cflags --libs gtk+-3.0` -g -w
	$(CC) $(CFLAGS) liftMng liftMng.c -g -w
	$(CC) $(CFLAGS) liftCtr liftCtr.c -g -w
	$(CC) $(CFLAGS) liftBody liftBody.c -g -w
	$(CC) $(CFLAGS) liftSensor liftSensor.c -g -w
	$(CC) $(CFLAGS) start start.c -g -w
