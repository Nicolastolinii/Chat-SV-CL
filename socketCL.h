#ifndef _SOCKETCL_H_
#define _SOCKETCL_H_
#include "global.h"

void ReceiveMessages(void *param);

char mensajes[MAX_BUFFER_SERVER_CLIENT];
char name_user[30];
char ip[16]; // strlen("255.255.255.255") == 16

#include "socketCL.c"
#endif