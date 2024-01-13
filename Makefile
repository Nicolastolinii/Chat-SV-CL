CC=gcc
CARGS=-Wall -lws2_32 -g -O0

main:
	$(CC) socketCL.c $(CARGS) -o socketCL.exe
	$(CC) socketSV.c $(CARGS) -o socketSV.exe